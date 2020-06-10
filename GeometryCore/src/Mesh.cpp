#include "../include/Mesh.h"
#include "../include/Polygon.h"

namespace geom_utils
{

	bool MeshHandler::read(const std::string& filepath, Mesh& mesh)
	{
		auto isAscii = [](std::string filepath) {
			std::string line;
			std::ifstream fromfile(filepath, std::ios::in);
			if (fromfile.is_open())
				while (getline(fromfile, line))
				{
					if (std::all_of(line.begin(), line.end(), isspace))	continue;
					if (line.find("solid") != std::string::npos) return true;
					else return false;
				}
			fromfile.close();
			return false;
		};

		if (isAscii(filepath)) {
			if (!readASCII(filepath, mesh) && !readBinary(filepath, mesh))
				return false;
		}
		else
			if (!readBinary(filepath, mesh))
				return false;
		return true;
	}


	bool MeshHandler::readASCII(const std::string& filepath, Mesh& mesh)
	{
		std::string line;
		std::ifstream fromfile;

		fromfile.open(filepath, std::ios::in);
		if (!fromfile.is_open()) {
			fromfile.close();
			return false;
		}

		fromfile >> line;
		fromfile >> line; //comment
		mesh.setModelName(line);

		while (fromfile >> line)
			if (line.find("vertex") != std::string::npos)
			{
				Triangle3D tri;
				fromfile >> tri.a.x >> tri.a.y >> tri.a.z;		   //vertex0 x, y, z
				fromfile >> line >> tri.b.x >> tri.b.y >> tri.b.z; //vertex1 x, y, z
				fromfile >> line >> tri.c.x >> tri.c.y >> tri.c.z; //vertex2 x, y, z
				mesh.add(tri);
			}

		fromfile.close();
		return true;
	}


	bool MeshHandler::readBinary(const std::string& filepath, Mesh& mesh)
	{
		std::ifstream fromfile;
		fromfile.open(filepath, std::ios::in | std::ios::binary);
		if (!fromfile.is_open()) {
			fromfile.close();
			return false;
		}

		fromfile.seekg(0, fromfile.end);
		size_t length = fromfile.tellg();
		fromfile.seekg(0, fromfile.beg);
		size_t actualFacetCount = (length - SIZE_OF_HEADER - sizeof(std::uint32_t)) / SIZE_OF_FACET;

		char header_info[SIZE_OF_HEADER] = "";
		fromfile.read(header_info, SIZE_OF_HEADER);
		mesh.setModelName(header_info);

		unsigned int expectedFacetCount = 0;
		fromfile.read((char*)&expectedFacetCount, 4);

		if (actualFacetCount != expectedFacetCount) return false;

		char buffer[SIZE_OF_FACET];
		while (actualFacetCount-- > 0 && fromfile.read(buffer, SIZE_OF_FACET))
		{
			float* ptr = ((float*)buffer);
			FPoint3D normal(*(ptr + 0), *(ptr + 1), *(ptr + 2));
			ptr += 3;
			Triangle3D triangle;
			triangle.a = FPoint3D(*(ptr + 0), *(ptr + 1), *(ptr + 2));
			triangle.b = FPoint3D(*(ptr + 3), *(ptr + 4), *(ptr + 5));
			triangle.c = FPoint3D(*(ptr + 6), *(ptr + 7), *(ptr + 8));
			mesh.add(triangle);
		}

		fromfile.close();
		return true;
	}


	bool MeshHandler::writeASCII(const std::string& filepath, const Mesh& mesh)
	{
		if (mesh.isEmpty()) return false;

		std::ofstream infile;
		infile.open(filepath, std::ios::out | std::ios::trunc);
		if (!infile.is_open()) {
			infile.close();
			return false;
		}

		infile << "solid " << mesh.getModelName();
		for (int i = 0; i < mesh.size(); i++)
		{
			FPoint3D normal = mesh[i].getNormal();
			infile << "\n facet normal " << normal.x << " " << normal.y << " " << normal.z << "\n  outer loop";

			infile << "\n  vertex " << mesh[i].a.x << " " << mesh[i].a.y << " " << mesh[i].a.z;
			infile << "\n  vertex " << mesh[i].b.x << " " << mesh[i].b.y << " " << mesh[i].b.z;
			infile << "\n  vertex " << mesh[i].c.x << " " << mesh[i].c.y << " " << mesh[i].c.z;

			infile << "\n  endloop\n endfacet\n";
		}
		infile << "\nendsolid " << mesh.getModelName();
		infile.close();
		return true;
	}


