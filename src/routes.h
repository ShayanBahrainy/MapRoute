#include <string>
#include <vector>
#include "IP.h"

class IPV4Path {
    private:
        std::vector<IPV4> path;

        static IPV4Path* fromAddress(struct addrinfo* addrinfo);

    public:
        IPV4Path(std::vector<IPV4> path);
        IPV4Path();

        void add(IPV4 ip);
        int size() const;

        static IPV4Path* fromDomain(std::string domain);
        static IPV4Path* fromIP(IPV4 target);

        std::string toString() const;
};

class ASPath {
    public:
        ASPath(const IPV4Path& ipPath);
};