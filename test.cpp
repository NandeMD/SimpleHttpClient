#include <iostream>

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>

#include "httpclient.hpp"

#ifndef Client
#define Client http::HttpClient
#endif

int main()
{
    const std::string url = "http://google.com";
    Client client = Client(url);

    std::cout << "This is the raw request: \n";
    std::cout << client.request->raw;
    http::Response response = client.request->send();

    return 0;
}