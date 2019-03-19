#include "PointVector.h"

namespace rt
{
struct Wave
{
    Real phase;
    Real r;
    Real a;
    Real l;

    Wave(){};
    Wave(Real r, Real a, Real l, Real phase) : phase(phase), r(r), a(a), l(l) {}
};
} // namespace rt