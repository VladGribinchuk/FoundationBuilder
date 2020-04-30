#pragma once

#include "Point.h"

#include <array>
#include <algorithm>

namespace geom_utils
{
    class FMatrix4x4
    {
        std::array<std::array<float, 4>, 4> m;
    public:
        FMatrix4x4()
        {
            identify();
        }

        FPoint3D apply(const FPoint3D& p) const
        {
            return FPoint3D(
                /* rotation component */ p.x * m[0][0] + p.y * m[1][0] + p.z * m[2][0] + /* translation component */ m[0][3],
                /* rotation component */ p.x * m[0][1] + p.y * m[1][1] + p.z * m[2][1] + /* translation component */ m[1][3],
                /* rotation component */ p.x * m[0][2] + p.y * m[1][2] + p.z * m[2][2] + /* translation component */ m[2][3]);
        }

        void setTranslation(const FPoint3D& p)
        {
            identify();
            m[0][3] = p.x;
            m[1][3] = p.y;
            m[2][3] = p.z;
        }

        void identify()
        {
            for (auto& row : m)
                std::fill(row.begin(), row.end(), 0.0);
            for (unsigned i = 0; i < 4; ++i)
                m[i][i] = 1.0;
        }

        std::array<float, 4>& operator[](unsigned index) { return m[index]; }
        const std::array<float, 4>& operator[](unsigned index) const { return m[index]; }
    };

}
