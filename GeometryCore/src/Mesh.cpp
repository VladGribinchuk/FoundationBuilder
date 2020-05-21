#include "../include/Mesh.h"

namespace geom_utils
{

	Mesh MeshHandler::read(const std::string& filepath)
	{
		std::string line;
		std::ifstream fromfile;

		fromfile.open(filepath, std::ios::in);
		if (fromfile.is_open())
			getline(fromfile, line);
		fromfile.close();

		if (line.find("solid") != std::string::npos)
			return MeshHandler::readASCII(filepath);
		else
			return MeshHandler::readBinary(filepath);
	}


	Mesh MeshHandler::readASCII(const std::string& filepath)
	{
		Mesh mesh;
		std::string line;
		std::ifstream fromfile;

		fromfile.open(filepath, std::ios::in);
		if (fromfile.is_open())
		{
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
		}
		fromfile.close();
		return mesh;
	}


	Mesh MeshHandler::readBinary(const std::string& filepath)
	{
		auto parseFloat = [](std::ifstream& s) {
			char f_buf[sizeof(float)];
			s.read(f_buf, 4);
			float* fptr = (float*)f_buf;
			return *fptr;
		};

		auto parsePoint = [&parseFloat](std::ifstream& s) {
			float x = parseFloat(s);
			float y = parseFloat(s);
			float z = parseFloat(s);
			return FPoint3D(x, y, z);
		};


		Mesh mesh;
		std::ifstream fromfile;

		fromfile.open(filepath, std::ios::in | std::ios::binary);
		if (fromfile.is_open())
		{
			char header_info[80] = "";
			fromfile.read(header_info, 80);
			mesh.setModelName(header_info);

			unsigned int numTris = 0;
			fromfile.read((char*)&numTris, 4);

			for (int i = 0; i < numTris; i++)
			{
				auto normal = parsePoint(fromfile);
				auto v1 = parsePoint(fromfile);
				auto v2 = parsePoint(fromfile);
				auto v3 = parsePoint(fromfile);
				mesh.add(Triangle3D(v1, v2, v3));				

				char dummy[2];
				fromfile.read(dummy, 2);
				
			}
		}
		fromfile.close();
		return mesh;
	}


	void MeshHandler::write(const std::string& filepath, const Mesh& mesh)
	{
		if (mesh.isEmpty()) return;

		std::ofstream infile;
		infile.open(filepath, std::ios::trunc);

		if (infile.is_open())
		{
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
		}
		infile.close();
	}
}