#include <string>
#include <vector>
#include "internet.h"

class IPV4Path {
    private:
        std::vector<IPV4> path;

    public:
        IPV4Path(std::vector<IPV4> path);
        IPV4Path();

        void add(IPV4 ip);
        int size();

        std::string toString() const;
};

class ASPath {
    ASPath(const IPV4Path& ipPath) {

    }
};