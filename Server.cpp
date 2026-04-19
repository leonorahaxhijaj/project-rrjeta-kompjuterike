#include <iostream> 
#include <string>  
#include <winsock2.h>  
#include <ws2tcpip.h>  
#include <fstream>  
#include <vector> 
#include <map>
#include <ctime>
#include <sstream>
#include <windows.h> 
#include <sys/stat.h>
#include <filesystem>

#pragma comment(lib, "ws2_32.lib") 
using namespace std;

const string server_ipAddres = "172.16.108.31"; 
int tcp_port = 7777; 

int http_port = 8080; 
int max_clients = 6;  
int timeout_seconds = 300; 

struct Client {
    SOCKET socket;
    string ip;
    time_t last_seen;
    int requests;
};

struct MessageLog {
    string ip;
    string msg;
    string time_str;
};

map<SOCKET, Client> clients;
vector<MessageLog> messages_log; 
SOCKET admin_client = INVALID_SOCKET; 

string get_time_now() {
    time_t now = time(0);
    struct tm tstruct;
    char buf[80];
    localtime_s(&tstruct, &now);
    strftime(buf, sizeof(buf), "%H:%M:%S", &tstruct);
    return string(buf);
}

void handleHttpRequest(SOCKET http_socket);

int main(){

    WSADATA data; 
    WSAStartup(MAKEWORD(2, 2), &data); 

    SOCKET main_socket = socket(AF_INET, SOCK_STREAM, 0); 
    sockaddr_in main_addr; 
    main_addr.sin_family = AF_INET; 
    main_addr.sin_addr.s_addr = INADDR_ANY; 
    main_addr.sin_port = htons(tcp_port); 
    bind(main_socket, (struct sockaddr*)&main_addr, sizeof(main_addr)); 
    
    listen(main_socket, max_clients);

    SOCKET http_socket = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in http_addr;
    http_addr.sin_family = AF_INET;
    http_addr.sin_addr.s_addr = INADDR_ANY;
    http_addr.sin_port = htons(http_port); 
    bind(http_socket, (struct sockaddr*)&http_addr, sizeof(http_addr));
    listen(http_socket, 5);

    cout << "SERVERI NE PUNE..." << endl;
    cout << "TCP Port: " << tcp_port << " | HTTP Port: " << http_port << endl;
   
    while (true) {
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(main_socket, &read_fds);
        FD_SET(http_socket, &read_fds);
        
        for (auto const& [id, c] : clients) FD_SET(id, &read_fds);

        struct timeval tv = {1, 0};
        select(0, &read_fds, NULL, NULL, &tv);

        if (FD_ISSET(main_socket, &read_fds)) {
            sockaddr_in addr; 
            int len = sizeof(addr);
            SOCKET new_sock = accept(main_socket, (struct sockaddr*)&addr, &len);

            if (clients.size() >= (size_t)max_clients) {
                string m = "Serveri plot!\n";
                send(new_sock, m.c_str(), m.length(), 0);
                closesocket(new_sock);
            } else {
                char ip[INET_ADDRSTRLEN];
                inet_ntop(AF_INET, &addr.sin_addr, ip, INET_ADDRSTRLEN);
                clients[new_sock] = {new_sock, string(ip), time(0), 0};
               
                if (admin_client == INVALID_SOCKET) {
                    admin_client = new_sock;
                    cout << "[SISTEMI] Admin i ri: " << ip << endl;
                } else {
                    cout << "[SISTEMI] User i ri (Read-Only): " << ip << endl;
                }
            }
        }

        if (FD_ISSET(http_socket, &read_fds)) handleHttpRequest(http_socket);
        for (auto it = clients.begin(); it != clients.end(); ) {
            SOCKET id = it->first;
            
            if (FD_ISSET(id, &read_fds)) { 
                char buf[4096] = {0}; 
                int r = recv(id, buf, 4096, 0); 
                
                if (r <= 0) {
                    cout << "U shkeput: " << it->second.ip << endl;
                    if (admin_client == id) admin_client = INVALID_SOCKET;
                    closesocket(id); 
                    it = clients.erase(it); 
                    continue;
                }

                if (id != admin_client) Sleep(1500); 

                string input(buf);
                it->second.last_seen = time(0);
                it->second.requests++;

                messages_log.push_back({
                    it->second.ip,
                    input.substr(0, 200),
                    get_time_now()
                });

                string response = "";
                bool is_admin = (id == admin_client);

                if (input[0] == '/') {
                    stringstream ss(input);
                    string cmd, arg1, arg2;

                    ss >> cmd >> arg1;
                    getline(ss, arg2);

                    if (!arg2.empty())
                        arg2 = arg2.substr(1);

                    if (!is_admin && (cmd == "/upload" || cmd == "/delete" || cmd == "/download")) {
                        response = "GABIM: Ju nuk jeni Admin! Keni vetem read() permission.";
                    } else {
                        if (cmd == "/list") {
                            WIN32_FIND_DATAA f; HANDLE h = FindFirstFileA("*", &f);
                            if (h != INVALID_HANDLE_VALUE) {
                                do { 
                                    response += string(f.cFileName) + " | "; 
                                } while (FindNextFileA(h, &f));
                                FindClose(h);
                            }
                        } else if (cmd == "/read") {
                            ifstream f(arg1);
                            if (f) { stringstream s; s << f.rdbuf(); response = s.str(); }
                            else response = "File nuk u gjet.";
                        } else if (cmd == "/upload") {
                            ofstream f(arg1); f << arg2; response = "SUKSES: File u ngarkua.";
                        } else if (cmd == "/delete") {
                            response = (remove(arg1.c_str()) == 0) ? "SUKSES: File u fshi." : "GABIM: Fshirja deshtoi.";
                        } else if (cmd == "/download") {
                            ifstream file(arg1); 
                            if (file) {
                                stringstream buffer;
                                buffer << file.rdbuf(); 
                            } else {
                                response = "GABIM: Skedari nuk ekziston ne Server.";
                            }
                        } else if (cmd == "/search") {
                            string keyword = arg1; 
                            response = "";
                            for (const auto& entry : std::filesystem::directory_iterator(".")) {
                                string filename = entry.path().filename().string();
                                if (filename.find(keyword) != string::npos) {
                                    response += filename + " | ";
                                }
                            }       
                            if (response == "") {
                                response = "Nuk u gjet asnje file qe permban: " + keyword;
                            } else {
                                response = "Rezultatet e kerkimit: " + response;
                            }
                        } else if (cmd == "/info") {
                            struct stat info;
                            if (stat(arg1.c_str(), &info) == 0) {
                                        
                                struct tm tm_create;
                                char create_time[80];
                                localtime_s(&tm_create, &info.st_ctime);
                                strftime(create_time, sizeof(create_time), "%d/%m/%Y %H:%M:%S", &tm_create);

                                struct tm tm_mod;
                                char mod_time[80];
                                localtime_s(&tm_mod, &info.st_mtime);
                                strftime(mod_time, sizeof(mod_time), "%d/%m/%Y %H:%M:%S", &tm_mod);

                                response = "\n--- INFO PER: " + arg1 + " ---\n";
                                response += "Madhesia: " + to_string(info.st_size) + " bytes\n";
                                response += "Krijuar me: " + string(create_time) + "\n";
                                response += "Modifikuar me: " + string(mod_time) + "\n";
                                response += "--------------------------";
                            } else {
                                response = "GABIM: Skedari nuk u gjet.";
                            }
                        } else {
                            response = "Komande e panjohur ose e paautorizuar.";
                        } 
                    }

                } else {
                    response = is_admin ? "[ADMIN] Mesazhi u mor." : "[USER] Mesazhi u mor.";
                }

                string final_r = response + "\n";
                send(id, final_r.c_str(), (int)final_r.length(), 0);

            }
            it++;
        }

        time_t now = time(0);
        for (auto it = clients.begin(); it != clients.end(); ) {
            if (now - it->second.last_seen > timeout_seconds) {
                cout << "[SISTEMI] U largua per inaktivitet: " << it->second.ip << endl;
                if (admin_client == it->first) admin_client = INVALID_SOCKET;
                closesocket(it->first);
                it = clients.erase(it);
            } else {
                it++;
            }
        }
    }

    return 0;
}

void handleHttpRequest(SOCKET http_socket) {
    SOCKET conn = accept(http_socket, NULL, NULL);
    char b[1024]; recv(conn, b, 1024, 0);
    
    string body = "{\"status\":\"Online\",\"lidhjet\":" + to_string(clients.size()) + ",\"logs\":[";
    for(auto const& m : messages_log) {
        body += "{\"ip\":\"" + m.ip + "\",\"msg\":\"" + m.msg + "\"},";
    }
    if(!messages_log.empty()) body.pop_back();
    body += "]}";

    string res = "HTTP/1.1 200 OK\r\nContent-Type: application/json\r\nContent-Length: " + to_string(body.length()) + "\r\n\r\n" + body;
    send(conn, res.c_str(), (int)res.length(), 0);
    closesocket(conn);
}