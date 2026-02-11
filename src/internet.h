#include <array>
#include <netdb.h>
#include <string>
#include <map>

class IPV4 {
    private:
        std::array<uint8_t, 4> ipData;

    public:
        IPV4(std::string ipString);

        ~IPV4();

        std::string toString() const;

        bool operator<(const IPV4& other) const;

};

class AS {
    static std::map<IPV4, AS> cache;

    int number;
    std::string name;

    public:
        AS(int number, std::string name);
        static AS fromIP(const IPV4& ip);
        std::string toString() const;
};