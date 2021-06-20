
#ifndef RAY_TRACING_CREATE_SCENE_H
#define RAY_TRACING_CREATE_SCENE_H

#include "hittable.h"
#include "material.h"
#include "hittable_list.h"

hittable_list *random_scene() {
    int n = 500;
    hittable **list = new hittable *[n + 1];
    list[0] = new sphere(vec3(0, -1000, 0), 1000, new lambertian(vec3(0.5, 0.5, 0.5)));
    int i = 1;
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            float choose_mat = get_random_number_0_to_1();
            vec3 center(a + 0.9 * get_random_number_0_to_1(), 0.2, b + 0.9 * get_random_number_0_to_1());
            if ((center - vec3(4, 0.2, 0)).length() > 0.9) {
                if (choose_mat < 0.8) { // diffuse

                    list[i++] = new
                            sphere(center, 0.2, new lambertian(
                            vec3(get_random_number_0_to_1(), get_random_number_0_to_1(), get_random_number_0_to_1())));
                } else if (choose_mat < 0.95) {   // metal

                    list[i++] = new sphere(center, 0.2,
                                           new metal(vec3(0.5 * (1 + get_random_number_0_to_1()),
                                                          0.5 * (1 + get_random_number_0_to_1()),
                                                          0.5 * (1 + get_random_number_0_to_1())),
                                                     0.5 * (1 + get_random_number_0_to_1())));

                } else {  // glass
                    list[i++] = new sphere(center, 0.2, new dielectric(1.5));
                }
            }
        }
    }

    list[i++] = new sphere(vec3(0, 1, 0), 1.0, new dielectric(1.5));
    list[i++] = new sphere(vec3(-4, 1, 0), 1.0, new lambertian(vec3(0.4, 0.2, 0.1)));
    list[i++] = new sphere(vec3(4, 1, 0), 1.0, new metal(vec3(0.7, 0.6, 0.5), 0.0));

    return new hittable_list(list, i);
}

#endif //RAY_TRACING_CREATE_SCENE_H
