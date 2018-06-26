///
/// Basic 2D vector.
///

#ifndef Vector2_h
#define Vector2_h

class Vector2 {
private:
	float x;
	float y;
public:
	Vector2() : x(0), y(0) {};
	Vector2(const float& x, const float& y);
	Vector2(const Vector2& v);
	Vector2& operator+=(const Vector2& rhs);
	Vector2& operator=(const Vector2& rhs);
	Vector2 operator+(const Vector2& rhs);
	Vector2 operator*(const int& rhs);
	float X() const { return x; }
	float Y() const { return y; }
	void SetX(int val) { x = val; }
	void SetY(int val) { y = val; }
};

#endif // Vector2_h