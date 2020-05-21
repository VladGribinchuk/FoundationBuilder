#include "../include/LinearAlgebra.h"
#include "../include/Triangle.h"

namespace geom_utils
{

	FPoint3D getNormal(const Triangle3D& triangel)
	{
		//compute vector
		FPoint3D normal = geom_utils::cross(triangel.a, triangel.b);

		//normalize vector
		float length = sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
		normal.x = normal.x / length;
		normal.y = normal.y / length;
		normal.z = normal.z / length;

		return normal;
	};


	FPoint2D getNormal(const Triangle2D& triangel)
	{
		//compute vector
		FPoint2D normal(triangel.b.y - triangel.a.y, -(triangel.b.x - triangel.a.x));

		//normalize vector
		float length = sqrt(normal.x * normal.x + normal.y * normal.y);
		normal.x = normal.x / length;
		normal.y = normal.y / length;

		return normal;
	};

}
