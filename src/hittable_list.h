
#ifndef RAY_TRACING_HITTABLE_LIST_H
#define RAY_TRACING_HITTABLE_LIST_H

#include "hittable.h"

class hittable_list: public hittable {
public:
    hittable_list() = default;
    hittable_list(hittable **lst, int n) {
        // lst is the pointer to an empty array of hittables
        // n is the size of that list
        list = lst;
        list_size = n;
    }
    virtual bool hit(const ray& r, float tmin, float tmax, hit_record& record) const;
    hittable **list;
    int list_size;
};

bool hittable_list::hit(const ray& r, float t_min, float t_max, hit_record& record) const {
    // Given the maximum t, minimum t, the ray and and hit record passed by reference
    // Find whether something is hit, and if so, update the record to only consider the closest hit

    hit_record temp_rec;
    bool hit_anything = false;
    double closest_so_far = t_max;

    // For each hittable in the hittable_list
    for (int i = 0; i < list_size; i++) {
        // If the ray hits the hittable
        if (list[i]->hit(r, t_min, closest_so_far, temp_rec)) {
            // Use closest_so_far, starting at t_max, to continually limit upper bound of the range of t
            // that we consider, so that we end up with a minimum solution
            hit_anything = true;
            // Update closest to far if a solution t that is within the bounds t_min and closest_so_far is found
            closest_so_far = temp_rec.t;
            record = temp_rec;
        }
    }
    return hit_anything;
}


#endif //RAY_TRACING_HITTABLE_LIST_H
