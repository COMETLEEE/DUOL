#include "DUOLEditor/Util/Color.h"

namespace DUOLEditor
{
	const Color Color::Red = { 1.f, 0.f, 0.f };
	const Color Color::Green = { 0.f, 1.f, 0.f };
	const Color Color::Blue = { 0.f, 0.f, 1.f };
	const Color Color::White = { 1.f, 1.f, 1.f };
	const Color Color::Black = { 0.f, 0.f, 0.f };
	const Color Color::Gray = { 0.5f, 0.5f, 0.5f };
	const Color Color::Yellow = { 1.f, 1.f, 0.f };
	const Color Color::Cyan = { 0.f, 1.f, 1.f };
	const Color Color::Magenta = { 1.f, 0.f, 1.f };

	Color::Color(float r, float g, float b, float a) :
		r(r), g(g), b(b), a(a)
	{
	}

	bool Color::operator==(const Color& other) const
	{
		return this->r == other.r && this->g == other.g && this->b == other.b && this->a == other.a;
	}

	bool Color::operator!=(const Color& other) const
	{
		return !(*this == other);
	}
}