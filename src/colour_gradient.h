
#include <iostream>
#include <fstream>
#include "vec3.h"
#include "ray.h"
#include "hittable.h"
#include "hittable_list.h"
#include <limits>
#include "camera.h"
#include <random>

using namespace std;

#ifndef RAY_TRACING_COLOUR_GRADIENT_H
#define RAY_TRACING_COLOUR_GRADIENT_H

#define MAX_FLOAT (std::numeric_limits<float>::max())

static float get_random_number_0_to_1();

class colour_gradient {
public:
    colour_gradient(int x_size, int y_size, int s_size) {
        x_pixels = x_size;
        y_pixels = y_size;
        ns = s_size;
    }

    int x_pixels;
    int y_pixels;
    int ns;

    inline void draw_diagonal_gradient(const string &filename, float default_blue) const;

    inline void draw_sky_background(const string &filename) const;

    static vec3 color(const ray &r, hittable *world);

    static vec3 matte_color(const ray &r, hittable *world);
};

inline void colour_gradient::draw_diagonal_gradient(const string &filename, float default_blue) const {
// Create then open text file
    ofstream File(filename);

    File << "P3\n" << x_pixels << " " << y_pixels << "\n255\n";

    for (int y_ind = y_pixels - 1; y_ind >= 0; y_ind--) {
        for (int x_ind = 0; x_ind < x_pixels; x_ind++) {
            vec3 colour(float(x_ind) / float(x_pixels), float(y_ind) / float(y_pixels), 0);
            int r = int(255.99 * colour[0]);
            int g = int(255.99 * colour[1]);
            int b = int(default_blue);

            File << r << " " << g << " " << b << "\n";
        }
    }
    File.close();
}

inline void colour_gradient::draw_sky_background(const string &filename) const {
    ofstream File(filename);

    File << "P3\n" << x_pixels << " " << y_pixels << "\n255\n";

    hittable *list[2];
    list[0] = new sphere(vec3(0, 0, -1), 0.5);
    list[1] = new sphere(vec3(0, -100.5, -1), 100);
    hittable *world = new hittable_list(list, 2);
    camera cam;
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> dist(0.0, 1.0);

    for (int y_ind = y_pixels - 1; y_ind >= 0; y_ind--) {
        for (int x_ind = 0; x_ind < x_pixels; x_ind++) {
            vec3 col(0, 0, 0);
            for (int s = 0; s < ns; s++) {
                float u = float(x_ind + dist(mt))/ float(x_pixels);
                float v = float(y_ind + dist(mt))/ float(y_pixels);
                ray ry = cam.get_ray(u, v);
                col += matte_color(ry, world);
            }
            col /= float(ns);
            int r = int(255.99 * col[0]);
            int g = int(255.99 * col[1]);
            int b = int(255.99 * col[2]);

            File << r << " " << g << " " << b << "\n";
        }
    }

}

vec3 colour_gradient::color(const ray &r, hittable *world) {
    hit_record record;
    if (world->hit(r, 0.0, MAX_FLOAT, record)) {
        return 0.5 * vec3(record.normal.x() + 1, record.normal.y() + 1, record.normal.z() + 1);
    } else {
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
    }
}

vec3 random_in_unit_sphere() {
    // Get a random vector with length less than 1
    vec3 point;

    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> dist(0.0, 1.0);

    do {
        point = (2.0 * vec3(dist(mt), dist(mt), dist(mt)))
                - vec3(1, 1, 1);
    } while (point.squared_length() >= 1.0);
    return point;
}

vec3 colour_gradient::matte_color(const ray &r, hittable *world) {
    hit_record record;
    if (world->hit(r, 0.0, MAX_FLOAT, record)) {
        vec3 target = record.point + record.normal + random_in_unit_sphere();
        return 0.5 * matte_color(ray(record.point, target - record.point), world);
    } else {
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
    }
}

static float get_random_number_0_to_1() {
    std::random_device rd;
    std::mt19937 mt(rd());
    std::uniform_real_distribution<float> dist(0.0, 1.0);
    return dist(mt);
}

#endif //RAY_TRACING_COLOUR_GRADIENT_H
