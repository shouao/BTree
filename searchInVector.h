#ifndef _searchInVector

#define _searchInVector
#include <vector>
#include <algorithm>
template <typename T>
 int searchInVector(const std::vector<T>& vec, const T& e) {
    auto it = upper_bound(vec.begin(), vec.end(), e);
    if (it == vec.begin()) {
        return -1;
    }
    --it;
    return it - vec.begin();
}
#endif
