#include <iostream>
#include <vector>
#include <sstream>
#include <numeric>

#include <tcpAcceptor.h>
#include <udpAcceptor.h>
#include "tcp-server.hpp"
#include "udp-server.hpp"

std::string processMessage(const std::string t_message)
{
    std::vector<unsigned int> num_vec;

    // Parse to vector
    std::stringstream input_mesage_str_strm;
    input_mesage_str_strm << t_message;
    std::string input_message_sub_str;
    double temp_num;
    while(!input_mesage_str_strm.eof()) 
    {
        input_mesage_str_strm >> input_message_sub_str;
        if(std::stringstream(input_message_sub_str) >> temp_num) {
            if (static_cast<double>(static_cast<unsigned int>(temp_num)) == temp_num) {
                num_vec.push_back(temp_num);
            }
        }
        input_message_sub_str = "";
    }

    // Sort and get sum
    std::sort(num_vec.begin(), num_vec.end());
    unsigned int sum = std::accumulate(num_vec.begin(), num_vec.end(), 0);

    // Convert to output message
    std::stringstream output_mesage_str_strm;
    std::copy(num_vec.begin(), num_vec.end(), std::ostream_iterator<int>(output_mesage_str_strm, " "));
    output_mesage_str_strm << "\n" << sum;

    return output_mesage_str_strm.str();
}

int main(int argc, char *argv[])
{
    if (argc != 3) {
        std::cerr << "Not enoughs input parameters\n";
        return -1;
    }

    std::string host = argv[1];
    int port = std::atoi(argv[2]);

    UDPServer udp_srv(host, port, &processMessage);
    TCPServer tcp_srv(host, port, &processMessage);

    std::cout << "Server is started on address " << host << " and port " << port << "\n";
    
    std::string input;
    getline(std::cin, input);
    while (input != "exit")
    {
        getline(std::cin, input);
    }

    return 0;
}