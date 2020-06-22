#ifndef TCP_SERVER_H
#define TCP_SERVER_H

#include <tcpAcceptor.h>
#include <udpAcceptor.h>
#include <iostream>

class TCPServer
{
public:
    typedef std::function<std::string(const std::string)> MessageProcessor; // Processes input message and returns the answer to be send

    explicit TCPServer(const std::string t_host, int t_port, MessageProcessor t_message_processor);
    ~TCPServer();

private:
    void newConnectionHandler(MessageProcessor t_message_processor, TCPSocket *t_new_client);
    void newMessageHandler(TCPSocket *t_self, MessageProcessor t_message_processor, std::string t_message) const;
    void stopConnectionHandler(TCPSocket *t_self) const;
    void errorHandler(int t_error_code, std::string t_error_message) const;
    
    TCPAcceptor m_tcp_acceptor;
};


#endif