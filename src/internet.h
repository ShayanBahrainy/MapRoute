#include <netdb.h>
#include <string>
#include <map>

class IPV4 {
    private:
        unsigned char* ipData;

    public:
        IPV4(std::string ipString);

        ~IPV4();

        std::string toString();

        bool operator<(const IPV4& other) const;

};

class ASN {
    static std::map<IPV4, ASN> cache;

    int number;
    std::string name;

    ASN(int number, std::string name);

    public:
        static ASN fromIP(const IPV4& ip);
};