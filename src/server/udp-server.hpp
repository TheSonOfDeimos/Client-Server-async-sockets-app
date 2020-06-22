#ifndef UDP_SERVER_H
#define UDP_SERVER_H

#include <udpAcceptor.h>
#include <iostream>

class UDPServer
{
public:
    typedef std::function<std::string(const std::string)> MessageProcessor; // Processes input message and returns the answer to be send

    explicit UDPServer(const std::string t_host, int t_port, MessageProcessor t_message_processor);
    ~UDPServer();

private:
    void newMessageHandler(UDPAcceptor* t_acceptor, MessageProcessor t_message_processor, std::string t_message, std::string t_host, uint16_t t_port) const;
    void errorHandler(int t_error_code, std::string t_error_message) const;
    
    UDPAcceptor m_udp_acceptor;
};

#endif