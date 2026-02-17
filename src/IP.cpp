#include <netdb.h>
#include <netinet/in.h>
#include <sstream>
#include <string>
#include <iostream>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <tuple>
#include "IP.h"

std::string addrToString(const struct addrinfo *addrinfo) {
    socklen_t size = INET_ADDRSTRLEN;
    char* buffer = new char[size];

    inet_ntop(AF_INET, &(((struct sockaddr_in*) addrinfo->ai_addr)->sin_addr), buffer, size);

    std::string result(buffer);

    delete[] buffer;

    return result;
}

unsigned short chksum(unsigned short* packet, int len) {
    int i;
    unsigned int sum = 0;
    unsigned short *ptr;
    unsigned short cksm;

    for (i = len, ptr = packet; i > 1 ; i-=2) {
        sum += *ptr;
        ptr++;
    }

    if (i == 1) {
        sum += *((unsigned char*) ptr);
    }

    sum = (sum & 0xffff) + (sum >> 16);

    sum += (sum >> 16);

    cksm = ~sum;

    return cksm;
}

IPV4::IPV4(std::string ipString) {
    if (ipString == NO_IP) {
        isNoIP = true;
        return;
    }

    isNoIP = false;

    int byteStart = 0;

    int byteCount = 0;

    for (int i = 0; i < ipString.length(); i++) {
        if (ipString.at(i) == '.') {
            ipData[byteCount] = std::stoi(ipString.substr(byteStart, i - byteStart));
            byteStart = i + 1;
            byteCount++;
        }
    }

    ipData[byteCount] = std::stoi(ipString.substr(byteStart, ipString.length() - byteStart));
    byteCount++;

    if (byteCount != 4) {
        std::cout << "Constructed IP with byte count of " << byteCount << std::endl;
        std::cout << "Error!" << std::endl;
    }
}

bool IPV4::operator==(const IPV4& other) const {
    if (this->isNoIP && other.isNoIP) return true;

    return (!(*this<other)) && (!(other<*this));
}

IPV4::~IPV4() {

}

bool IPV4::operator<(const IPV4& other) const {
    return std::tie(ipData[0], ipData[1], ipData[2], ipData[3]) < std::tie(other.ipData[0], other.ipData[1], other.ipData[2], other.ipData[3]);
}

bool IPV4::isNotFound() const {
    return isNoIP;
}

std::string IPV4::toString() const {
    if (isNoIP) {
        return "-.-.-.-";
    }

    std::ostringstream out;

    out << (int) ipData[0];
    out << '.';
    out << (int) ipData[1];
    out << '.';
    out << (int) ipData[2];
    out << '.';
    out << (int) ipData[3];

    return out.str();
}

