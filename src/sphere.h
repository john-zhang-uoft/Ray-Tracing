#include "colour_gradient.h"
#include "ray.h"
#include "vec3.h"
#ifndef RAY_TRACING_SPHERE_H
#define RAY_TRACING_SPHERE_H


float hit_sphere(const vec3 &center, float radius, const ray &r) {
    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = 2.0 * dot(oc, r.direction());
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b * b - 4 * a * c;

    // If there are no solutions to the quadratic equation solving for t
    if (discriminant < 0) {
        return -1.0;
    }
    else {
        // Return the smaller solution t (the side of the sphere closer to the camera)
        return (-b - sqrt(discriminant) ) / (2.0 * a);
    }
}

vec3 color(const ray& r) {
    float t = (hit_sphere(vec3(0, 0, -1), 0.5, r))
    if (t > 0.0) { // if the solutions are positive, that means, in front of the camera
        // (excludes objects that would be behind the camera)
        vec3 N = unit_vector(r.point_given_parameter(t) - vec3(0, 0, -1));
        return 0.5 * vec3(N.x() + 1, N.y() + 1, N.z() + 1)
    }
    vec3 unit_direction = unit_vector(r.direction());
    float t = 0.5 * (unit direction.y() + 1.0);
    return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}

#endif //RAY_TRACING_SPHERE_H
