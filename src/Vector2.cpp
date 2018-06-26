///
/// Basic 2D vector.
///

#include "Vector2.h"

Vector2::Vector2(const float& x, const float& y) {
	this->x = x;
	this->y = y;
}

Vector2::Vector2(const Vector2& v) {
	x = v.X();
	y = v.Y();
}

Vector2& Vector2::operator+=(const Vector2& rhs) {
	x += rhs.X();
	y += rhs.Y();
	return *this;
}

Vector2& Vector2::operator=(const Vector2& rhs) {
	x = rhs.X();
	y = rhs.Y();
	return *this;
}

Vector2 Vector2::operator+(const Vector2& rhs) {
	Vector2 v;
	v.SetX(this->x + rhs.X());
	v.SetY(this->y + rhs.Y());
	return v;
}

Vector2 Vector2::operator*(const int& rhs) {
	Vector2 v;
	v.SetX(this->x * rhs);
	v.SetY(this->y * rhs);
	return v;
}