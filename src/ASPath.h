#include "IPV4Path.h"
#include "AS.h"
#include <ostream>

class ASPath {
    private:
        std::vector<AS> path;

    public:
        ASPath(const IPV4Path& ipPath);

        friend std::ostream& operator<<(std::ostream& ostream, const ASPath& asPath);
};