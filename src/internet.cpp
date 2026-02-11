#include <netdb.h>
#include <sstream>
#include <string>
#include <map>
#include <iostream>
#include <cpr/cpr.h>
#include <nlohmann/json.hpp>
#include <tuple>
#include "internet.h"
#include "nlohmann/json_fwd.hpp"

std::map<IPV4, AS> AS::cache;

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

bool IPV4::operator<(const IPV4& other) const {
    return std::tie(ipData[0], ipData[1], ipData[2], ipData[3]) < std::tie(other.ipData[0], other.ipData[1], other.ipData[2], other.ipData[3]);
}

AS::AS(int number, std::string name) {
    this->number = number;
    this->name = name;
}

AS getAS(const IPV4& ip) {
    cpr::Response r = cpr::Get(cpr::Url("https://stat.ripe.net/data/prefix-overview/data.json?resource=" + ip.toString()));

    if (r.status_code != 200) {
        std::cout << "error fetching asn; status" << r.status_code << std::endl;
    }

    nlohmann::json result = nlohmann::json::parse(r.text);

    if (result["data"]["asns"].is_array() && !result["data"]["asns"].empty()) {
        return AS(result["data"]["asns"][0]["asn"], result["data"]["asns"][0]["holder"]);
    }
    else {
        return AS(-1, "<Not-Announced>");
    }
}

AS AS::fromIP(const IPV4& ip) {
    if (AS::cache.count(ip) > 0) {
        std::cout << "Retrieved from cache for ip " << ip.toString() << std::endl;

        return AS::cache.at(ip);
    }
    else {
        std::cout << "Performing lookup for ip " << ip.toString() << std::endl;

        AS as = getAS(ip);

        AS::cache.insert({ip, as});

        return as;
    }

    return AS(0, "1");
}

std::string AS::toString() const {
    std::ostringstream oss;
    oss << name;
    oss << " (AS ";
    oss << number;
    oss << ")";

    return oss.str();
}
