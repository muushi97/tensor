#ifndef IG_SHAPE_HPP
#define IG_SHAPE_HPP

#include <iostream>
#include <vector>
#include <cmath>
#include <array>
#include <memory>
#include <random>

template <class T>
class tensor;

class shape {
    std::vector<std::size_t> dim_;
    std::size_t size_;

    void recalc() {
        size_ = 1;
        if (dim_.size() > 0)
            for (int i = 0; i < dim_.size(); i++)
                size_ *= dim_[i];
    }

public:
    shape() : shape({}) { }
    shape(std::initializer_list<std::size_t> il) : dim_{il} {
        recalc();
    }

    void reshape_rank(std::size_t r) {
        std::vector<std::size_t> d(r);
        if (rank() < r) {
            for (int i = 0; i < rank(); i++)
                d[i] = dim_[i];
            for (int i = rank(); i < r; i++)
                d[i] = 1;
        }
        else if (rank() > r) {
            for (int i = 0; i < r; i++)
                d[i] = dim_[i];
        }
        dim_ = std::move(d);
        recalc();
    }
    void set_dim(std::size_t i, std::size_t d) {
        dim_[i] = d;
        recalc();
    }

    std::size_t dim(std::size_t i) const { return dim_[i]; }
    std::size_t rank() const { return dim_.size(); }
private:
    std::size_t size() const { return size_; }
public:

    friend shape merge_shape(const shape &s1, const shape &s2);
    friend shape merge_shape(std::size_t i1, const shape &s1);
    friend shape merge_shape(const shape &s1, std::size_t i1);

    template <class T> friend class tensor;
};

shape merge_shape(const shape &s1, const shape &s2) {
    shape s;
    s.dim_.resize(s1.rank() + s2.rank());
    s.size_ = s1.size() * s2.size();
    for (int i = 0; i < s1.rank(); i++)
        s.dim_[i            ] = s1.dim(i);
    for (int i = 0; i < s2.rank(); i++)
        s.dim_[i + s1.rank()] = s2.dim(i);
    return s;
}
shape merge_shape(std::size_t i1, const shape &s1) {
    shape s;
    s.dim_.resize(1 + s1.rank());
    s.size_ = i1 * s1.size();
    for (int i = 0; i < 1; i++)
        s.dim_[i            ] = i1;
    for (int i = 0; i < s1.rank(); i++)
        s.dim_[i + 1        ] = s1.dim(i);
    return s;
}
shape merge_shape(const shape &s1, std::size_t i1) {
    shape s;
    s.dim_.resize(s1.rank() + 1);
    s.size_ = s1.size() * i1;
    for (int i = 0; i < s1.rank(); i++)
        s.dim_[i            ] = s1.dim(i);
    for (int i = 0; i < 1; i++)
        s.dim_[i + s1.rank()] = i1;
    return s;
}

#endif

