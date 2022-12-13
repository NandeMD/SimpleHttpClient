#include "Socket.hpp"

void sck::SocketCL::parse_url()
{
    const size_t sep_start = URL.find(PROTO_SEP);

    if (sep_start < 0 || sep_start == SIZE_MAX)
    {
        PROTO = "http";
        ADDR_STR = URL;

        return;
    }

    const size_t add_start = sep_start + PROTO_SEP_LEN;

    PROTO = URL.substr(0, sep_start);
    ADDR_STR = URL.substr(add_start, URL.size()-1);
}

void sck::SocketCL::prep_hints()
{
    memset(&hints, 0, sizeof(hints));
    hints.ai_family     = AF_UNSPEC;
    hints.ai_socktype   = SOCK_STREAM;
}

void sck::SocketCL::get_address_in_from_url()
{
    if ((status = getaddrinfo(ADDR_STR.c_str(), PROTO.c_str(), &hints, &res)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
    }

    for(p = res;p != NULL; p = p->ai_next) 
    {
        if (p->ai_family == AF_INET) 
        { // IPv4
            ipv4 = (struct sockaddr_in *)p->ai_addr;
            ipv4_addr = *(p->ai_addr);
        } else 
        { // IPv6
            ipv6 = (struct sockaddr_in6 *)p->ai_addr;
            ipv6_addr = *(p->ai_addr);
        }
    }

    freeaddrinfo(res);
}

void sck::SocketCL::create_socket()
{
    sockfd = socket(ipv4->sin_family, SOCK_STREAM, 0);

    if (sockfd == -1)
    {
        std::cout << "Socket creation failed!\n";
        throw "Socket creation failed!\n";
    }
}

sck::SocketCL::SocketCL(const std::string URL)
{
    prep_hints();

    this->URL = URL;
    parse_url();

    get_address_in_from_url();
    create_socket();
}

void sck::SocketCL::Connect()
{
    int conn = connect(sockfd, &ipv4_addr, sizeof(ipv4_addr));
    
    if (conn == -1)
    {
        std::cout << "Error when connecting to server!\n";
        throw "Error when connecting to server!\n";;
    }

    is_connected = true;
}

void sck::SocketCL::SendMsg(std::string msg)
{   
    const char * message_arr = msg.c_str();
    int len = strlen(message_arr);

    int bytes_sent = send(sockfd, message_arr, len, 0);

    if (bytes_sent == -1)
    {
        std::cout << "Error when sending message!\n";
        throw "Error when sending message!\n";
    }

    std::cout << "\nRequest sent: " << bytes_sent << " bytes\n";
}

std::string sck::SocketCL::RecvMsg()
{
    int numbytes;
    size_t total_received = 0;
    std::string message = "";

    do
    {
        numbytes = recv(sockfd, buf, BUFFER_SIZE-1, 0);
        buf[BUFFER_SIZE-1] = '\0';
        total_received += numbytes;
        message.append(std::string(buf));
        memset(buf, 0, BUFFER_SIZE);
    }
    while (numbytes > 0);


    if (numbytes == 0) {
        std::cout << "Server closed the connection!\n";
        return "ERROR!"; 
    } else if (numbytes == -1)
    {
        std::cout << "Error when receiving message!\n";
        std::cout << strerror(errno) << "\n";
        return "ERROR!";
    }

    std::cout << "Message received: " << total_received << " bytes\n";
    return std::string(buf);
}

void sck::SocketCL::Disconnect()
{
    int sht = shutdown(sockfd, 2);

    if (sht == -1)
    {
        std::cout << "Error when shutting down the socket!\n";
        throw "Error when shutting down the socket!\n";
    }
}

sck::SocketCL::~SocketCL()
{
    close(sockfd);
}