#include <array>
#include <netdb.h>
#include <string>

#pragma once
class IPV4 {
    private:
        std::array<uint8_t, 4> ipData;

    public:
        IPV4(std::string ipString);

        ~IPV4();

        std::string toString() const;

        bool operator<(const IPV4& other) const;

};

