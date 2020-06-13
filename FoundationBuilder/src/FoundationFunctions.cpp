#include "../include/FoundationFunctions.h"

#include <vector>


namespace geom_utils
{

	Mesh createFoundation(const Mesh& inputModel, const FPoint3D::coord foundationHeight, const FPoint3D::coord inflateValue) {
		auto from2dTo3dTriangle = [](const Triangle2D& inputTriangle) { return Triangle3D(FPoint3D(inputTriangle.a.x, inputTriangle.a.y, 0), FPoint3D(inputTriangle.b.x, inputTriangle.b.y, 0), FPoint3D(inputTriangle.c.x, inputTriangle.c.y, 0)); };
		auto moveUp = [](std::vector<Triangle3D>& foundationPart, FPoint3D::coord by) {for_each(foundationPart.begin(), foundationPart.end(), [&](Triangle3D& input) {input.a.z += by; input.b.z += by; input.c.z += by; }); };

		std::vector<Triangle3D> facets(inputModel.getFacets());
		Polygon verticles;

		for (auto i : facets) {
			verticles.add(FPoint2D(i.a.x, i.a.y));
			verticles.add(FPoint2D(i.b.x, i.b.y));
			verticles.add(FPoint2D(i.c.x, i.c.y));
		}
		auto convex = verticles.convexHull();
		convex.simplify(convex.polygonLength() * 0.01);
		auto planeForFoundation = convex.inflate(inflateValue);

		auto polygonOfBottom = planeForFoundation.triangulate();

		std::vector<Triangle3D> foundationBottom;
		std::transform(polygonOfBottom.begin(), polygonOfBottom.end(), std::back_inserter(foundationBottom), from2dTo3dTriangle);
		auto foundationTop(foundationBottom);
		moveUp(foundationTop, foundationHeight);

		Mesh foundation;
		for_each(foundationBottom.begin(), foundationBottom.end(), [](Triangle3D& i) {i.reverse(); });
		for (int i = 0; i < foundationBottom.size(); ++i) {
			foundation.add(foundationTop[i]);

			if (i == 0) {
				foundation.add(Triangle3D(foundationTop[i].b, foundationBottom[i].b, foundationBottom[i].a));
				foundation.add(Triangle3D(foundationTop[i].b, foundationBottom[i].a, foundationTop[i].c));
			}
			foundation.add(Triangle3D(foundationTop[i].c, foundationBottom[i].a, foundationBottom[i].c));
			foundation.add(Triangle3D(foundationTop[i].c, foundationBottom[i].c, foundationTop[i].a));
			if (i == foundationBottom.size() - 1) {
				foundation.add(Triangle3D(foundationTop[i].a, foundationBottom[i].c, foundationBottom[i].b));
				foundation.add(Triangle3D(foundationTop[i].a, foundationBottom[i].b, foundationTop[i].b));
			}

			foundation.add(foundationBottom[i]);
		}
		return foundation;
	}


	Mesh integrateFoundationIntoModel(const Mesh& model, Mesh foundation)
	{
		Mesh resultFigure;
		if (!model.isEmpty() && !foundation.isEmpty())
		{
			auto zDiff = model.getAABB().pmin.z - foundation.getAABB().pmax.z;
			const float gap = 0.5;
			zDiff -= gap;
			foundation.translate(FPoint3D(0, 0, zDiff));
			resultFigure = model + foundation;
		}
		return resultFigure;
	}

}