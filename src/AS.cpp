#include <cpr/cpr.h>
#include <iostream>
#include <nlohmann/json.hpp>

#include "AS.h"
#include "IP.h"

std::map<IPV4, AS> AS::cache;

const std::string NOT_VISIBLE = "<Not-Visible>";

AS::AS(int number, std::string name) {
    this->number = number;
    this->name = name;
}

AS getAS(const IPV4& ip) {
    cpr::Response r = cpr::Get(cpr::Url("https://stat.ripe.net/data/prefix-overview/data.json?resource=" + ip.toString()));

    if (r.status_code != 200) {
        std::cout << "Error fetching AS; Status " << r.status_code << std::endl;
    }

    nlohmann::json result = nlohmann::json::parse(r.text);

    if (result["data"]["asns"].is_array() && !result["data"]["asns"].empty()) {
        return AS(result["data"]["asns"][0]["asn"], result["data"]["asns"][0]["holder"]);
    }
    else {
        return AS(-1, NOT_VISIBLE);
    }
}

AS AS::fromIP(const IPV4& ip) {
    if (ip.isNotFound()) {
        return AS(-1, NOT_VISIBLE);
    }

    if (AS::cache.count(ip) > 0) {
        return AS::cache.at(ip);
    }
    else {
        AS as = getAS(ip);

        AS::cache.insert({ip, as});

        return as;
    }

    return AS(0, "1");
}

int AS::getASN() const {
    return number;
}

std::string AS::getName() const {
    return name;
}

bool AS::operator==(const AS& other) const {
    return this->number == other.number;
}

std::ostream& operator<<(std::ostream& ostream, const AS& as) {
    ostream << as.name;
    ostream << " (AS ";
    ostream << as.number;
    ostream << ")";

    return ostream;
}
