#pragma once

#include <cassert>
#include <cmath>
#include <iostream>
#include <vector>
#include "vec.h"

template <int ROW, int COL>
struct Matrix {

    double data[ROW][COL];

    static Matrix<ROW, COL> identity() {
        Matrix<ROW, COL> m;
        for (int i = 0; i < ROW; i++) {
            for (int j = 0; j < COL; j++) {
                if (i == j) {
                    m[i][j] = 1;
                } else {
                    m[i][j] = 0;
                }
            }
        }
        return m;
    }

    Matrix() {
        for (int i = 0; i < ROW; ++i) {
            for (int j = 0; j < COL; ++j) {
                data[i][j] = 0;
            }
        }
    }

    template<int ROW2>
    Matrix(Vec<ROW2, double> vec){
        for (int i = 0; i < ROW; ++i) {
            if(i >= ROW2){
                data[i][0] = 1;
            }
            else{
                data[i][0] = vec[i];
            }
        }
    }

    static Matrix<4, 4> invert44(Matrix<4, 4> m) {
        Matrix<4, 4> r;
        m.InverseMat44(r);

        return r;
    }

    Matrix transpose() {
        Matrix<ROW, COL> r;
        for (int i = 0; i < ROW; i++) {
            for (int j = 0; j < COL; j++) {
                r.data[i][j] = data[j][i];
            }
        }

        return r;
    }

    double* operator[](int row) {
        return data[row];
    }

    void set_col(int col, Vec<ROW, double> vec){
        assert(col < COL);
        for (int i = 0; i < ROW; i++){
            data[i][col] = vec[i];
        }
    }

    std::string string() {
        std::string s = "";
        for (int i = 0; i < ROW; i++) {
            for (int j = 0; j < COL; j++) {
                s += std::to_string(data[i][j]) + ",\t";
            }
            s += "\n";
        }

        return s;
    }

