#pragma once

#include <limits>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include "Units.h"

namespace geom_utils
{
    template<typename T>
    struct Point2D
    {
        constexpr Point2D(T x = T{}, T y = T{})
            : x(x), y(y)
        {}
        T x, y;

        typedef T coord;
    };

    template<typename T>
    struct Point3D
    {
        constexpr Point3D(T x = T{}, T y = T{}, T z = T{})
            : x(x), y(y), z(z)
        {}
        T x, y, z;

        typedef T coord;
    };

    using FPoint2D = Point2D<mm>;
    using FPoint3D = Point3D<mm>;


    // Point2D related:

    template<typename Pnt>
    constexpr Pnt minPoint() { static_assert(false, "Not implemented yet for this type of Point"); return {}; }
    
    template<typename Pnt>
    constexpr Pnt maxPoint() { static_assert(false, "Not implemented yet for this type of Point"); return {}; }

    template<typename Pnt>
    constexpr Pnt notAPoint() { static_assert(false, "Not implemented yet for this type of Point"); return {}; }

    template<>
    constexpr FPoint2D minPoint() { return FPoint2D(std::numeric_limits<FPoint2D::coord>::min(), std::numeric_limits<FPoint2D::coord>::min()); }

    template<>
    constexpr FPoint2D maxPoint() { return FPoint2D(std::numeric_limits<FPoint2D::coord>::max(), std::numeric_limits<FPoint2D::coord>::max()); }

    template<>
    constexpr FPoint2D notAPoint() { return minPoint<FPoint2D>(); }

    template<typename T>
    constexpr Point2D<T> operator-(const Point2D<T>& p) { return Point2D<T>(-p.x, -p.y); }

    template<typename T>
    constexpr Point2D<T> operator+(const Point2D<T>& p, const Point2D<T>& p2) { return Point2D<T>(p.x + p2.x, p.y + p2.y); }

    template<typename T>
    constexpr Point2D<T> operator-(const Point2D<T>& p, const Point2D<T>& p2) { return Point2D<T>(p.x - p2.x, p.y - p2.y); }

    template<typename T>
    constexpr Point2D<T>& operator+=(Point2D<T>& p, const Point2D<T>& p2) { p.x += p2.x; p.y += p2.y; return p; }

    template<typename T>
    constexpr Point2D<T>& operator-=(Point2D<T>& p, const Point2D<T>& p2) { p.x -= p2.x; p.y -= p2.y; return p; }

    template<typename T, typename V>
    constexpr Point2D<T>  operator*(const Point2D<T>& p, const V val) { return Point2D<T>(p.x * val, p.y * val); }

    template<typename T, typename V>
    constexpr Point2D<T>  operator*(const V i, const Point2D<T>& p) { return p * i; }

    template<typename T>
    constexpr bool operator==(const Point2D<T>& lhs, const Point2D<T>& rhs) { return lhs.x == rhs.x && lhs.y == rhs.y; }

    template<typename T>
    constexpr bool operator!=(const Point2D<T>& lhs, const Point2D<T>& rhs) { return !(lhs == rhs); }

    // special treatment for float-point number
    template<>
    inline bool operator==(const Point2D<float>& lhs, const Point2D<float>& rhs) { return std::fabs(lhs.x - rhs.x) < getUnitTolerance() && std::fabs(lhs.y - rhs.y) < getUnitTolerance(); }
    template<>
    inline bool operator==(const Point2D<double>& lhs, const Point2D<double>& rhs) { return std::fabs(lhs.x - rhs.x) < (double)getUnitTolerance() && std::fabs(lhs.y - rhs.y) < (double)getUnitTolerance(); }


    // Point3D related:

    template<>
    constexpr FPoint3D minPoint<FPoint3D>() { return FPoint3D(std::numeric_limits<FPoint3D::coord>::min(), std::numeric_limits<FPoint3D::coord>::min(), std::numeric_limits<FPoint3D::coord>::min()); }

    template<>
    constexpr FPoint3D maxPoint<FPoint3D>() { return FPoint3D(std::numeric_limits<FPoint3D::coord>::max(), std::numeric_limits<FPoint3D::coord>::max(), std::numeric_limits<FPoint3D::coord>::max()); }

    template<>
    constexpr FPoint3D notAPoint() { return minPoint<FPoint3D>(); }

    template<typename T>
    constexpr Point3D<T> operator-(const Point3D<T>& p) { return Point3D<T>(-p.x, -p.y, -p.z); }

    template<typename T>
    constexpr Point3D<T> operator+(const Point3D<T>& p, const Point3D<T>& p2) { return Point3D<T>(p.x + p2.x, p.y + p2.y, p.z + p2.z); }

    template<typename T>
    constexpr Point3D<T> operator-(const Point3D<T>& p, const Point3D<T>& p2) { return Point3D<T>(p.x - p2.x, p.y - p2.y, p.z - p2.z); }

    template<typename T>
    constexpr Point3D<T>& operator+=(Point3D<T>& p, const Point3D<T>& p2) { p.x += p2.x; p.y += p2.y; p.z += p2.z; return p; }

