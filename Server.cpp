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
      
// HTTP setup
 // Check if there is an incoming HTTP request

// Convert received buffer into a string message
string input(buf);

// Update client activity timestamp (last time seen online)
it->second.last_seen = time(0);

// Increase request counter for this client
it->second.requests++;

// Save message into log (limit message to 200 chars for safety/log size control)
messages_log.push_back({
    it->second.ip,
    input.substr(0, 200),
    get_time_now()
});

// Prepare response string (to be filled later based on command)
string response = "";

// Check if current client is admin
bool is_admin = (id == admin_client);

// If message starts with '/', treat it as a command
if (input[0] == '/') {
    stringstream ss(input);
    string cmd, arg1, arg2;

    // Extract command and first argument
    ss >> cmd >> arg1;

    // Read the rest of the line as second argument (if any)
    getline(ss, arg2);

    // Remove leading space from arg2 if it exists
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
                        }else {
                            response = "Komande e panjohur ose e paautorizuar.";
                        } 
    }

}
    return 0;
}
