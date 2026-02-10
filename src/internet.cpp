#include <netdb.h>
#include <sstream>
#include <string>
#include <map>
#include <iostream>
#include <cpr/cpr.h>
#include <tuple>
#include "internet.h"

std::map<IPV4, ASN> ASN::cache;

IPV4::IPV4(std::string ipString) {
    ipData = new unsigned char[4];

    int byteStart = 0;

    int byteCount = 0;

    for (int i = 0; i < ipString.length(); i++) {
        if (ipString.at(i) == '.') {
            *(ipData + byteCount) = std::stoi(ipString.substr(byteStart, i - byteStart));
            byteStart = i + 1;
            byteCount++;
        }
    }

    *(ipData + byteCount) = std::stoi(ipString.substr(byteStart, ipString.length() - byteStart));
    byteCount++;

    if (byteCount != 4) {
        std::cout << "Constructed IP with byte count of " << byteCount << std::endl;
        std::cout << "Error!" << std::endl;
    }
}

IPV4::~IPV4() {
    delete ipData;
    ipData = nullptr;
}

std::string IPV4::toString() {
    std::ostringstream out;

    out << (int) *ipData;
    out << '.';
    out << (int) *(ipData + 1);
    out << '.';
    out << (int) *(ipData + 2);
    out << '.';
    out << (int) *(ipData + 3);

    return out.str();
}

bool IPV4::operator<(const IPV4& other) const {
    return std::tie(*(this->ipData), *(this->ipData + 1), *(this->ipData + 2), *(this->ipData + 3)) < std::tie(*(this->ipData), *(other.ipData + 1), *(other.ipData + 2), *(other.ipData + 3));
}

ASN::ASN(int number, std::string name) {
    this->number = number;
    this->name = name;
}

ASN ASN::fromIP(const IPV4& ip) {
    if (ASN::cache.count(ip) > 0) {
        return ASN::cache.at(ip);
    }
    else {
        //ASN()
    }

    return ASN(0, "1");
}
