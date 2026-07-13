# TCP Port Scanner

A simple TCP port scanner written in C++ using the Windows Winsock API. The program attempts to establish TCP connections to specified ports on a target host to determine whether they are open or closed.

The scanner works by creating a TCP socket and using the `connect()` function to test each port. If a connection is successfully established, the port is considered open.

## Features
- Scans a range of TCP ports
- Detects open ports by attempting TCP connections
- Uses Windows Winsock networking API
- Provides basic port status output


windows compile:
g++ .\portscanner.cpp -o portscanner -lws2_32
