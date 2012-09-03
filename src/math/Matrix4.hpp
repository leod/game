#pragma once

#include <cmath>
#include <cassert>

namespace game {

template<typename T> struct Matrix4 {
    T m11, m12, m13, m14,
      m21, m22, m23, m24,
      m31, m32, m33, m34,
      m41, m42, m43, m44;

    Matrix4() {

    }

    Matrix4& operator+=(const Matrix4& m) {

    }

    Matrix4& operator-=(const Matrix4& m) {

    }

    Matrix4& operator*=(const T& t) {

    }

    Matrix4 inverted() const {

    }

    T determinant() const {

    }
};

template<typename T> Matrix4<T> operator+(const Matrix4<T>& a,
                                          const Matrix4<T>& b) {
    Matrix4<T> result = a;
    result += b;

    return result;
}

template<typename T> Matrix4<T> operator-(const Matrix4<T>& a,
                                          const Matrix4<T>& b) {
    Matrix4<T> result = a;
    result -= b;

    return result;
}

template<typename T> Matrix4<T> operator*(const Matrix4<T>& m, const T& t) {
    Matrix4<T> result = m;
    result *= t;

    return result;
}

template<typename T> Matrix4<T> operator*(const Matrix4<T>& a,
                                          const Matrix4<T>& b) {

}

typedef Matrix4<float> Matrix4f;

} // namespace game
