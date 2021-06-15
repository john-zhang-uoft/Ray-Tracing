#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"
//
// Created by johnz on 2021-06-12.
//
#include "ray.h"

#ifndef RAY_TRACING_HITTABLE_H
#define RAY_TRACING_HITTABLE_H

struct hit_record {
    // Time
    float t;
    // Point hit
    vec3 point;
    // Normal vector to point
    vec3 normal;
};

class hittable {
public:
    // pure virtual function for determining whether a hittable has been hit
    virtual bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const = 0;
};


class sphere : public hittable {
public:
    sphere() = default;

    sphere(vec3 c, float r) : center(c), radius(r) {};

    bool hit(const ray &r, float t_min, float t_max, hit_record &record) const override;

    vec3 center{};
    float radius{};
};


bool sphere::hit(const ray &r, float t_min, float t_max, hit_record &record) const {
    // Because the result of the dot product of oc and r.direction times 2 is b in the quadratic equation,
    // we say that the dot product of oc and r.direction is 1/2 b, so that the discriminant can be
    // written as 4 * half_b * half_b - 4 * a * c
    // We can factor out 4, when taken out of the square root that is applied to the discriminant,
    // becomes 2 * sqrt(half_b * half_b - a * c)
    // This 2 cancels out the 2 in the denominator 2a, so we simplify this expression
    // so that the "discriminant" is half_b * half_b - a * c, and the denominator is a
    // Note that this simplification does not effect whether a certain equation gives 1, 2, or no solutions

    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float half_b = dot(oc, r.direction());
    float c = dot(oc, oc) - radius * radius;
    float discriminant = half_b * half_b - a * c;

    // There are no solutions; no hits to the sphere
    if (discriminant < 0) {
        return false;
    }

    // If there are 1 or 2 solutions t to the quadratic equation (the ray intersects with the sphere)
    // Find the nearest root that is in the the range of view (between t_min and t_max)

    double sqrt_discriminant = sqrt(discriminant);

    // Start by seeing if -half_b - sqrt_discriminant is valid in the field of view
    // If valid, that means that it is the smallest valid root
    float root = -half_b - sqrt_discriminant / a;
    // If not valid, consider the larger root (farther point)
    if (root < t_min || t_max < root) {
        root = -half_b + sqrt_discriminant / a;
        if (root < t_min || t_max < root) {
            return false;
        }
    }
    // set the parameter of the hit record
    record.t = root;
    // set the point at which the sphere was hit
    record.point = r.point_given_parameter(record.t);
    // set the normal vector of that point
    record.normal = (record.point - center) / radius;
    return true;
}

#endif //RAY_TRACING_HITTABLE_H
