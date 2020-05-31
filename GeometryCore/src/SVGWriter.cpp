#include "../include/SVGWriter.h"

#include <algorithm>

namespace geom_utils
{
    SVGWriter::SVGWriter(std::string filename, AABB2D aabb, double xSize, double ySize)
        : aabb(aabb), xSize(xSize), ySize(ySize), scale(std::min(float(xSize - xSize / 5 * 2) / (aabb.max().x - aabb.min().x), float(ySize - ySize / 5) / (aabb.max().y - aabb.min().y)))
    {
        file.open(filename.c_str(), std::ios_base::out);
        if (file.is_open())
        {
            file << std::fixed;
            file << "<?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"?>\n";
            file << "<svg \nxmlns=\"http://www.w3.org/2000/svg\"\nxmlns:inkscape=\"http://www.inkscape.org/namespaces/inkscape\"\n";
            file << "height=\"" << xSize << "\"\n";
            file << "width=\"" << ySize << "\"\n";
            file << "version=\"1.1\">\n<g\ninkscape:groupmode=\"layer\"\ninkscape:label=\"layer" << 1 << "\"\nid=\"" << 1 << "\">\n";
        }
    }

    SVGWriter::~SVGWriter()
    {
        file << "</g>\n</svg>\n";
        file.close();
    }

    void SVGWriter::writePoint(FPoint2D p, bool labelCoords, int size, Color color)
    {
        auto pt = transform(p);
        file << "<circle cx=\"" << pt.x << "\" cy=\"" << pt.y << "\" r=\"" << size << "\" stroke=\"" << toString(color).c_str() << "\" stroke-width=\"1\" fill=\"" << toString(color).c_str() << "\" />\n";

        if (labelCoords)
            file << "<text x=\"" << pt.x << "\" y=\"" << pt.y << "\" style=\"font-size: 10px;\" fill=\"black\">" << p.x << "," << p.y << "</text>\n";
    }

    void SVGWriter::writeLine(FPoint2D a, FPoint2D b, Color color, float stroke)
    {
        a = transform(a);
        b = transform(b);
        file << "<line x1=\"" << a.x << "\" y1=\"" << a.y << "\" x2=\"" << b.x << "\" y2=\"" << b.y << "\" style=\"stroke:" << toString(color).c_str() << ";stroke-width:" << stroke << "\" />\n";
    }
}