	bool MeshHandler::writeBinary(const std::string& filepath, const Mesh& mesh)
	{
		if (mesh.isEmpty()) return false;

		std::ofstream infile;
		infile.open(filepath, std::ios::out | std::ios::binary | std::ios::trunc);
		if (!infile.is_open()) {
			infile.close();
			return false;
		}

		infile.write(mesh.getModelName().data(), SIZE_OF_HEADER);
		uint32_t facetCount = (uint32_t)mesh.getFacets().size();
		infile.write(reinterpret_cast<const char*>(&facetCount), sizeof(facetCount));

		char buffer[SIZE_OF_FACET] = { 0 };
		for (const auto& tri : mesh.getFacets())
		{
			float* ptr = ((float*)buffer);
			FPoint3D normal = tri.getNormal();

			*(ptr + 0) = normal.x;
			*(ptr + 1) = normal.y;
			*(ptr + 2) = normal.z;
			ptr += 3;
			*(ptr + 0) = tri.a.x; *(ptr + 1) = tri.a.y; *(ptr + 2) = tri.a.z;
			*(ptr + 3) = tri.b.x; *(ptr + 4) = tri.b.y; *(ptr + 5) = tri.b.z;
			*(ptr + 6) = tri.c.x; *(ptr + 7) = tri.c.y; *(ptr + 8) = tri.c.z;
			infile.write(buffer, sizeof(buffer));
		}
		infile.close();
		return true;
	}

	Mesh createFoundation(const Mesh& inputModel, const FPoint3D::coord foundation_height) {
		auto from_2d_to_3d_triangle = [](const Triangle2D& input_triangle) { return Triangle3D(FPoint3D(input_triangle.a.x, input_triangle.a.y, 0), FPoint3D(input_triangle.b.x, input_triangle.b.y, 0), FPoint3D(input_triangle.c.x, input_triangle.c.y, 0)); };
		auto move_up = [](std::vector<Triangle3D>& foundation_part, FPoint3D::coord by) {for_each(foundation_part.begin(), foundation_part.end(), [&](Triangle3D& input) {input.a.z += by; input.b.z += by; input.c.z += by; }); };

		std::vector<Triangle3D> facets(inputModel.getFacets());
		Polygon verticles;

		for (auto i : facets) {
			verticles.add(FPoint2D(i.a.x, i.a.y));
			verticles.add(FPoint2D(i.b.x, i.b.y));
			verticles.add(FPoint2D(i.c.x, i.c.y));
		}
		auto convex = verticles.convexHull();
		convex.simplify(convex.polygonLength() * 0.01);
		auto plane_for_foundation = convex.inflate(5.00);

		auto polygon_of_bottom = plane_for_foundation.triangulate();

		std::vector<Triangle3D> foundation_bottom;
		std::for_each(polygon_of_bottom.begin(), polygon_of_bottom.end(), [&](const Triangle2D& input_triangle) {foundation_bottom.push_back(from_2d_to_3d_triangle(input_triangle)); });
		auto foundation_top(foundation_bottom);
		move_up(foundation_top, foundation_height);

		Mesh foundation;
		for (int i = 0; i < foundation_bottom.size(); ++i) {
			foundation.add(foundation_top[i]);

			if (i == 0) {
				foundation.add(Triangle3D(foundation_top[i].b, foundation_bottom[i].b, foundation_bottom[i].c));
				foundation.add(Triangle3D(foundation_top[i].b, foundation_bottom[i].c, foundation_top[i].c));
			}
			foundation.add(Triangle3D(foundation_top[i].c, foundation_bottom[i].c, foundation_bottom[i].a));
			foundation.add(Triangle3D(foundation_top[i].c, foundation_bottom[i].a, foundation_top[i].a));
			if (i == foundation_bottom.size() - 1) {
				foundation.add(Triangle3D(foundation_top[i].a, foundation_bottom[i].a, foundation_bottom[i].b));
				foundation.add(Triangle3D(foundation_top[i].a, foundation_bottom[i].b, foundation_top[i].b));
			}

			foundation.add(foundation_bottom[i]);
		}
		return foundation;
	}
}