#pragma once

#include "utils.hpp"

#include <cmath>
#include <cstdint>
#include <algorithm>
#include <tuple>

namespace plot
{

struct Color;

struct Color32 {
    std::uint8_t r, g, b, a;
};

constexpr Color operator+(Color const& lhs, Color const& rhs);
constexpr Color operator-(Color const& lhs, Color const& rhs);
constexpr Color operator*(Color const& lhs, float const& rhs);
constexpr Color operator*(float const& lhs, Color const& rhs);
constexpr Color operator/(Color const& lhs, float const& rhs);
constexpr Color operator/(float const& lhs, Color const& rhs);

struct Color {
    constexpr Color() : r(0), g(0), b(0), a(0) {}

    constexpr Color(float r, float g, float b, float a = 1.0f)
        : r(r), g(g), b(b), a(a)
        {}

    constexpr Color(Color32 c, std::uint8_t white = 255, std::uint8_t opaque = 255)
        : r(float(c.r)/white), g(float(c.g)/white), b(float(c.b)/white), a(float(c.a)/opaque)
        {}

    constexpr Color premultiplied() const {
        return { r*a, g*a, b*a, a };
    }

    constexpr Color unpremultiplied() const {
        return { r/a, g/a, b/a, a };
    }

    constexpr float distance(Color const& other) const {
        auto rmean = (other.r + r)/2;
        auto diff = other - (*this);
        return std::sqrt((2.0f + rmean)*diff.r*diff.r + 4.0f*diff.g*diff.g + (3.0f - rmean)*diff.b*diff.b);
    }

    constexpr float hue_distance(Color const& other) const {
        return std::abs(other.hue() - hue());
    }

    constexpr float hue() const {
        float min = utils::min(r, utils::min(g, b)),
              max = utils::max(r, utils::max(g, b));
        auto h = (max == r) ? (g - b)/(max - min) :
                 (max == g) ? 2.0f + (b - r)/(max - min) : 4.0f + (r - g)/(max - min);
        return (h >= 0.0f) ? h : h + 360.0f;
    }

    constexpr Color over(Color const& other) const {
        return (premultiplied() + other.premultiplied()*(1.0f - a)).unpremultiplied();
    }

    constexpr Color32 color32(std::uint8_t white = 255, std::uint8_t opaque = 255) const {
        using utils::clamp;
        return {
            std::uint8_t(std::lround(clamp(r, 0.0f, 1.0f)*white)),
            std::uint8_t(std::lround(clamp(g, 0.0f, 1.0f)*white)),
            std::uint8_t(std::lround(clamp(b, 0.0f, 1.0f)*white)),
            std::uint8_t(std::lround(clamp(a, 0.0f, 1.0f)*opaque))
        };
    }

    Color& operator+=(Color const& other) {
        return (*this) = (*this) + other;
    }

    Color& operator-=(Color const& other) {
        return (*this) = (*this) - other;
    }

    Color& operator*=(float n) {
        return (*this) = (*this) * n;
    }

    Color& operator/=(float n) {
        return (*this) = (*this) / n;
    }

    constexpr bool operator==(Color const& other) const {
        return r == other.r && g == other.g && b == other.b && a == other.a;
    }

    constexpr bool operator!=(Color const& other) const {
        return r != other.r || g != other.g || b != other.b || a != other.a;
    }

    float r, g, b, a;
};

inline constexpr Color operator+(Color const& lhs, Color const& rhs) {
    return {
        lhs.r + rhs.r,
        lhs.g + rhs.g,
        lhs.b + rhs.b,
        lhs.a + rhs.a
    };
}

inline constexpr Color operator-(Color const& lhs, Color const& rhs) {
    return {
        lhs.r - rhs.r,
        lhs.g - rhs.g,
        lhs.b - rhs.b,
        lhs.a - rhs.a
    };
}

inline constexpr Color operator*(Color const& lhs, float const& rhs) {
    return {
        lhs.r*rhs,
        lhs.g*rhs,
        lhs.b*rhs,
        lhs.a*rhs
    };
}

inline constexpr Color operator*(float const& lhs, Color const& rhs) {
    return {
        lhs*rhs.r,
        lhs*rhs.g,
        lhs*rhs.b,
        lhs*rhs.a
    };
}

inline constexpr Color operator/(Color const& lhs, float const& rhs) {
    return {
        lhs.r/rhs,
        lhs.g/rhs,
        lhs.b/rhs,
        lhs.a/rhs
    };
}

inline constexpr Color operator/(float const& lhs, Color const& rhs) {
    return {
        lhs/rhs.r,
        lhs/rhs.g,
        lhs/rhs.b,
        lhs/rhs.a
    };
}

} /* namespace plot */