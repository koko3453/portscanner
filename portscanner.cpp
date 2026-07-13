#include <iostream>
#include <string>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <vector>
using namespace std;

struct OpenPort
{
    int port;
    string service;
};

bool isPortOpen(const string &ip, int port)
{

    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    if (sock == INVALID_SOCKET)
    {
        return false;
    }

    sockaddr_in target{};
    target.sin_family = AF_INET;
    target.sin_port = htons(port);

    if (inet_pton(AF_INET, ip.c_str(), &target.sin_addr) <= 0)
    {
        closesocket(sock);
        return false;
    }

    bool open = false;
    if (connect(sock, (sockaddr *)&target, sizeof(target)) == 0)
    {
        open = true;
    }
    closesocket(sock);

    return open;
}

int main(int argc, char *argv[])
{
    if (argc != 2 && argc != 4)
    {
        std::cout << "Usage:\n";
        std::cout << "portscanner.exe <IP>\n";
        std::cout << "portscanner.exe <IP> <start_port> <end_port>\n";
        return 1;
    }

    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        std::cerr << "Failed to initialize Winsock\n";
        return 1;
    }

    string ip = argv[1];
    int startPort;
    int endPort;
    vector<OpenPort> openPortList;

    if (argc == 2)
    {
        startPort = 1;
        endPort = 65535;
    }
    else
    {
        startPort = stoi(argv[2]);
        endPort = stoi(argv[3]);
    }

    if (startPort < 1 || endPort > 65535 || startPort > endPort)
    {
        cout << "Invalid port range\n";
        return 1;
    }

    for (int port = startPort; port <= endPort; ++port)
    {
        cout << "scanning port " << port << "\r";
        if (isPortOpen(ip, port))
        {
            OpenPort entry;
            entry.port = port;

            servent *service = getservbyport(htons(port), "tcp");

            if (service)
                entry.service = service->s_name;
            else
                entry.service = "unknown";

            openPortList.push_back(entry);

            cout << "Port " << port << " is OPEN\n";
        }
    }
    for (auto &p : openPortList)
    {
        cout << "Port: " << p.port
             << " Service: " << p.service << endl;
    }
    WSACleanup();
    return 0;
}
