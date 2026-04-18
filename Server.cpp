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

