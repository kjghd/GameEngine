#pragma once

struct Location
{
	float x, y;

	void operator += (Location location) { x += location.x; y += location.y; };
};

struct Size
{
	float width, height;

	Size operator * (float f) { width * f; height * f; };

	Size operator + (float f) { width + f; height + f; };
	Size operator + (Size size) { width + size.width; height + size.height; };

	Size operator - (float f) { width - f; height - f; };
	Size operator - (Size size) { width - size.width; height - size.height; };

	void operator += (float f) { width += f; height += f; };
};
