//
// Created by Admin on 3/16/2025.
//

#include "Vector2D.h"
#include <bits/stdc++.h>
using namespace std;


Vector2D::Vector2D() : x(0.0f), y(0.0f) {}
Vector2D::Vector2D(float x, float y) : x(x), y(y) {}

Vector2D Vector2D::operator+(const Vector2D& v) const {
    return Vector2D(x + v.x, y + v.y);
}

Vector2D Vector2D::operator-(const Vector2D& v) const {
    return Vector2D(x - v.x, y - v.y);
}

Vector2D Vector2D::operator*(float scalar) const {
    return Vector2D(x * scalar, y * scalar);
}

Vector2D Vector2D::operator/(float scalar) const {
    return Vector2D(x / scalar, y / scalar);
}

Vector2D& Vector2D::operator+=(const Vector2D& v) {
    x += v.x;
    y += v.y;
    return *this;
}

Vector2D& Vector2D::operator-=(const Vector2D& v) {
    x -= v.x;
    y -= v.y;
    return *this;
}

Vector2D& Vector2D::operator*=(float scalar) {
    x *= scalar;
    y *= scalar;
    return *this;
}

Vector2D& Vector2D::operator/=(float scalar) {
    x /= scalar;
    y /= scalar;
    return *this;
}

float Vector2D::length() const {
    return std::sqrt(x * x + y * y);
}

Vector2D Vector2D::normalize() const {
    float len = length();
    if (len > 0) {
        return Vector2D(x / len, y / len);
    }
    return *this;
}

float Vector2D::distance(const Vector2D& v) const {
    float dx = x - v.x;
    float dy = y - v.y;
    return std::sqrt(dx * dx + dy * dy);
}