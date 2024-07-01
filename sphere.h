#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

class sphere : public hittable {
    public:
        sphere(const point3& center, double radius, std::shared_ptr<material> mat)
            : center(center), radius(std::fmax(0, radius)), mat(mat) {}

        bool hit(const ray& r, interval ray_t, hit_record& rec) const override {
            vec3 oc = center - r.origin();
            auto d = r.direction();

            auto a = d.length_sqr();
            auto h = dot(d, oc);
            auto c = oc.length_sqr() - radius * radius;
            auto disc = h*h - a*c;
            if (disc < 0) {
                return false;
            }

            auto discSqrt = sqrt(disc);
            auto root = (h - discSqrt) / a;
            if (!ray_t.surrounds(root)) {
                root = (h + discSqrt) / a;
                if (!ray_t.surrounds(root)) {
                    return false;
                }
            }
            rec.t = root;
            rec.p = r.at(rec.t);
            vec3 outward_normal = (rec.p - center) / radius;
            rec.set_face_normal(r, outward_normal);
            rec.mat = mat;

            return true;
        }

    private:
        point3 center;
        double radius;
        std::shared_ptr<material> mat;
};

#endif
