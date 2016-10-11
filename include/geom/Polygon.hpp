#pragma once

#include "definitions.hpp"
#include "Line.hpp"
#include "Circle.hpp"
#include "AABB.hpp"

#include <initializer_list>

namespace geom {

class Polygon {
public:
	Polygon(int nPoints, const Vec2* points);
	Polygon(std::initializer_list<Vec2> points);
	Polygon(const Circle& c, int sides);
	Polygon(const Polygon& o);
	Polygon(Polygon&& o);
	Polygon& operator=(const Polygon& o);
	Polygon& operator=(Polygon&& o);
	~Polygon();

	int size() const;
	Vec2 point(int i) const;
	Line edge(int i) const;
	double area() const;
	Vec2 centroid() const;
	Circle boundingCircle() const;
	AABB boundingBox() const;
private:
	Vec2* ps_;
	int n_;
};

}
