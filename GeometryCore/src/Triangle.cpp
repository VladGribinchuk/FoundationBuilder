#include "../include/LinearAlgebra.h"
#include "../include/Triangle.h"

namespace geom_utils
{

	FPoint3D Triangle3D::getNormal() const
	{
		//calculate vector
		FPoint3D vec1(this->b - this->a);
		FPoint3D vec2(this->c - this->a);

		//compute vector
		FPoint3D normal = geom_utils::cross(vec1, vec2);

		//normalize vector
		float length = sqrt(normal.x * normal.x + normal.y * normal.y + normal.z * normal.z);
		normal.x = normal.x / length;
		normal.y = normal.y / length;
		normal.z = normal.z / length;

		return normal;
	};

}
