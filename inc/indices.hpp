#ifndef IG_INDICES_HPP
#define IG_INDICES_HPP

#include <iostream>
#include <vector>
#include <cmath>
#include <array>
#include <memory>
#include <random>

#include "shape.hpp"

class indices {
    shape s;

    std::vector<std::size_t> ind;

    template <class I, class... Is>
    void ind_set(std::size_t r, I i, Is... is) {
        if (r < ind.size()) {
            ind[r] = i;
            ind_set(r+1, is...);
        }
    }

public:
    template <class... Is>
    indices(const shape &sh, Is... is) : s(sh), ind(sh.rank()) {
        if (sh.rank() == 0) ind.resize(1);
        else ind.resize(sh.rank());
        for (int i = 0; i < ind.size(); i++) ind[i] = 0;
        if constexpr (sizeof...(Is) != 0) ind_set(0, is...);
    }

    void next() {
        if (s.rank() == 0) {
            ind[0] = (ind[0] + 1) % 2;
        }
        else {
            int i = 0;
            while (true) {
                ind[i]++;
                if (i == s.rank()-1) {
                    if (ind[i] <= s.dim(i)) return;
                }
                else 
                    if (ind[i] < s.dim(i)) return;

                ind[i] = 0;
                i++;
                if (i >= s.rank()) return;
            }
        }
    }
    void prev() {
        if (s.rank() == 0) {
            ind[0] = (ind[0] + 1) % 2;
        }
        else {
        int i = 0;
            while (true) {
                if (ind[i] > 0) {
                    ind[i]--;
                    return;
                }

                if (i == s.rank()-1) {
                    ind[i] = s.dim(i);
                    for (i = 0; i < s.rank()-1; i++)
                        ind[i] = 0;
                    return;
                }
                else  {
                    ind[i] = s.dim(i) - 1;
                    i++;
                }
            }
        }
    }
    std::size_t &index() { return ind[0]; }
    std::size_t index() const { return ind[0]; }
    std::size_t &index(std::size_t i) { return ind[i]; }
    std::size_t index(std::size_t i) const { return ind[i]; }
    std::size_t getIndex() const {
        std::size_t i = ind[rank()-1];
        for (int r = 1; r < rank(); r++)
            i = ind[rank()-1-r] + dim(rank()-1-r) * i;
        return i;
    }

    std::size_t dim(std::size_t i) const { return s.dim(i); }
    std::size_t rank() const { return s.rank(); }
};

bool operator == (const indices &i1, const indices &i2) {
    if (i1.rank() != i2.rank()) return false;
    for (int i = 0; i < i1.rank(); i++)
        if (i1.index(i) != i2.index(i)) return false;
    return true;
}
bool operator != (const indices &i1, const indices &i2) {
    return i1 == i2 ? false : true;
}

#endif

