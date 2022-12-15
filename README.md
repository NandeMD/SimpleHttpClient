# SimpleHttpClient

This is a really really simple HTTP client library in C++.

I wrote this code to learn c++ fundamentals so probably this will not work very well. 

This library can only send GET requests for now. Maybe I will complete it when I have a lot of free time, or maybe I won't.

### Example usage:
```c++
#ifndef Client
#define Client http::HttpClient
#endif

const std::string url = "http://google.com";
Client client = Client(url); // Create the client object

std::cout << "This is the raw request: \n";
std::cout << client.request->raw;
http::Response response = client.request->send(); // send the GET request

std::cout << response.raw      << "\n"; // print the raw response
std::cout << response.http_ver << "\n"; // print the http protocol version
std::cout << response.status   << "\n"; // print the status code and status message if there is one
std::cout << response.headers  << "\n"; // get response headers as a map<std::string, std::string>
std::cout << response.content  << "\n"; // print the response body as string

// Client generates all the minimal request data and headers automatically.
```