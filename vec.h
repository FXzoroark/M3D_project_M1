#pragma once

#include <cmath>
#include <cstdarg>
#include <ostream>
#include <string>
#include <iostream>
#include <assert.h>


template<int DIM, typename T, typename... Args>
struct Vec {
    Vec(){
        for (int i = 0; i < DIM; ++i) {
            data[i] = (T)0;
        }
    }
    Vec(...) {
        std::va_list list;
        va_start(list, nullptr);
        for (int i = 0; i < DIM; i++) {
            data[i] = va_arg(list, T);
        }
        va_end(list);
    }

    Vec(double* v) {
        for (int i = 0; i < DIM; i++) {
            data[i] = v[i];
            // std::cout << v[i] << "\n";
        }
    }

    std::string toString() {
        std::string s{};
        s
         .append("Dim: ")
         .append(std::to_string(DIM))
         .append("; ");
        for (auto e : data) {
            s.append(std::to_string(e));
            s.append(", ");
        }

        return s;
    }
    
    Vec scl(T f) {
        Vec r;
        for (int i = 0; i < DIM; i++) r[i] = data[i]*f;
        return r;
    }

    Vec operator-(Vec rhs) {
        Vec r;
        for (int i = 0; i < DIM; i++) r[i] = data[i]*rhs[i];
        return r;
    }

    Vec normalize() {
        T norm = this->norm();
        T n_data[DIM];
        for (int i = 0; i < DIM; i++) {
            n_data[i] = data[i]/norm;
        }
        return Vec<DIM, T>{n_data};

    }
    
    T norm() {
        T sum = 0;
        for (int i = 0; i < DIM; i++) sum += data[i]*data[i];
        return sqrt(sum);
    }

    T& operator[] (int idx) {
        assert(idx >= 0 && idx < DIM);
        return data[idx];
    }

    Vec operator+(Vec<DIM, T> rhs) {
        T n_data[DIM];
        for (int i = 0; i < DIM; i++) {
            n_data[i] = data[i] + rhs[i];
        }
        return Vec<DIM, T>{n_data};
    }
    
    T operator*(Vec<DIM, T> rhs) {
        T r = 0;
        for (int i = 0; i < DIM; i++) {
            r += data[i] * rhs[i];
        }
        return r;
    }



    T data[DIM];
};

template<typename T> struct Vec<2, T> {
    Vec() {
        data[0] = (T)0;
        data[1] = (T)0;
    };

    Vec(T x, T y) {
        data[0] = x;
        data[1] = y;
    }

    Vec operator+(Vec rhs) {
        return Vec{x() + rhs.x(), y() + rhs.y()};
    }

    Vec operator-(Vec rhs) {
        return Vec{x() - rhs.x(), y() - rhs.y()};
    }

    T& operator[](int idx) { return data[idx]; };

    Vec operator*(Vec<3, T> rhs) {
        Vec<3, T> v = Vec<3, T>{ x(), y(), 1 } * rhs;
        return Vec{v.x(), v.y()};
    }

    T norm() {
        T sum = 0;
        for (int i = 0; i < 3; i++) sum += data[i]*data[i];
        return sqrt(sum);
    }

    T x() { return data[0]; }
    T y() { return data[1]; }

    void x(T v) { data[0] = v; }
    void y(T v) { data[1] = v; }
private:
    T data[2];
};

