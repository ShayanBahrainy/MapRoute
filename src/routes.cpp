#include "routes.h"
#include "IP.h"
#include <cstddef>
#include <vector>
#include <arpa/inet.h>


IPV4Path::IPV4Path() {
    path = std::vector<IPV4>();
}

void IPV4Path::add(IPV4 ip) {
    path.push_back(ip);
}

int IPV4Path::size() const {
    return path.size();
}

IPV4Path* IPV4Path::fromDomain(std::string domain) {
    struct addrinfo* addrinfo;

    if (getaddrinfo(domain.c_str(), NULL, NULL, &addrinfo) < 0) {
        perror("address resolve ");
        exit(1);
        return nullptr;
    }

    socklen_t size = INET_ADDRSTRLEN;
    char* buffer = new char[size];

    void *data;

    data = (sockaddr_in*) addrinfo->ai_addr;

    inet_ntop(AF_INET, data, buffer, size);

}


IPV4Path* IPV4Path::fromIP(IPV4 target) {

}