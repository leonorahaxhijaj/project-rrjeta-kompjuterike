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
#pragma comment(lib, "ws2_32.lib") 
using namespace std;

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
    return 0;
}
