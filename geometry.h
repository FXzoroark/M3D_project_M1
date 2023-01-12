//
// Created by fxzoroark on 10/01/23.
//

#ifndef PROJECT_GEOMETRY_H
#define PROJECT_GEOMETRY_H
#pragma once

#include <cmath>
#include <functional>
#include <string>
#include <cassert>

template<typename T>
struct Vec2 {
    T x;
    T y;
    T & operator[](const int i)       { assert(i>=0 && i<2); return i ? y : x; }
    T   operator[](const int i) const { assert(i>=0 && i<2); return i ? y : x; }

    Vec2<T> add(T x, T y) {
        return Vec2<T>{ this->x + x, this->y + y };
    }


    Vec2<T> scl(T x, T y) {
        return Vec2<T>{ this->x * x, this->y * y };
    }

    Vec2<T> normalize() {
        float n = norm();
        return Vec2<T>{ x/n, y/n };
    }

    T operator*(Vec2<T> o) {
        return x*o.x + y*o.y;
    }

    Vec2<T> operator+(Vec2<T> o) {
        return Vec2<T>{ x + o.x, y + o.y };
    }

    Vec2<T> operator-(Vec2<T> o) {
        return Vec2<T>{ x-o.x, y-o.y};
    }


    T norm() {
        return std::sqrt(x*x + y*y);
    }

    operator std::string() {
        return "Vec2<...>(" + std::to_string(this->x) + ", " + std::to_string(this->y) + ")";
    }
};

template<typename T>
struct Vec3 {
    T x;
    T y;
    T z;

    T& operator[](const int i)       { assert(i>=0 && i<3); return i ? (1==i ? y : z) : x; }
    T  operator[](const int i) const { assert(i>=0 && i<3); return i ? (1==i ? y : z) : x; }

    Vec3<T> scl(T x, T y, T z) {
        return Vec3<T>{ this->x * x, this->y * y, this->z * z };
    }

    Vec3<T> normalize() {
        float n = norm();
        return Vec3<T>{ x/n, y/n, z/n };
    }

    T norm() {
        return std::sqrt(x*x + y*y + z*z);
    }

    T operator*(Vec3<T> o) {
        return x*o.x + y*o.y + z*o.z;
    }

    Vec3<T> operator+(Vec3<T> o) {
        return Vec3<T>{ x + o.x, y + o.y, z + o.z };
    }

    Vec3<T> operator-(Vec3<T> o) {
        return Vec3<T>{ x-o.x, y-o.y, z-o.z };
    }

    Vec3<T> operator^(Vec3<T> o) {
        T s1 = y*o.z - o.y*z;
        T s2 = z*o.x - o.z*x;
        T s3 = x*o.y - o.x*y;
        return Vec3<T>{ s1, s2, s3 };
    }

    operator std::string() {
        return "Vec3<...>(" + std::to_string(this->x) + ", " + std::to_string(this->y) + ", " + std::to_string(this->z) + ")";
    }
    operator Vec2<T>() { return Vec2<T>{ x, y }; }
};

#endif //PROJECT_GEOMETRY_H
