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

// Client generates all the minimal request data and headers automatically.
```