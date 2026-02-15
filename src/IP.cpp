#include <netdb.h>
#include <sstream>
#include <string>
#include <iostream>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <tuple>
#include "IP.h"

IPV4::IPV4(std::string ipString) {
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

IPV4::~IPV4() {

}

bool IPV4::operator<(const IPV4& other) const {
    return std::tie(ipData[0], ipData[1], ipData[2], ipData[3]) < std::tie(other.ipData[0], other.ipData[1], other.ipData[2], other.ipData[3]);
}

std::string IPV4::toString() const {
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

