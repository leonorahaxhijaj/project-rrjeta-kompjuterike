#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>

#pragma comment(lib, "ws2_32.lib")
using namespace std;

int main() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2, 2), &wsaData);

    SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_port = htons(7777);
    inet_pton(AF_INET, "172.16.108.31", &addr.sin_addr);

    if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        cout << "Lidhja deshtoi!" << endl;
        return 1;
    }

    cout << "U lidhet me Serverin!" << endl;

    string line;
    while (true) {
        cout << "Komanda: ";
        getline(cin, line);
        if (line == "exit") break;

        send(sock, line.c_str(), (int)line.length(), 0);

        char buf[4096] = {0};
        int r = recv(sock, buf, 4096, 0);
        if (r > 0) cout << "Serveri: " << buf << endl;
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}