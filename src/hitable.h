#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"
//
// Created by johnz on 2021-06-12.
//
#include "ray.h"
#ifndef RAY_TRACING_HITABLE_H
#define RAY_TRACING_HITABLE_H

struct hit_record {
    float t;
    vec3 p;
    vec3 normal;
};

class hitable {
public:
    virtual bool hit(const ray& r, float t_min, float t_max, hit_record rec) const = 0;
};

class sphere: public hitable {
public:
    sphere() = default;
    sphere(vec3 c, float r) : center(cen), radius(r) {};
    virtual bool hit(const ray& r, float tmin, float tmax, hit_record &rec) const;
    vec3 center{};
    float radius{};
};

bool sphere::hit(const ray& r, float t_min, float t_max, hit_record & rec) const {
    vec3 oc = r.origin() - center;
    float a = dot(r.direction(), r.direction());
    float b = dot(oc, r.direction());
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b*b - a * c;

    // If there are solutions t to the quadratic equation (the ray intersects with the sphere)
    if (discriminant > 0) {
        float temp = (-b - sqrt(b*b - a*c)) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            return true;
        }
        temp = (-b + sqrt(b*b - a * c)) / a;
        if (temp < t_max && temp > t_min) {
            rec.t = temp;
            rec.p = r.point_at_parameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            return true;
        }
    }
    return false;
}
#endif //RAY_TRACING_HITABLE_H

#pragma clang diagnostic pop