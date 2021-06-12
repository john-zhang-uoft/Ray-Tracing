
#ifndef RAY_TRACING_RAY_H
#define RAY_TRACING_RAY_H

#include "vec3.h"

class ray {
    // Ray is represented by the equation p(t) = A + t * B
public:
    ray() = default;

    ray(const vec3 &a, const vec3 &b) {
        A = a;
        B = b;
    }

    vec3 origin() const {
        return A;
    }

    vec3 direction() const {
        return B;
    }

    vec3 point_at_parameter(float t) const {
        return A + t * B;
    }

    vec3 A;
    vec3 B;
};


#endif //RAY_TRACING_RAY_H
