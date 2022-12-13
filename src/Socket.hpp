#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <unistd.h>


#ifndef SOCKETCL_H
#define SOCKETCL_H
#define BUFFER_SIZE 1024


namespace sck {
    class SocketCL
    {
        private:
            struct  addrinfo hints, *res, *p;
            int              status;

            int              sockfd;
            char             buf[BUFFER_SIZE];

            const std::string PROTO_SEP = "://";
            const size_t PROTO_SEP_LEN = PROTO_SEP.size();

            void parse_url();
            void prep_hints();
            void get_address_in_from_url();
            void create_socket();

        public:
            std::string      URL;
            std::string      PROTO;
            std::string      ADDR_STR;

            struct sockaddr      ipv4_addr;
            struct sockaddr      ipv6_addr;

            struct sockaddr_in  *ipv4;
            struct sockaddr_in6 *ipv6;

            bool is_connected = false;

            SocketCL(const std::string URL);
            ~SocketCL();

            void Connect();
            void SendMsg(std::string msg);
            std::string RecvMsg();
            void Disconnect();
    };
};

#endif