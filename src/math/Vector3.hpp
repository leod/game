#pragma once

#include <cmath>
#include <cassert>

namespace game {

template<typename T> struct Vector3 {
    T x, y, z;

    Vector3()
        : x(), y(), z() {
    }

    Vector3(const T& x, const T& y, const T& z)
        : x(x), y(y), z(z) {
    }

    Vector3(const Vector3& v)
        : x(v.x), y(v.y), z(v.z) {
    }

    T length() const {
        return sqrt(x * x + y * y + z * z);
    }

    Vector3 normalized() const {
        return *this / length();
    }

    Vector3& operator+=(const Vector3& v) {
        x += v.x;
        y += v.y;
        z += v.z;

        return *this;
    }

    Vector3& operator-=(const Vector3& v) {
        x -= v.x;
        y -= v.y;
        z -= v.z;

        return *this;
    }

    Vector3& operator*=(const T& t) {
        x *= t;
        y *= t;
        z *= t;

        return *this;
    }

    Vector3& operator/=(const T& t) {
        assert(t != 0);

        x /= t;
        y /= t;
        z /= t;

        return *this;
    }
};

template<typename T> Vector3<T> operator+(const Vector3<T>& a, const Vector3<T>& b) {
    return Vector3<T>(a.x + b.x, a.y + b.y, a.z + b.z);
}

template<typename T> Vector3<T> operator-(const Vector3<T>& a, const Vector3<T>& b) {
    return Vector3<T>(a.x - b.x, a.y - b.y, a.z - b.z);
}

template<typename T> Vector3<T> operator*(const Vector3<T>& v, const T& t) {
    return Vector3<T>(v.x * t, v.y * t, v.z * t);
}

template<typename T> Vector3<T> operator/(const Vector3<T>& v, const T& t) {
    assert(t != 0);

    return Vector3<T>(v.x / t, v.y / t, v.z / t);
}

template<typename T> Vector3<T> operator-(const Vector3<T>& v) {
    return Vector3<T>(-v.x, -v.y, -v.z);
}

template<typename T> T dot(const Vector3<T>& a, const Vector3<T>& b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

template<typename T> Vector3<T> cross(const Vector3<T>& a, const Vector3<T>& b) {
    // TODO
    assert(false);
    return Vector3<T>();
}

typedef Vector3<float> Vector3f;

} // namespace game
