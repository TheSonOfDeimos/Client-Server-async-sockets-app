#include "udp-server.hpp"

UDPServer::UDPServer(const std::string t_host, int t_port, MessageProcessor t_message_processor)
{
    m_udp_acceptor.onMessageReceived = std::bind(&UDPServer::newMessageHandler, this, &m_udp_acceptor, t_message_processor, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
    m_udp_acceptor.bind(t_host, t_port, std::bind(&UDPServer::errorHandler, this, std::placeholders::_1, std::placeholders::_2));
}

UDPServer::~UDPServer()
{
    m_udp_acceptor.closeRaw();
}

void UDPServer::newMessageHandler(UDPAcceptor *t_acceptor, MessageProcessor t_message_processor, std::string t_message, std::string t_host, uint16_t t_port) const
{
    std::cout << "[INFO] New message from [" << t_host << ":" << t_port << "] => " << t_message << "\n";
    std::string message_to_send = t_message_processor(t_message);
    t_acceptor->writeTo(message_to_send, t_host, t_port);
}

void UDPServer::errorHandler(int t_error_code, std::string t_error_message) const
{
    std::cout << "[ERROR] " << t_error_code << " : " << t_error_message << "\n";
}