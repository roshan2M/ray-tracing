#ifndef VEC3_H
#define VEC3_H

class vec3 {
    public:
        double x;
        double y;
        double z;

        vec3(): x(0), y(0), z(0) {}
        vec3(double x, double y, double z): x(x), y(y), z(z) {}

        vec3 operator-() const { return vec3(-x, -y, -z); }

        vec3& operator+=(const vec3& v) {
            x += v.x;
            y += v.y;
            z += v.z;
            return *this;
        }

        vec3& operator*=(double t) {
            x *= t;
            y *= t;
            z *= t;
            return *this;
        }

        vec3& operator/=(double t) { return *this *= 1/t; }

        double length_sqr() const { return x*x + y*y + z*z; }

        double length() const { return std::sqrt(length_sqr()); }

        static vec3 random() {
            return vec3(random_double(), random_double(), random_double());
        }

        static vec3 random(double min, double max) {
            return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
        }

        bool near_zero() const {
            // Return true if the vector is close to zero in all dimensions.
            auto s = 1e-8;
            return (std::fabs(x) < s) && (std::fabs(y) < s) && (std::fabs(z) < s);
        }
};

using point3 = vec3;

// Vector utility functions

inline std::ostream& operator<<(std::ostream& out, const vec3& v) {
    return out << v.x << ' ' << v.y << ' ' << v.z;
}

inline vec3 operator+(const vec3& u, const vec3& v) {
    return vec3(u.x + v.x, u.y + v.y, u.z + v.z);
}

inline vec3 operator-(const vec3& u, const vec3& v) {
    return vec3(u.x - v.x, u.y - v.y, u.z - v.z);
}

inline vec3 operator*(const vec3& u, const vec3& v) {
    return vec3(u.x * v.x, u.y * v.y, u.z * v.z);
}

inline vec3 operator*(double t, const vec3& v) {
    return vec3(t * v.x, t * v.y, t * v.z);
}

inline vec3 operator*(const vec3& v, double t) {
    return t * v;
}

inline vec3 operator/(const vec3& v, double t) {
    return (1/t) * v;
}

inline double dot(const vec3& u, const vec3& v) {
    return u.x * v.x
         + u.y * v.y
         + u.z * v.z;
}

inline vec3 cross(const vec3& u, const vec3& v) {
    return vec3(u.y * v.z - u.z * v.y,
                u.z * v.x - u.x * v.z,
                u.x * v.y - u.y * v.x);
}

inline vec3 unit_vector(const vec3& v) {
    return v / v.length();
}

inline vec3 random_unit_vector() {
    return unit_vector(vec3::random(-1,1));
}

inline vec3 random_on_hemisphere(const vec3& normal) {
    vec3 on_unit_sphere = random_unit_vector();
    if (dot(on_unit_sphere, normal) > 0.0) // In the same hemisphere as the normal
        return on_unit_sphere;
    else
        return -on_unit_sphere;
}

inline vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2*dot(v,n)*n;
}

inline vec3 refract(const vec3& iv, const vec3& n, double etai_over_etat) {
    auto cos_theta = std::fmin(dot(-iv, n), 1.0);
    auto r_out_perp = etai_over_etat * (iv + n * cos_theta);
    auto r_out_parl = -n * std::sqrt(1.0 - r_out_perp.length_sqr());
    return r_out_perp + r_out_parl;
}

#endif
