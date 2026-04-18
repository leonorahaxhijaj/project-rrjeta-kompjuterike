#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <fstream>
#include <sstream>

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

        string to_send = line;
        if (line.find("/upload") == 0) {
            size_t pos = line.find(' ');
            if (pos != string::npos) {
                string fn = line.substr(pos + 1);
                ifstream f(fn);
                if (f) {
                    stringstream ss; ss << f.rdbuf();
                    to_send = "/upload " + fn + " " + ss.str();
                }
            }
        }

        int s = send(sock, to_send.c_str(), (int)to_send.length(), 0);

        if (s == SOCKET_ERROR) {
            cout << "Lidhja ka rene (Timeout). Duke u rilidhur automatikisht..." << endl;

            closesocket(sock);
            sock = socket(AF_INET, SOCK_STREAM, 0);

            if (connect(sock, (struct sockaddr*)&addr, sizeof(addr)) != SOCKET_ERROR) {
                cout << "Rilidhja e suksesshme!" << endl;
                send(sock, to_send.c_str(), (int)to_send.length(), 0);
            } else {
                cout << "Serveri nuk po pergjigjet. Provo me vone." << endl;
                continue;
            }
        }

        char buf[4096] = {0};
        int r = recv(sock, buf, 4096, 0);
        if (r > 0) cout << "Serveri: " << buf << endl;
    }

    closesocket(sock);
    WSACleanup();
    return 0;
}