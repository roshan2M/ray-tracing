#ifndef MATERIAL_H
#define MATERIAL_H

#include "rtwknd.h"

class hit_record;

class material {
    public: 
        virtual ~material() = default;

        virtual bool scatter(
            const ray& r, const hit_record& rec, color& attenuation, ray& scattered
        ) const {
            return false;
        }
};

class lambertian : public material {
    public:
        lambertian(const color& albedo): albedo(albedo) {}

        bool scatter(
            const ray& r, const hit_record& rec, color& attenuation, ray& scattered
        ) const override {
            auto scatter_direction = rec.normal + random_unit_vector();
            if (scatter_direction.near_zero()) {
                scatter_direction = rec.normal;
            }

            scattered = ray(rec.p, scatter_direction);
            attenuation = albedo;
            return true;
        }

    private:
        color albedo;
};

class metal : public material {
    public:
        metal(const color& albedo, double fuzz): albedo(albedo), fuzz(std::fmin(1, fuzz)) {}

        bool scatter(
            const ray& r, const hit_record& rec, color& attenuation, ray& scattered
        ) const override {
            vec3 reflected = reflect(r.direction(), rec.normal);
            reflected = unit_vector(reflected) + (fuzz * random_unit_vector());
            scattered = ray(rec.p, reflected);
            attenuation = albedo;
            return dot(scattered.direction(), rec.normal) > 0;
        }

    private:
        color albedo;
        double fuzz;
};

class dielectric : public material {
    public:
        dielectric(double refraction_index): refraction_index(refraction_index) {}

        bool scatter(
            const ray& r, const hit_record& rec, color& attenuation, ray& scattered
        ) const override {
            attenuation = color(1.0, 1.0, 1.0);
            double ri = rec.front_face ? 1.0 / refraction_index : refraction_index;

            vec3 unit_dir = unit_vector(r.direction());
            double cos_theta = std::fmin(dot(-unit_dir, rec.normal), 1.0);
            double sin_theta = std::sqrt(1.0 - cos_theta * cos_theta);

            bool cannot_refract = ri * sin_theta > 1.0;
            vec3 outgoing_dir;

            // If this condition is true the ray will reflect.
            if (cannot_refract || reflectance(cos_theta, ri) > random_double()) {
                outgoing_dir = reflect(unit_dir, rec.normal);
            }
            // Otherwise the ray will refract.
            else {
                outgoing_dir = refract(unit_dir, rec.normal, ri);
            }
            scattered = ray(rec.p, outgoing_dir);
            return true;
        }

    private:
        double refraction_index;

        static double reflectance(double cosine, double refraction_idx) {
            // Use Schlick's approximation for reflectance.
            auto r0 = (1 - refraction_idx) / (1 + refraction_idx);
            r0 = r0*r0;
            return r0 + (1-r0) * std::pow(1 - cosine, 5);
        }
};

#endif
