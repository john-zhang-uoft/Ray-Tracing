
#include <iostream>
#include <fstream>
#include "vec3.h"
#include "ray.h"

using namespace std;

#ifndef RAY_TRACING_COLOUR_GRADIENT_H
#define RAY_TRACING_COLOUR_GRADIENT_H


class colour_gradient {
public:
    colour_gradient(int x_size, int y_size) {
        x_pixels = x_size;
        y_pixels = y_size;
    }

    int x_pixels;
    int y_pixels;

    inline void draw_diagonal_gradient(const string &filename, float default_blue) const;
    inline void draw_sky_background(const string &filename) const;
    static vec3 color(const ray& r);
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
    vec3 lower_left_corner(-2.0, -1.0, -1.0);
    vec3 horizontal(4.0, 0.0, 0.0);
    vec3 vertical(0.0, 2.0, 0.0);
    vec3 origin(0.0, 0.0, 0.0);
    for (int y_ind = y_pixels - 1; y_ind >= 0; y_ind--) {
        for (int x_ind = 0; x_ind < x_pixels; x_ind++) {
            float u = float(x_ind) / float(x_pixels);
            float v = float(y_ind) / float(y_pixels);

            ray ry(origin, lower_left_corner + u * horizontal + v * vertical);
            vec3 col = color(ry);
            int r = int(255.99 * col[0]);
            int g = int(255.99 * col[1]);
            int b = int(255.99 * col[2]);

            File << r << " " << g << " " << b << "\n";
        }
    }

}

vec3 colour_gradient::color(const ray &r) {
    vec3 unit_direction = unit_vector(r.direction());
    float t = 0.5 * (unit_direction.y() + 1.0);
    return (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
}


#endif //RAY_TRACING_COLOUR_GRADIENT_H
