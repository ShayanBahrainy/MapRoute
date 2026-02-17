#include <map>
#include <ostream>
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

        int getASN() const;
        std::string getName() const;

        friend std::ostream& operator<<(std::ostream& stream, const AS& as);

        bool operator==(const AS& other) const;

};