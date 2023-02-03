//
// Created by Win10 on 2023/2/1.
//

#ifndef CTCI_RAYTRACER_MATRIX_H_
#define CTCI_RAYTRACER_MATRIX_H_

#include <cmath>
#include <iostream>

#include "xmath.h"

namespace xmath {

template <typename T>
class Vector2D
{
public:
	Vector2D() : e{0, 0} {}
	Vector2D(T e0, T e1) : e{e0, e1} {}

	T x() const { return e[0]; }
	T y() const { return e[1]; }

private:
	ValueType e[2];
};

template <typename T> using Point2D = Vector2D<T>;

template<typename T>
class Vector3D {
public:
	Vector3D() : e{0, 0, 0} {}
	Vector3D(T e0, T e1, T e2) : e{e0, e1, e2} {}

	T x() const { return e[0]; }
	T y() const { return e[1]; }
	T z() const { return e[2]; }

	Vector3D operator-() const { return Vector3D(-e[0], -e[1], -e[2]); }
	T operator[](int i) const { return e[i]; }
	T &operator[](int i) { return e[i]; }

	Vector3D &operator+=(const Vector3D &v)
	{
		e[0] += v.e[0];
		e[1] += v.e[1];
		e[2] += v.e[2];
		return *this;
	}

	Vector3D &operator*=(const T t)
	{
		e[0] *= t;
		e[1] *= t;
		e[2] *= t;
		return *this;
	}

	Vector3D &operator/=(const T t)
	{
		return *this *= 1 / t;
	}

	T length() const
	{
		return sqrt(length_squared());
	}

	T length_squared() const
	{
		return e[0] * e[0] + e[1] * e[1] + e[2] * e[2];
	}

	bool near_zero() const
	{
		// Return true if the vector is close to zero in all dimensions.
		const auto s = 1e-8;
		return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
	}

public:
	T e[3];
};


// Type aliases for Vector
template<typename T> using Point3D = Vector3D<T>;   // 3D point


// Vector Utility Functions
template<typename T>
inline std::ostream &operator<<(std::ostream &out, const Vector3D<T> &v)
{
	return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

template<typename T>
inline Vector3D<T> operator+(const Vector3D<T> &u, const Vector3D<T> &v)
{
	return Vector3D(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

template<typename T>
inline Vector3D<T> operator-(const Vector3D<T> &u, const Vector3D<T> &v)
{
	return Vector3D(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

template<typename T>
inline Vector3D<T> operator*(const Vector3D<T> &u, const Vector3D<T> &v)
{
	return Vector3D(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

template<typename T>
inline Vector3D<T> operator*(T t, const Vector3D<T> &v)
{
	return Vector3D(t * v.e[0], t * v.e[1], t * v.e[2]);
}

template<typename T>
inline Vector3D<T> operator*(const Vector3D<T> &v, T t)
{
	return t * v;
}

template<typename T>
inline Vector3D<T> operator/(Vector3D<T> v, T t)
{
	return (1 / t) * v;
}

template<typename T>
inline bool operator==(const Vector3D<T> &lhs, const Vector3D<T> &rhs)
{
	return lhs.e[0] == rhs.e[0] && lhs.e[1] == rhs.e[1] && lhs.e[2] == rhs.e[2];
}

template<typename T>
inline bool operator<(const Vector3D<T> &lhs, const Vector3D<T> &rhs)
{
	return (lhs.x() < rhs.x() || lhs.x() == rhs.x() && (lhs.y() < rhs.y() || lhs.y() == rhs.y() && lhs.z() < rhs.z()));
}

template<typename T>
inline T dot(const Vector3D<T> &u, const Vector3D<T> &v)
{
	return u.e[0] * v.e[0]
		+ u.e[1] * v.e[1]
		+ u.e[2] * v.e[2];
}

template<typename T>
inline Vector3D<T> cross(const Vector3D<T> &u, const Vector3D<T> &v)
{
	return Vector3D(u.e[1] * v.e[2] - u.e[2] * v.e[1],
					u.e[2] * v.e[0] - u.e[0] * v.e[2],
					u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

template<typename T>
inline Vector3D<T> unit_vector(Vector3D<T> v)
{
	return v / v.length();
}

template<typename T>
inline Vector3D<T> reflect(const Vector3D<T> &v, const Vector3D<T> &n)
{
	return v - 2 * dot(v, n) * n;
}

template<typename T>
inline Vector3D<T> refract(const Vector3D<T> &uv, const Vector3D<T> &n, T etai_over_etat)
{
	auto cos_theta = fmin(dot(-uv, n), 1.0);
	Vector3D r_out_perp = etai_over_etat * (uv + cos_theta * n);
	Vector3D r_out_parallel = -sqrt(fabs(1.0 - r_out_perp.length_squared())) * n;
	return r_out_perp + r_out_parallel;
}

}

#endif //CTCI_RAYTRACER_MATRIX_H_
