#ifndef TCP_CLIENT_H
#define TCP_CLIENT_H

#include <tcpSocket.h>
#include <iostream>

class TCPClient
{
public:
    TCPClient(const std::string t_host, int t_port);
    ~TCPClient();
    void write(std::string t_message);
private:
    void newMessageHandler(std::string t_message);
    void errorHandler(int t_error_code, std::string t_error_message) const;
    TCPSocket m_socket;
};

#endif