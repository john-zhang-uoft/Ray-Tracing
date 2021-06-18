
#include <iostream>
#include <fstream>
#include "vec3.h"
#include "ray.h"
#include "hittable.h"
#include "hittable_list.h"
#include <limits>
#include "camera.h"

using namespace std;

#ifndef RAY_TRACING_COLOUR_GRADIENT_H
#define RAY_TRACING_COLOUR_GRADIENT_H

#define MAX_FLOAT (std::numeric_limits<float>::max())


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

    static vec3 color(const ray &r, hittable *world, int depth);

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

    hittable *list[6];
    list[0] = new sphere(vec3(0, 0, -3), 0.5, new lambertian(vec3(0.8, 0.3, 0.3)));
    list[1] = new sphere(vec3(0, -100.5, -1), 100, new lambertian(vec3(0.8, 0.8, 0.0)));
    list[2] = new sphere(vec3(1, 0, -2), 0.5, new metal(vec3(0.8, 0.6, 0.2)));
    list[3] = new sphere(vec3(-1, 0, -2), 0.5, new metal(vec3(0.8, 0.8, 0.8)));
    list[4] = new sphere(vec3(0.4, 0, -1), 0.2, new metal(vec3(0.8, 0.8, 0.8)));
    list[5] = new sphere(vec3(-0.3, 0, -1), 0.3, new metal(vec3(0.8, 0.8, 0.8)));

    hittable *world = new hittable_list(list, 6);
    camera cam;

    for (int y_ind = y_pixels - 1; y_ind >= 0; y_ind--) {
        for (int x_ind = 0; x_ind < x_pixels; x_ind++) {
            vec3 col(0, 0, 0);
            for (int s = 0; s < ns; s++) {
                float u = float(x_ind + get_random_number_0_to_1())/ float(x_pixels);
                float v = float(y_ind + get_random_number_0_to_1())/ float(y_pixels);
                ray ry = cam.get_ray(u, v);
                col += matte_color(ry, world);
            }
            col /= float(ns);
            col = vec3( sqrt(col[0]), sqrt(col[1]), sqrt(col[2]) );
            int r = int(255.99 * col[0]);
            int g = int(255.99 * col[1]);
            int b = int(255.99 * col[2]);

            File << r << " " << g << " " << b << "\n";
        }
    }

}

vec3 colour_gradient::color(const ray &r, hittable *world, int depth) {
    hit_record record;
    if (world->hit(r, 0.001, MAX_FLOAT, record)) {
        ray scattered;
        vec3 attenuation;
        if (depth < 50 && record.mat_ptr->scatter(r, record, attenuation, scattered)) {
            return attenuation * color(scattered, world, depth + 1);
        }
        else {
            return vec3(0, 0, 0);
        }
    }
    else {
        vec3 unit_direction = unit_vector(r.direction());
        float t = 0.5 * (unit_direction.y() + 1.0);
        return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
    }
}


vec3 colour_gradient::matte_color(const ray &r, hittable *world) {
    ray cur_ray = r;
    float cur_attenuation = 1.0;
    for (int i = 0; i < 50; i++) {
        hit_record record;
        if (world->hit(cur_ray, 0.001, MAX_FLOAT, record)) {
            vec3 target = record.point + record.normal + random_in_unit_sphere();
            cur_ray = ray(record.point, target - record.point);
            cur_attenuation *= 0.5;
        } else {
            vec3 unit_direction = unit_vector(cur_ray.direction());
            float t = 0.5 * (unit_direction.y() + 1.0);
            vec3 c = (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
            return cur_attenuation * c;
        }
    }
    return vec3(0.0, 0.0, 0.0); // exceeded maximum depth of recursion
}


#endif //RAY_TRACING_COLOUR_GRADIENT_H
