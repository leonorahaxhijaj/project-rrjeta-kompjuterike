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
if (FD_ISSET(http_socket, &read_fds))
    handleHttpRequest(http_socket);

// Iterate through all connected clients
for (auto it = clients.begin(); it != clients.end(); ) {
    SOCKET id = it->first;

    // Check if this client has sent data
    if (FD_ISSET(id, &read_fds)) {
        char buf[4096] = {0};

        // Receive data from client
        int r = recv(id, buf, 4096, 0);

        // If connection is closed or error occurred
        if (r <= 0) {
            cout << "U shkeput: " << it->second.ip << endl;

            // If admin disconnected, reset admin socket
            if (admin_client == id)
                admin_client = INVALID_SOCKET;

            // Close socket and remove client from map
            closesocket(id);
            it = clients.erase(it);
            continue;
        }

        // If client is NOT admin, apply delay (rate limiting / anti-spam)
        if (id != admin_client)
            Sleep(1500);