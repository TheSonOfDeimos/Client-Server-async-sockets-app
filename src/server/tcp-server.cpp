#include "tcp-server.hpp"

TCPServer::TCPServer(const std::string t_host, int t_port, MessageProcessor t_message_processor)
{
    m_tcp_acceptor.onNewConnection = std::bind(&TCPServer::newConnectionHandler, this, t_message_processor, std::placeholders::_1);
    m_tcp_acceptor.bind(t_host, t_port, std::bind(&TCPServer::errorHandler, this, std::placeholders::_1, std::placeholders::_2));
    m_tcp_acceptor.listen(std::bind(&TCPServer::errorHandler, this, std::placeholders::_1, std::placeholders::_2));
}

TCPServer::~TCPServer()
{
    m_tcp_acceptor.closeRaw();
}

void TCPServer::newConnectionHandler(MessageProcessor t_message_processor, TCPSocket *t_new_client)
{
    std::cout << "[INFO] New client [" << t_new_client->remoteAddress() << ":" << t_new_client->remotePort() << "]\n";

    t_new_client->onMessageReceived = std::bind(&TCPServer::newMessageHandler, this, t_new_client, t_message_processor, std::placeholders::_1);
    t_new_client->onSocketClosed = std::bind(&TCPServer::stopConnectionHandler, this, t_new_client);
}

void TCPServer::newMessageHandler(TCPSocket *t_self, MessageProcessor t_message_processor, std::string t_message) const
{
    std::cout << "[INFO] New message from [" << t_self->remoteAddress() << ":" << t_self->remotePort() << "] => " << t_message << "\n";
    std::string message_to_send = t_message_processor(t_message);
    t_self->write(message_to_send);
}

void TCPServer::stopConnectionHandler(TCPSocket *t_self) const
{
    std::cout << "[INFO] Stop connection with [" << t_self->remoteAddress() << ":" << t_self->remotePort() << "]\n";
}

void TCPServer::errorHandler(int t_error_code, std::string t_error_message) const
{
    std::cout << "[ERROR] " << t_error_code << " : " << t_error_message << "\n";
}
