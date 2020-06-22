#ifndef UDP_CLIENT_H
#define UDP_CLIENT_H

#include <udpSocket.h>
#include <iostream>

class UDPClient
{
public:
    UDPClient(const std::string t_host, int t_port);
    ~UDPClient();
    void write(std::string t_message);
   
private:
    void newMessageHandler(std::string t_message, std::string t_host, uint16_t t_port);
    void errorHandler(int t_error_code, std::string t_error_message) const;
    UDPSocket m_socket;
};

#endif