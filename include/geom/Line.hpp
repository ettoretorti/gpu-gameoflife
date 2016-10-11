#pragma once

#include "definitions.hpp"

namespace geom {

class Line {
public:
	Line(Vec2 start, Vec2 end);
	Line(const Line& o);
	Line(Line&& o);
	Line& operator=(const Line& o);
	Line& operator=(Line&& o);

	Vec2 start() const;
	Vec2 end() const;
	double len() const;
	double lenSquared() const;
private:
	Vec2 a_;
	Vec2 b_;
};

}
