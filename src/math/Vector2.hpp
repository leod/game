#pragma once

#include <cmath>
#include <cassert>

namespace game {

template<typename T> struct Vector2 {
    T x, y; 

    Vector2()
        : x(), y() {
    }

    Vector2(const T& x, const T& y)
        : x(x), y(y) {
    }

    Vector2(const Vector2& v)
        : x(v.x), y(v.y) {
    }

    T length() const {
        return sqrt(x * x + y * y);
    }

    Vector2 normalized() const {
        return *this / length(); 
    }

    Vector2& operator+=(const Vector2& v) {
        x += v.x;
        y += v.y;

        return *this;
    }

    Vector2& operator-=(const Vector2& v) {
        x -= v.x;
        y -= v.y;

        return *this;
    }

    Vector2& operator*=(const T& t) {
        x *= t;
        y *= t;

        return *this;
    }

    Vector2& operator/=(const T& t) {
        assert(t != 0);

        x /= t;
        y /= t;

        return *this;
    }
};

template<typename T> Vector2<T> operator+(const Vector2<T>& a, const Vector2<T>& b) {
    return Vector2<T>(a.x + b.x, a.y + b.y);
}

template<typename T> Vector2<T> operator-(const Vector2<T>& a, const Vector2<T>& b) {
    return Vector2<T>(a.x - b.x, a.y - b.y);
}

template<typename T> Vector2<T> operator*(const Vector2<T>& v, const T& t) {
    return Vector2<T>(v.x * t, v.y * t);
}

template<typename T> Vector2<T> operator/(const Vector2<T>& v, const T& t) {
    assert(t != 0);

    return Vector2<T>(v.x / t, v.y / t);
}

template<typename T> Vector2<T> operator-(const Vector2<T>& v) {
    return Vector2<T>(-v.x, -v.y);
}

template<typename T> T dot(const Vector2<T>& a, const Vector2<T>& b) {
    return a.x * b.x + a.y * b.y;
}

typedef Vector2<float> Vector2f;

} // namespace game
