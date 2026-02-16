#include <array>
#include <netdb.h>
#include <string>

const std::string NO_IP = "<no-ip>";

unsigned short chksum(unsigned short* packet, int len);

#pragma once
class IPV4 {
    private:
        std::array<uint8_t, 4> ipData;

        bool isNoIP;

    public:
        IPV4(std::string ipString);

        ~IPV4();

        std::string toString() const;

        bool operator<(const IPV4& other) const;

        bool operator==(const IPV4& other) const;

};

