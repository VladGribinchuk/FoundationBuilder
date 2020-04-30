#pragma once

#include <string>
#include <fstream>
#include "AABB.h"

namespace geom_utils
{

    class SVGWriter
    {
    public:
        enum Color
        {
            NONE,
            BLACK,
            GREEN
        };
        
        std::string toString(Color color)
        {
            switch (color)
            {
            case Color::BLACK: return "black";
            case Color::GREEN: return "green";
            case Color::NONE: return "none";
            default: return "black";
            }
        }

        float scale = 1;
        std::ofstream file;
        AABB2D aabb;

        SVGWriter(std::string filename, AABB2D aabb, double xSize = 1024, double ySize = 1024);
        ~SVGWriter();

        FPoint2D transform(const FPoint2D& p) { return FPoint2D((p.x - aabb.min().x) * scale, (p.y - aabb.min().y) * scale); }
        void writePoint(FPoint2D p, bool labelCoords, int size = 1, Color color = Color::BLACK);
        void writeLine(FPoint2D a, FPoint2D b, Color color = Color::BLACK, float stroke = 1.0);
    };
}