    template<typename T>
    constexpr Point3D<T>& operator-=(Point3D<T>& p, const Point3D<T>& p2) { p.x -= p2.x; p.y -= p2.y; p.z -= p2.z; return p; }

    template<typename T, typename V>
    constexpr Point3D<T>  operator*(const Point3D<T>& p, const V val) { return Point3D<T>(p.x * val, p.y * val, p.z * val); }

    template<typename T, typename V>
    constexpr Point3D<T>  operator*(const V i, const Point3D<T>& p) { return p * i; }

    template<typename T>
    constexpr bool operator==(const Point3D<T>& lhs, const Point3D<T>& rhs) { return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z; }

    template<typename T>
    constexpr bool operator!=(const Point3D<T>& lhs, const Point3D<T>& rhs) { return !(lhs == rhs); }

    // special treatment for float-point number
    template<>
    inline bool operator==(const Point3D<float>& lhs, const Point3D<float>& rhs) { return std::fabs(lhs.x - rhs.x) < getUnitTolerance() && std::fabs(lhs.y - rhs.y) < getUnitTolerance() && std::fabs(lhs.z - rhs.z) < getUnitTolerance(); }
    template<>
    inline bool operator==(const Point3D<double>& lhs, const Point3D<double>& rhs) { return std::fabs(lhs.x - rhs.x) < (double)getUnitTolerance() && std::fabs(lhs.y - rhs.y) < (double)getUnitTolerance() && std::fabs(lhs.z - rhs.z) < (double)getUnitTolerance(); }


    // Point output/input operators

    template<typename T>
    constexpr std::string PointToString(const Point2D<T>& p)
    {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(3) << "{" << p.x << "; " << p.y << "}";
        return ss.str();
    }

    template<typename T>
    constexpr std::string PointToString(const Point3D<T>& p)
    {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(3) << "{" << p.x << "; " << p.y << "; " << p.z << "}";
        return ss.str();
    }

    template<typename T>
    constexpr std::ostream& operator<<(std::ostream& os, const Point2D<T>& p)
    {
        os << PointToString(p);
        return os;
    }

    template<typename T>
    constexpr std::ostream& operator<<(std::ostream& os, const Point3D<T>& p)
    {
        os << PointToString(p);
        return os;
    }

    inline std::string ltrim(std::string s) { s.erase(s.begin(), std::find_if_not(s.begin(), s.end(), isspace)); return s; }
    inline std::string rtrim(std::string s) { s.erase(std::find_if_not(s.rbegin(), s.rend(), isspace).base(), s.end()); return s; }
    inline std::string trim(std::string s)  { return ltrim(rtrim(s)); }

    template<typename T>
    constexpr T stringTo(const std::string& s)
    {
        std::stringstream ss(s);
        T val; ss >> val;
        return val;
    }

    template<typename T>
    inline std::istream& operator>>(std::istream& is, Point2D<T>& p)
    {
        std::string s;
        char c;
        size_t brace1 = std::string::npos, brace2 = std::string::npos, semicol = std::string::npos;
        while (is.get(c))
        {
            s += c;
            if (c == '{')
                brace1 = s.length() - 1;
            if (c == '}')
            {
                brace2 = s.length() - 1;
                break;
            }
            if (c == ';')
                semicol = s.length() - 1;
        }

        if (/*point defined correct*/brace1 != std::string::npos && brace2 != std::string::npos && semicol != std::string::npos)
        {
            p.x = stringTo<T>(trim(s.substr(brace1 + 1, semicol - brace1 - 1)));
            p.y = stringTo<T>(trim(s.substr(semicol + 1, brace2 - semicol - 1)));
        }
        else
            p = notAPoint<Point2D<T>>();

        return is;
    }

    template<typename T>
    inline std::istream& operator>>(std::istream& is, Point3D<T>& p)
    {
        std::string s;
        char c;
        size_t brace1 = std::string::npos, brace2 = std::string::npos, semicol1 = std::string::npos, semicol2 = std::string::npos;
        while (is.get(c))
        {
            s += c;
            if (c == '{')
                brace1 = s.length() - 1;
            if (c == '}')
            {
                brace2 = s.length() - 1;
                break;
            }
            if (c == ';')
            {
                if (semicol1 == std::string::npos)
                    semicol1 = s.length() - 1;
                else
                    semicol2 = s.length() - 1;
            }
        }

        if (/*point defined correct*/brace1 != std::string::npos && brace2 != std::string::npos && semicol1 != std::string::npos && semicol2 != std::string::npos)
        {
            p.x = stringTo<T>(trim(s.substr(brace1 + 1, semicol1 - brace1 - 1)));
            p.y = stringTo<T>(trim(s.substr(semicol1 + 1, semicol2 - semicol1 - 1)));
            p.z = stringTo<T>(trim(s.substr(semicol2 + 1, brace2 - semicol2 - 1)));
        }
        else
            p = notAPoint<Point3D<T>>();

        return is;
    }

} // namespace geom_utils