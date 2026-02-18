#include <iostream>

#include "ASPath.h"
#include "AS.h"

ASPath::ASPath() {

}

void ASPath::push(const AS& as) {
    if (path.size() > 0 && path.back() == as) return;

    path.push_back(as);
}

ASPath* ASPath::fromIpPath(const IPV4Path& IPpath) {
    ASPath* newPath = new ASPath;

    for (int i = 0; i < IPpath.size(); i++) {
        AS currAS = AS::fromIP(IPpath.at(i));

        newPath->push(currAS);
    }

    return newPath;
}

std::ostream& operator<<(std::ostream& ostream, const ASPath& asPath)  {
    for (int i = 0; i < asPath.path.size(); i++) {
        ostream << asPath.path.at(i) << " ";
        if (i < asPath.path.size() - 1) ostream << "---> ";
    }
    return ostream;
}