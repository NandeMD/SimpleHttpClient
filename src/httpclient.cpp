#include "httpclient.hpp"


http::Response::Response(std::string raw)
{
    this->raw = raw;
}


void http::Request::parse_url_route(const std::string url)
{
    const size_t first_dot = url.find(".");
    const size_t is_slash_found = url.find("/", first_dot);
    const size_t route_slash = (is_slash_found != -1) ? is_slash_found : url.size();

    std::string url_part = url.substr(0, route_slash);
    std::string route_part = url.substr(route_slash, url.size());

    std::cout << url_part << "\n";
    std::cout << route_part << "\n";

    this->route = (is_slash_found != -1) ? route_part : "/";
    this->url = url_part;
}

http::Request::Request(const std::string url)
{
    parse_url_route(url);
    this->s_client = new sck::SocketCL(this->url);
    this->headers.insert({"Host", this->s_client->ADDR_STR});

    this->raw.append(this->method);
    this->raw.append(" ");
    this->raw.append(this->route);
    this->raw.append(" ");
    this->raw.append(this->http_ver);
    this->raw.append("\n");

    std::map<std::string, std::string>::iterator it;
    for (it = this->headers.begin(); it != this->headers.end(); ++it)
    {
        this->raw.append(it->first);
        this->raw.append(": ");
        this->raw.append(it->second);
        this->raw.append("\n");
    }
}

http::Response http::Request::send()
{
    this->s_client->Connect();
    this->s_client->SendMsg(this->raw);
    http::Response response = http::Response(this->s_client->RecvMsg());
    this->s_client->Disconnect();


    return response;
}


http::HttpClient::HttpClient(const std::string url)
{
    this->url = url;
    this->request = new Request(url);
}
