#include "BezierCurves.h"

#include <DUOLMath/DUOLMath.h>

BezierCurves::BezierCurves()
{
	_pointA = DUOLMath::Vector2(0, 0);
	_pointB = DUOLMath::Vector2(0.333f, 1.0f);
	_pointC = DUOLMath::Vector2(0.666f, 1.0f);
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

	ImDrawList* draw_list = ImGui::GetWindowDrawList();


	auto pos = ImGui::GetCursorScreenPos();


	ImGui::PlotLines(label, values, 100, 0, "",
		scale_min, scale_max, graph_size);

	pos.y += 80.0f;

	ImVec2 a, b, c, d;

	a.x = pos.x + _pointA.x * graph_size.x;
	a.y = pos.y - _pointA.y * graph_size.y;

	draw_list->AddCircleFilled(a, 5, IM_COL32(255, 100, 100, 255));

	b.x = pos.x + _pointB.x * graph_size.x;
	b.y = pos.y - _pointB.y * graph_size.y;
	draw_list->AddCircleFilled(b, 5, IM_COL32(255, 100, 100, 255));

	c.x = pos.x + _pointC.x * graph_size.x;
	c.y = pos.y - _pointC.y * graph_size.y;
	draw_list->AddCircleFilled(c, 5, IM_COL32(255, 100, 100, 255));

	d.x = pos.x + _pointD.x * graph_size.x;
	d.y = pos.y - _pointD.y * graph_size.y;
	draw_list->AddCircleFilled(d, 5, IM_COL32(255, 100, 100, 255));

	draw_list->AddBezierCurve(a, b, c, d, IM_COL32(255, 100, 100, 255), 1);



}

void BezierCurves::SetPoints(DUOLMath::Vector2 a, DUOLMath::Vector2 b, DUOLMath::Vector2 c, DUOLMath::Vector2 d)
{
	_pointA = a;
	_pointB = b;
	_pointC = c;
	_pointD = d;
}
