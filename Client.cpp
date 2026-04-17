#include <iostream> 
#include <string>  
#include <winsock2.h>  
#include <ws2tcpip.h>  
#include <fstream>  
#include <vector> 
#include <map>

#pragma comment(lib, "ws2_32.lib")
using namespace std;

#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <fstream>
#include <vector>
#include <map>
#include <ctime>

#pragma comment(lib, "ws2_32.lib")
using namespace std;

int tcp_port = 7777;
int max_clients = 6;

int main() {
    WSADATA data;
    if (WSAStartup(MAKEWORD(2, 2), &data) != 0) {
        cout << "WSAStartup failed\n";
        return 1;
    }

    SOCKET main_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (main_socket == INVALID_SOCKET) {
        cout << "Socket creation failed\n";
        return 1;
    }

    sockaddr_in main_addr{};
    main_addr.sin_family = AF_INET;
    main_addr.sin_addr.s_addr = INADDR_ANY;
    main_addr.sin_port = htons(tcp_port);

    if (bind(main_socket, (sockaddr*)&main_addr, sizeof(main_addr)) < 0) {
        cout << "Bind failed\n";
        return 1;
    }

    listen(main_socket, max_clients);

    cout << "Serveri duke punuar ne portin " << tcp_port << endl;

    while (true) {
        SOCKET client_socket = accept(main_socket, NULL, NULL);
        if (client_socket != INVALID_SOCKET) {
            cout << "Klient u lidh!" << endl;
        }
    }

    closesocket(main_socket);
    WSACleanup();
    return 0;
}
