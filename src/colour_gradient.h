//
// Created by johnz on 2021-06-10.
//
#include <iostream>
#include <fstream>
#include "vec3.h"

using namespace std;

#ifndef RAY_TRACING_COLOUR_GRADIENT_H
#define RAY_TRACING_COLOUR_GRADIENT_H


class colour_gradient {
public:
    colour_gradient(int x_size, int y_size, float default_blue = 0.2) {
        x_pixels = x_size;
        y_pixels = y_size;
        default_b = default_blue;
    }
    int x_pixels;
    int y_pixels;
    float default_b;

    inline void draw_gradient(const string& filename) const;
};

inline void colour_gradient::draw_gradient(const string& filename) const  {
// Create then open text file
    ofstream File(filename);

    File << "P3\n" << x_pixels << " " << y_pixels << "\n255\n";

    for (int y_ind = y_pixels - 1; y_ind >= 0; y_ind--) {
        for (int x_ind = 0; x_ind < x_pixels; x_ind++) {
            vec3 colour(float(x_ind) / float(x_pixels), float(y_ind) / float(y_pixels), 0);
            int red = int(255.99 * colour[0]);
            int green = int(255.99 * colour[1]);
            int blue = int(default_b);

            File << red << " " << green << " " << blue << "\n";
        }
    }
    File.close();
}

#endif //RAY_TRACING_COLOUR_GRADIENT_H
