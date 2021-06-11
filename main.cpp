//
// Created by johnz on 2021-06-10.
//
#include <colour_gradient.h>

int main() {
    auto * gradient = new colour_gradient(200, 100, 255);
    gradient->draw_gradient("Gradient.ppm");
}