template<typename T> struct Vec<3, T> {
    Vec() {
        data[0] = (T)0;
        data[1] = (T)0;
        data[2] = (T)0;
    };

    Vec(T x, T y, T z) {
        data[0] = x;
        data[1] = y;
        data[2] = z;
    }

    Vec(Vec<4, T> o) {
        x(o.x());
        y(o.y());
        z(o.z());
    }

    Vec normalize() {
        T norm = sqrt(x()*x()+y()*y()+z()*z());
        return Vec{ x()/norm, y()/norm, z()/norm };
    }



    Vec scl(T X, T Y, T Z) {
        return Vec{x()*X, y()*Y, z()*Z};
    }

    Vec scl(T f) {
        return Vec{x()*f, y()*f, z()*f};
    }


    Vec operator+(Vec rhs) {
        return Vec{x() + rhs.x(), y() + rhs.y(), z() + rhs.z()};
    }

    void operator+=(Vec rhs) {
        x(x() + rhs.x());
        y(y() + rhs.y());
        z(z() + rhs.z());
    }

    Vec operator-(Vec rhs) {
        return Vec{x() - rhs.x(), y() - rhs.y(), z() - rhs.z()};
    }

    Vec operator/(T rhs) {
        return Vec{x() / rhs, y() / rhs, z() / rhs};
    }

    Vec operator^(Vec rhs) {
        T s1 = y()*rhs.z() - rhs.y()*z();
        T s2 = z()*rhs.x() - rhs.z()*x();
        T s3 = x()*rhs.y() - rhs.x()*y();
        return Vec{ s1, s2, s3 };
    }

    T operator*(Vec rhs) {
        return x()*rhs.x() + y()*rhs.y() + z()*rhs.z();
    }

    operator Vec<2, T>() {
        return Vec<2, T>{x(), y()};
    }

    T& operator[] (int idx) {
        assert(idx >= 0 && idx < 3);
        return data[idx];
    }

    template<int D> Vec<D, double> proj(){
        assert(D > 0);
        Vec<D, double> res;
        for (int i = 0; i < D; ++i) {
            res[i] = i>=3 ? 0 : data[i];
        }
        return res;
    }

    T norm() {
        T sum = 0;
        for (int i = 0; i < 3; i++) sum += data[i]*data[i];
        return sqrt(sum);
    }

    std::string toString(){
        std::string s = "Vec3: " + std::to_string(x()) + " " + std::to_string(y()) + " " + std::to_string(z()) + "\n";
        return s;
    }

    T x() { return data[0]; }
    T y() { return data[1]; }
    T z() { return data[2]; }

    void x(T v) { data[0] = v; }
    void y(T v) { data[1] = v; }
    void z(T v) { data[2] = v; }
private:
    T data[3];
};


template<typename T> struct Vec<4, T> {
    Vec() {
        data[0] = (T)0;
        data[1] = (T)0;
        data[2] = (T)0;
        data[3] = (T)0;

    };

    Vec(T x, T y, T z, T zz) {
        data[0] = x;
        data[1] = y;
        data[2] = z;
        data[3] = zz;
    }

    Vec normalize() {
        T norm = sqrt(x()*x()+y()*y()+z()*z()+zz()*zz());
        return Vec{ x()/norm, y()/norm, z()/norm , zz()/norm};
    }



    Vec scl(T X, T Y, T Z, T ZZ) {
        return Vec{x()*X, y()*Y, z()*Z, zz()*ZZ};
    }

    Vec scl(T f) {
        return Vec{x()*f, y()*f, z()*f, zz()*f};
    }


    Vec operator/(T rhs) {
        return Vec{x() / rhs, y() / rhs, z() / rhs, zz() / rhs};
    }


    T& operator[] (int idx) {
        assert(idx >= 0 && idx < 4);
        return data[idx];
    }

    template<int D> Vec<D, double> proj(){
        assert(D > 0);
        Vec<D, double> res;
        for (int i = 0; i < D; ++i) {
            res[i] = i>=3 ? 0 : data[i];
        }
        return res;
    }

    T norm() {
        T sum = 0;
        for (int i = 0; i < 4; i++) sum += data[i]*data[i];
        return sqrt(sum);
    }

    std::string toString(){
        std::string s = "Vec3: " + std::to_string(x()) + " " + std::to_string(y()) + " " + std::to_string(z()) + "\n";
        return s;
    }

    T x() { return data[0]; }
    T y() { return data[1]; }
    T z() { return data[2]; }
    T zz() {return data[3]; }

    void x(T v) { data[0] = v; }
    void y(T v) { data[1] = v; }
    void z(T v) { data[2] = v; }
    void zz(T v) {data[3] = v; }
private:
    T data[4];
};




