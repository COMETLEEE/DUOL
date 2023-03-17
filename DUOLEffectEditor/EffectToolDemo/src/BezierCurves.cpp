#include "BezierCurves.h"

#include <DUOLMath/DUOLMath.h>

BezierCurves::BezierCurves()
{
	_pointA = DUOLMath::Vector2(0, 0);
	_pointB = DUOLMath::Vector2(0, 1.0f);
	_pointC = DUOLMath::Vector2(1.0f, 1.0f);
	_pointD = DUOLMath::Vector2(1.0f, 1.0f);
}

void BezierCurves::Draw(const char* label, float scale_min, float scale_max,
	ImVec2 graph_size)
{

	float values[100];

	for (int i = 0; i < 100; i++)
	{
		float t = static_cast<float>(i) * 0.01f;
		float s = 1.0f - t;

		auto point = powf(s, 3) * _pointA
			+ 3 * powf(s, 2) * t * _pointB
			+ 3 * s * powf(t, 2) * _pointC
			+ powf(t, 3) * _pointD;

		values[i] = point.y;
	}

	ImGui::PlotLines(label, values, 100, 0, "",
		scale_min, scale_max, graph_size);

}

void BezierCurves::SetPoints(DUOLMath::Vector2 a, DUOLMath::Vector2 b, DUOLMath::Vector2 c, DUOLMath::Vector2 d)
{
	_pointA = a;
	_pointB = b;
	_pointC = c;
	_pointD = d;
}
