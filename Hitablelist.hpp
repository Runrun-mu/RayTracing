#pragma once

#include "Hitable.hpp"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class hitablelist: public hitable {
    public:
        hitablelist(){}
        hitablelist(shared_ptr<hitable> object) { add(object); }

        void clear() { objects.clear(); }
        void add(shared_ptr<hitable> object) { objects.push_back(object); }

        virtual bool hit(const ray& r, double t_min, double t_max, hitrecord& rec) const override;

    public:
        std::vector<shared_ptr<hitable>> objects;
};

bool hitablelist::hit(const ray& r, double t_min, double t_max, hitrecord& rec) const {
    hitrecord temp_rec;
    bool hit_anything = false;
    auto closest_so_far = t_max;

    for(const auto& object : objects){
        if(object->hit(r, t_min, closest_so_far, temp_rec)){
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }

    return hit_anything;
}