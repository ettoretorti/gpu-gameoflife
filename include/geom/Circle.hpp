#pragma once

#include "definitions.hpp"

namespace geom {

class Circle {
public:
	Circle(Vec2 center, float radius);
	Circle(const Circle& o);
	Circle(Circle&& o);
	Circle& operator=(const Circle& o);
	Circle& operator=(Circle&& o);

	Vec2 center() const;
	float radius() const;
	double area() const;
private:
	Vec2 c_;
	float r_;
};

}
