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