#include <iostream>

#include "ASPath.h"
#include "AS.h"

ASPath::ASPath(const IPV4Path& IPpath) {
    if (IPpath.size() == 0) return;

    AS prevAS = AS::fromIP(IPpath.at(0));
    path.push_back(prevAS);
    for (int i = 1; i < IPpath.size(); i++) {
        AS currAS = AS::fromIP(IPpath.at(i));
        if (currAS == prevAS) continue;

        path.push_back(currAS);

        prevAS = currAS;
    }
}

std::ostream& operator<<(std::ostream& ostream, const ASPath& asPath)  {
    for (int i = 0; i < asPath.path.size(); i++) {
        ostream << asPath.path.at(i) << " ";
        if (i < asPath.path.size() - 1) ostream << "---> ";
    }
    return ostream;
}