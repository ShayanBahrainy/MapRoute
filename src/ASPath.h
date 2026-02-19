#include "IPV4Path.h"
#include "AS.h"
#include <ostream>

class ASPath {
    private:
        std::vector<AS> path;

    public:
        int size() const;
        AS& at(int i);

        ASPath();
        void push(const AS& as);

        static ASPath* fromIpPath(const IPV4Path& IPpath);

        friend std::ostream& operator<<(std::ostream& ostream, const ASPath& asPath);
};