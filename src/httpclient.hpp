#include "Socket.hpp"
#include <vector>
#include <map>


#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H


namespace http
{
    class Response
    {
        public:
            std::string                         status;
            std::string                         raw;
            std::string                         content;
            std::string                         http_ver;
            std::map<std::string, std::string>  headers;

            Response(std::string raw);
    };

    class Request
    {
        private:
            sck::SocketCL *s_client;
            void parse_url_route(std::string url);

        public:
            std::string                         url;
            std::string                         raw;
            std::string                         body;
            std::string                         route;
            const std::string                   method = "GET";
            const std::string                   http_ver = "HTTP/1.1";
            std::map<std::string, std::string>  headers;

            Request(const std::string url);
            Response send();    
    };


    class HttpClient 
    {
        public:
            std::string url;
            Request *request;

            HttpClient(const std::string url);
            
            Response Get();
    };
};


#endif