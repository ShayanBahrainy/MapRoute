#include <map>
#include <string>

#include "IP.h"

#pragma once
class AS {
    static std::map<IPV4, AS> cache;

    int number;
    std::string name;

    public:
        AS(int number, std::string name);

        static AS fromIP(const IPV4& ip);
        std::string toString() const;
};