#include <cpr/cpr.h>
#include <iostream>
#include <nlohmann/json.hpp>

#include "AS.h"
#include "IP.h"

std::map<IPV4, AS> AS::cache;

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
