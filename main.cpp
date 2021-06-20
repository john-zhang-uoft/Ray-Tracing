
#include <src/colour_gradient.h>

int main() {
    auto * gradient = new colour_gradient(1600, 800, 10);
//    gradient->draw_diagonal_gradient("Gradient.ppm", 255);
    gradient->draw_random_scene("Random Scene.ppm");
}