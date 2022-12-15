#include "httpclient.hpp"


http::Response::Response(std::string raw)
{
    this->raw = raw;

    size_t double_new_line = raw.find("\n\n");
    bool is_there_a_double_new = double_new_line != -1;
    this->content = (is_there_a_double_new) ? raw.substr(double_new_line+2, raw.size()) : "\n";
    std::string resp_and_headers = (is_there_a_double_new) ? raw.substr(0, double_new_line) : raw.substr(0, raw.size());

    size_t first_new_line = resp_and_headers.find("\n");
    std::string resp = resp_and_headers.substr(0, first_new_line);
    size_t first_space = resp.find(" ");
    this->http_ver = resp.substr(0, first_space);
    this->status = resp.substr(first_space+1, resp.size());

    std::string raw_headers = resp_and_headers.substr(first_new_line+1, resp_and_headers.size());
    first_new_line = raw_headers.find("\n");

    do
    {
        std::string header = raw_headers.substr(0, first_new_line);
        size_t colon = header.find(":");
        this->headers.insert({header.substr(0, colon), header.substr(colon+1, first_new_line)});
        raw_headers = raw_headers.substr(first_new_line+1, raw_headers.size());
        first_new_line = raw_headers.find("\n");
    } while (first_new_line != -1);
    
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
