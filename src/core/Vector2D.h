//
// Created by Admin on 3/16/2025.
//

#ifndef VECTOR2D_H
#define VECTOR2D_H




class Vector2D {
public:
    float x, y;

    Vector2D();
    Vector2D(float x, float y);

    Vector2D operator+(const Vector2D& v) const; // cong 2 vector
    Vector2D operator-(const Vector2D& v) const; // tru 2 vector
    Vector2D operator*(float scalar) const;      // nhan vector voi 1 hang so
    Vector2D operator/(float scalar) const;      // chia vector voi 1 hang so

    Vector2D& operator+=(const Vector2D& v);
    Vector2D& operator-=(const Vector2D& v);
    Vector2D& operator*=(float scalar);
    Vector2D& operator/=(float scalar);

    float length() const; // do dai vector
    Vector2D normalize() const; // chuan hoa vector ve do dai bang 1
    float distance(const Vector2D& v) const; // tinh khoang cach giua vector nay voi 1 vector khac
};



#endif //VECTOR2D_H
