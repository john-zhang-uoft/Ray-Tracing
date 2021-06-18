
#ifndef RAY_TRACING_HITTABLE_H
#define RAY_TRACING_HITTABLE_H

#include "ray.h"
#include <random>

float get_random_number_0_to_1();
vec3 random_in_unit_sphere();

class hittable;
class sphere;
class material;
class lambertian;
class metal;

struct hit_record {
    // Time
    float t;
    // Point hit
    vec3 point;
    // Normal vector to point
    vec3 normal;
    material *mat_ptr;
};

class hittable {
public:
    // pure virtual function for determining whether a hittable has been hit
    virtual bool hit(const ray &r, float t_min, float t_max, hit_record &rec) const = 0;
};


class sphere : public hittable {
public:
    sphere() = default;

    sphere(vec3 c, float r, material* m) : center(c), radius(r), mat(m) {};

    bool hit(const ray &r, float t_min, float t_max, hit_record &record) const override;

    vec3 center{};
    float radius{};
    material *mat;
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
    float root = (-half_b - sqrt_discriminant) / a;
    // If not valid, consider the larger root (farther point)
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrt_discriminant) / a;
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
    record.mat_ptr = mat;
    return true;
}

class material {
public:
    virtual bool scatter(const ray& r_in, const hit_record& rec, vec3& attenuation, ray& scattered) const = 0;
};

vec3 reflect(const vec3 &v, const vec3 &n) {
    return v - (2*dot(v, n)*n);
}

class lambertian : public material {
public:
    lambertian(const vec3 &a) : albedo(a) {}

    virtual bool scatter(const ray &r_in, const hit_record &record, vec3 &attenuation, ray &scattered) const {
        vec3 target = record.point + record.normal + random_in_unit_sphere();
        scattered = ray(record.point, target - record.point);
        attenuation = albedo;
        return true;
    }

    vec3 albedo;
};

class metal : public material {
public:
    metal(const vec3 &a) : albedo(a) {}

    virtual bool scatter(const ray &r_in, const hit_record &rec, vec3 &attenuation, ray &scattered) const {
        vec3 reflected = reflect(unit_vector(r_in.direction()), rec.normal);
        scattered = ray(rec.point, reflected);
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }
    vec3 albedo;
};

float get_random_number_0_to_1() {
    return rand() / (RAND_MAX+ 1.0);
}

vec3 random_in_unit_sphere() {
    // Get a random vector with length less than 1
    vec3 point;
    do {
        point = (2.0 * vec3(get_random_number_0_to_1(),
                            get_random_number_0_to_1(),
                            get_random_number_0_to_1())) - vec3(1, 1, 1);
    } while (point.squared_length() >= 1.0);
    return point;
}



#endif //RAY_TRACING_HITTABLE_H
