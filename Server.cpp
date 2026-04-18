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
#include <sys/stat.h>

#pragma comment(lib, "ws2_32.lib") 
using namespace std;
//leonis-pjesa 1
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
//leonis-pjesa 2

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
    
    //leonis-pjesa 3
    while (true) {
        fd_set read_fds;
        FD_ZERO(&read_fds);
        FD_SET(main_socket, &read_fds);
        FD_SET(http_socket, &read_fds);
        
        for (auto const& [id, c] : clients) FD_SET(id, &read_fds);

        struct timeval tv = {1, 0};
        select(0, &read_fds, NULL, NULL, &tv);

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
                        }
    }
    return 0;
}