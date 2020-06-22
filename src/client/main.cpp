#include <iostream>

#include "udp-client.hpp"
#include "tcp-client.hpp"

int main(int argc, char *argv[])
{
    if (argc != 4) {
        std::cerr << "Not enoughs input parameters\n";
        return -1;
    }

    std::string host = argv[2];
    int port = std::atoi(argv[3]);

    std::function<void(std::string)> write_func;

    if (!std::strcmp(argv[1], "udp")) {
        UDPClient client(host, port);
        std::cout << "UDP Client run on address " << host << " and port " << port << "\n";

        std::string input;
        getline(std::cin, input);
        while (input != "exit")
        {
            client.write(input);
            getline(std::cin, input);
        }
    }
    else if (!std::strcmp(argv[1], "tcp")) {
        TCPClient client(host, port);
        std::cout << "TCP Client run on address " << host << " and port " << port << "\n";

        std::string input;
        getline(std::cin, input);
        while (input != "exit")
        {
            client.write(input);
            getline(std::cin, input);
        }
    }

    else
    {
        std::cerr << "Invalit protocol name\n";
    }
    
    return 0;
}