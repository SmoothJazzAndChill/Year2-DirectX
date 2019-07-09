#pragma once
#include <DirectXMath.h>
#include <math.h>

class Vector2
{
public:
	Vector2() : x(0), y(0) {}
	Vector2(float _x, float _y) : x(_x), y(_y) {}

	float x;
	float y;

	void Normalize()
	{
		float x2 = x * x;
		float y2 = y * y;
		float c2 = x2 + y2;
		float c = sqrt(c2);

		x = x/c;
		y = y/c;
	}

	DirectX::XMFLOAT2 GetDX()
	{
		DirectX::XMFLOAT2 f = { x, y };
		return f;
	}

	friend Vector2 operator*(const Vector2& v1, const Vector2& v2)
	{
		return Vector2(v1.x * v2.x, v1.y * v2.y);
	}

	friend Vector2 operator+(const Vector2& v1, const Vector2& v2)
	{
		return Vector2(v1.x + v2.x, v1.y + v2.y);
	}

	friend Vector2 operator-(const Vector2& v1, const Vector2& v2)
	{
		return Vector2(v1.x - v2.x, v1.y - v2.y);
	}

	friend Vector2 operator/(const Vector2& v1, const Vector2& v2)
	{
		return Vector2(v1.x / v2.x, v1.y / v2.y);
	}

	friend Vector2 operator*(const Vector2& v, const float& f)
	{
		return Vector2(v.x * f, v.y * f);
	}
};

class Vector3 : public Vector2
{
public:
	Vector3() : Vector2(), z(0) {}
	Vector3(Vector2 v, float _z) : Vector2(v.x, v.y), z(_z) {}
	Vector3(float _x, float _y, float _z) : Vector2(_x, _y), z(_z) {}

	float z;

	DirectX::XMFLOAT3 GetDX()
	{
		DirectX::XMFLOAT3 f = { x, y, z };
		return f;
	}

	friend Vector3 operator+(const Vector3& v1, const Vector3& v2)
	{
		return Vector3(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z);
	}

	friend Vector3 operator*(const Vector3& v1, const Vector3& v2)
	{
		return Vector3(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z);
	}

	friend Vector3 operator-(const Vector3& v1, const Vector3& v2)
	{
		return Vector3(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z);
	}

	friend Vector3 operator/(const Vector3& v1, const Vector3& v2)
	{
		return Vector3(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z);
	}

	friend Vector3 operator*(const Vector3& v, const float& f)
	{
		return Vector3(v.x * f, v.y * f, v.z * f);
	}
};

class Vector4 : public Vector3
{
public:
	Vector4() : Vector3(), w(0) {}
	Vector4(Vector2 v, float _z, float _w) : Vector3(Vector2(v.x, v.y), _z), w(_w){}
	Vector4(Vector3 v, float _w) : Vector3(v.x, v.y, v.z), w(_w){}
	Vector4(float _x, float _y, float _z, float _w): Vector3(_x, _y, _z), w(_w){}

	float w;

	DirectX::XMFLOAT4 GetDX()
	{
		DirectX::XMFLOAT4 f = { x, y, z, w };
		return f;
	}

	friend Vector4 operator+(const Vector4& v1, const Vector4& v2)
	{
		return Vector4(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w);
	}

	friend Vector4 operator*(const Vector4& v1, const Vector4& v2)
	{
		return Vector4(v1.x * v2.x, v1.y * v2.y, v1.z * v2.z, v1.w * v2.w);
	}

	friend Vector4 operator-(const Vector4& v1, const Vector4& v2)
	{
		return Vector4(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w);
	}

	friend Vector4 operator/(const Vector4& v1, const Vector4& v2)
	{
		return Vector4(v1.x / v2.x, v1.y / v2.y, v1.z / v2.z, v1.w / v2.w);
	}
};
