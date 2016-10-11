#pragma once

#include "definitions.hpp"

namespace geom {

class AABB {
public:
	AABB(Vec2 center, Vec2 extents);
	AABB(const AABB& o);
	AABB(AABB&& o);
	AABB& operator=(const AABB& o);
	AABB& operator=(AABB&& o);

	Vec2 center() const;
	Vec2 extents() const;
	double area() const;
private:
	Vec2 c_;
	Vec2 e_;
};

}
