
#ifndef RAY_TRACING_CAMERA_H
#define RAY_TRACING_CAMERA_H

#include "ray.h"

vec3 random_in_unit_disk() {
    vec3 p;
    do {
        p = 2.0 * vec3(get_random_number_0_to_1(), get_random_number_0_to_1(), 0) - vec3(1, 1, 0);
    } while (dot(p, p) >= 1.0);
    return p;
}


class camera {
public:
    camera(vec3 lookfrom, vec3 lookat, vec3 v_up, float vertical_fov, float aspect, float aperture, float focus_dist) { // vertical_fov is the top to bottom in degrees
        lens_radius = aperture / 2;
        float theta = vertical_fov * M_PI / 180;
        float half_height = tan(theta / 2);
        float half_width = aspect * half_height;

        origin = lookfrom;
        w = unit_vector(lookfrom - lookat);
        u = unit_vector(cross(v_up, w));
        v = cross(w, u);

        lower_left_corner = origin - half_width * focus_dist * u - half_height * focus_dist * v - focus_dist * w;
        horizontal = 2 * half_width * focus_dist * u;
        vertical = 2 * half_height * focus_dist * v;
    }

    ray get_ray(float s, float t) {
        vec3 rd = lens_radius * random_in_unit_disk();
        vec3 offset = u * rd.x() + v * rd.y();
        return ray(origin + offset, lower_left_corner + s * horizontal + t * vertical - origin - offset);
    }

    vec3 origin;
    vec3 lower_left_corner;
    vec3 horizontal;
    vec3 vertical;
    vec3 u, v, w;
    float lens_radius;
};

#endif //RAY_TRACING_CAMERA_H
