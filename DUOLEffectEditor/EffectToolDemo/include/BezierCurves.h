#pragma once

#include <DUOLMath/DUOLMath.h>

#include "../Common/Imgui/imgui.h"

class BezierCurves
{
public:
	BezierCurves();

	~BezierCurves() = default;
public:
	void Draw(const char* label, float scale_min, float scale_max, ImVec2 graph_size);

	void SetPoints(DUOLMath::Vector2 a, DUOLMath::Vector2 b, DUOLMath::Vector2 c, DUOLMath::Vector2 d);
private:
	DUOLMath::Vector2 _pointA;
	DUOLMath::Vector2 _pointB;
	DUOLMath::Vector2 _pointC;
	DUOLMath::Vector2 _pointD;
private:

	void Update();
};

