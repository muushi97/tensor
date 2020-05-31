#ifndef IG_TENSOR_HPP
#define IG_TENSOR_HPP

#include <iostream>
#include <vector>
#include <cmath>
#include <array>
#include <memory>
#include <random>

#include "shape.hpp"
#include "indices.hpp"

// tensor
template <class T>
class tensor {
    shape s;

    std::vector<T> v;

    // 添字変換
    template <class I, class... Is>
    std::size_t _getIndex(std::size_t r, I i, Is... is) const { return i + dim(r) * _getIndex<Is...>(r+1, is...); }
    template <class I, class... Is>
    std::size_t _getIndex(std::size_t r, I i) const { return i; }
    std::size_t _getIndex(std::size_t r) const { return 0; }
    template <class... Is>
    std::size_t getIndex(Is... is) const { return _getIndex(0, is...); }

public:
    tensor() : s() {
        v.resize(s.size());
    }
    tensor(std::vector<std::size_t> iv) : s() {
        s.reshape_rank(iv.size());
        for (int i = 0; i < iv.size(); i++)
            s.set_dim(i, iv[i]);
        v.resize(s.size());
    }
    tensor(std::initializer_list<std::size_t> il) : s(il) {
        v.resize(s.size());
    }
    // 添字生成
    indices begin() const { return indices(s); }
    indices end() const { indices i(s); i.prev(); return i; }
    // アクセス
    template <class... Is>
          T &operator () (Is... is)            { return v[getIndex<Is...>(is...)]; }
    template <class... Is>
    const T &operator () (Is... is)    const   { return v[getIndex<Is...>(is...)]; }
    template <class... Is>
          T  operator () (Is... is)    const&& { return v[getIndex<Is...>(is...)]; }
          T &at(const indices &ind)            { return v[ind.getIndex()]; }
    const T &at(const indices &ind)    const   { return v[ind.getIndex()]; }

    std::size_t dim(std::size_t i) const { return s.dim(i); }
    std::size_t rank() const { return s.rank(); }
};

#endif
