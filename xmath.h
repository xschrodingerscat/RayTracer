//
// Created by Win10 on 2023/2/2.
//

#ifndef CTCI_RAYTRACER_XMATH_H_
#define CTCI_RAYTRACER_XMATH_H_

#include <limits>

#include "common.h"

namespace xmath {

const ValueType infinity 	= std::numeric_limits<ValueType>::infinity();
const ValueType pi 			= 3.1415926535897932385;
const ValueType epsilon 	= std::numeric_limits<ValueType>::epsilon();
const ValueType epsilon16   = 1e-16;
const ValueType epsilon12   = 1e-12;
const ValueType epsilon09   = 1e-09;
const ValueType epsilon06   = 1e-06;
const ValueType epsilon03   = 1e-03;

// Utility Functions
inline double degrees_to_radians(double degrees) {
	return degrees * pi / 180.0;
}

inline int sgn(ValueType a) { return a < -epsilon ? -1 : a > epsilon;}
inline int cmp(ValueType a, ValueType b) { return sgn(a - b); }


}

#endif //CTCI_RAYTRACER_XMATH_H_