    template<int ROW2, int COL2> Matrix<ROW, COL2> operator*(Matrix<ROW2, COL2> rhs) {
        assert(COL == ROW2);
        Matrix<ROW, COL2> m;
        for (int i = 0; i < ROW; i++) {
            for (int j = 0; j < COL2; j++) {
                double sum = 0;
                for (int k = 0; k < COL; k++) {
                    sum += data[i][k] * rhs[k][j];
                }
                m[i][j] = sum;
            }
        }

        return m;
    }
    Vec<ROW, double> operator*(Vec<COL, double> vec){
        Vec<ROW, double> ret;
        for (int i = 0; i < ROW; ++i) {
            for (int j = 0; j < COL; ++j) {
                ret[i] += data[i][j] * vec[j];
            }
        }
        return ret;
    }

private:
    bool InverseMat44(Matrix<4, 4> &inverted)
    {
        float inv[16], det;
        int i;

        inv[0]  =  getLinear(5) * getLinear(10) * getLinear(15) - getLinear(5) * getLinear(11) * getLinear(14) - getLinear(9) * getLinear(6) * getLinear(15) + getLinear(9) * getLinear(7) * getLinear(14) + getLinear(13) * getLinear(6) * getLinear(11) - getLinear(13) * getLinear(7) * getLinear(10);
        inv[4]  = -getLinear(4) * getLinear(10) * getLinear(15) + getLinear(4) * getLinear(11) * getLinear(14) + getLinear(8) * getLinear(6) * getLinear(15) - getLinear(8) * getLinear(7) * getLinear(14) - getLinear(12) * getLinear(6) * getLinear(11) + getLinear(12) * getLinear(7) * getLinear(10);
        inv[8]  =  getLinear(4) * getLinear(9)  * getLinear(15) - getLinear(4) * getLinear(11) * getLinear(13) - getLinear(8) * getLinear(5) * getLinear(15) + getLinear(8) * getLinear(7) * getLinear(13) + getLinear(12) * getLinear(5) * getLinear(11) - getLinear(12) * getLinear(7) * getLinear(9);
        inv[12] = -getLinear(4) * getLinear(9)  * getLinear(14) + getLinear(4) * getLinear(10) * getLinear(13) + getLinear(8) * getLinear(5) * getLinear(14) - getLinear(8) * getLinear(6) * getLinear(13) - getLinear(12) * getLinear(5) * getLinear(10) + getLinear(12) * getLinear(6) * getLinear(9);
        inv[1]  = -getLinear(1) * getLinear(10) * getLinear(15) + getLinear(1) * getLinear(11) * getLinear(14) + getLinear(9) * getLinear(2) * getLinear(15) - getLinear(9) * getLinear(3) * getLinear(14) - getLinear(13) * getLinear(2) * getLinear(11) + getLinear(13) * getLinear(3) * getLinear(10);
        inv[5]  =  getLinear(0) * getLinear(10) * getLinear(15) - getLinear(0) * getLinear(11) * getLinear(14) - getLinear(8) * getLinear(2) * getLinear(15) + getLinear(8) * getLinear(3) * getLinear(14) + getLinear(12) * getLinear(2) * getLinear(11) - getLinear(12) * getLinear(3) * getLinear(10);
        inv[9]  = -getLinear(0) * getLinear(9)  * getLinear(15) + getLinear(0) * getLinear(11) * getLinear(13) + getLinear(8) * getLinear(1) * getLinear(15) - getLinear(8) * getLinear(3) * getLinear(13) - getLinear(12) * getLinear(1) * getLinear(11) + getLinear(12) * getLinear(3) * getLinear(9);
        inv[13] =  getLinear(0) * getLinear(9)  * getLinear(14) - getLinear(0) * getLinear(10) * getLinear(13) - getLinear(8) * getLinear(1) * getLinear(14) + getLinear(8) * getLinear(2) * getLinear(13) + getLinear(12) * getLinear(1) * getLinear(10) - getLinear(12) * getLinear(2) * getLinear(9);
        inv[2]  =  getLinear(1) * getLinear(6)  * getLinear(15) - getLinear(1) * getLinear(7)  * getLinear(14) - getLinear(5) * getLinear(2) * getLinear(15) + getLinear(5) * getLinear(3) * getLinear(14) + getLinear(13) * getLinear(2) * getLinear(7)  - getLinear(13) * getLinear(3) * getLinear(6);
        inv[6]  = -getLinear(0) * getLinear(6)  * getLinear(15) + getLinear(0) * getLinear(7)  * getLinear(14) + getLinear(4) * getLinear(2) * getLinear(15) - getLinear(4) * getLinear(3) * getLinear(14) - getLinear(12) * getLinear(2) * getLinear(7)  + getLinear(12) * getLinear(3) * getLinear(6);
        inv[10] =  getLinear(0) * getLinear(5)  * getLinear(15) - getLinear(0) * getLinear(7)  * getLinear(13) - getLinear(4) * getLinear(1) * getLinear(15) + getLinear(4) * getLinear(3) * getLinear(13) + getLinear(12) * getLinear(1) * getLinear(7)  - getLinear(12) * getLinear(3) * getLinear(5);
        inv[14] = -getLinear(0) * getLinear(5)  * getLinear(14) + getLinear(0) * getLinear(6)  * getLinear(13) + getLinear(4) * getLinear(1) * getLinear(14) - getLinear(4) * getLinear(2) * getLinear(13) - getLinear(12) * getLinear(1) * getLinear(6)  + getLinear(12) * getLinear(2) * getLinear(5);
        inv[3]  = -getLinear(1) * getLinear(6)  * getLinear(11) + getLinear(1) * getLinear(7)  * getLinear(10) + getLinear(5) * getLinear(2) * getLinear(11) - getLinear(5) * getLinear(3) * getLinear(10) - getLinear(9)  * getLinear(2) * getLinear(7)  + getLinear(9)  * getLinear(3) * getLinear(6);
        inv[7]  =  getLinear(0) * getLinear(6)  * getLinear(11) - getLinear(0) * getLinear(7)  * getLinear(10) - getLinear(4) * getLinear(2) * getLinear(11) + getLinear(4) * getLinear(3) * getLinear(10) + getLinear(8)  * getLinear(2) * getLinear(7)  - getLinear(8)  * getLinear(3) * getLinear(6);
        inv[11] = -getLinear(0) * getLinear(5)  * getLinear(11) + getLinear(0) * getLinear(7)  * getLinear(9)  + getLinear(4) * getLinear(1) * getLinear(11) - getLinear(4) * getLinear(3) * getLinear(9)  - getLinear(8)  * getLinear(1) * getLinear(7)  + getLinear(8)  * getLinear(3) * getLinear(5);
        inv[15] =  getLinear(0) * getLinear(5)  * getLinear(10) - getLinear(0) * getLinear(6)  * getLinear(9)  - getLinear(4) * getLinear(1) * getLinear(10) + getLinear(4) * getLinear(2) * getLinear(9)  + getLinear(8)  * getLinear(1) * getLinear(6)  - getLinear(8)  * getLinear(2) * getLinear(5);

        det = getLinear(0) * inv[0] + getLinear(1) * inv[4] + getLinear(2) * inv[8] + getLinear(3) * inv[12];
        if (det == 0) return false;
        det = 1.0 / det;

        for (i = 0; i < 16; i++)
            inverted.setLinear(i, inv[i] * det);

        return true;
    }

    double getLinear(int idx) {
        return data[idx%ROW][idx/COL];
    }

    void setLinear(int idx, double v) {
        data[idx%ROW][idx/COL] = v;
    }


};