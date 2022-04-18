#include "Vecter3.h"
#include <math.h>

Vector3::Vector3() : x(0), y(0), z(0) {}

Vector3::Vector3(float x, float y, float z) : x(x), y(y), z(z) {}

float Vector3::length() const
{ return sqrtf((x * x) + (y * y) + (z * z)); }





