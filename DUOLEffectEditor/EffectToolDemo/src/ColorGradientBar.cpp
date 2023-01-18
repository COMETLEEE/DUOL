#include "ColorGradientBar.h"
#include "KeyBoard.h"
#include <algorithm>

bool cmp(const DUOLMath::Vector4& a, const DUOLMath::Vector4& b)
{
	return a.w < b.w;
};

int ColorGradientBar::_id = 0;
ColorGradientBar::ColorGradientBar() : _selectMark(-1), _isDraging(false), _arrSize(0), _markerArr(nullptr)
{
	_strID = std::to_string(_id);
	_id++;
}

void ColorGradientBar::Draw(ImVec2 pos, float height, float width, DUOLMath::Vector4* markerArr, int arrSize)
{
	_pos = pos; // 그릴 위치의 좌표 값.

	_height = height; // ColorGradientBar의 높이

	_width = width; // ColorGradientBar의 너비

	if (_markerArr != markerArr)
	{
		_markerArr = markerArr; // 마커의 배열.

		_arrSize = arrSize;

		_selectMark = -1;

		_isDraging = false;
	}

	DrawColorGradientBar();

	ImGui::SetCursorScreenPos(ImVec2(_pos.x, _pos.y));
	ImGui::InvisibleButton((std::string("GrdientBar##Color") + _strID).c_str(), ImVec2(_width, _height - 5));

	if (ImGui::IsItemHovered())
	{
		if (ImGui::IsMouseClicked(0))
		{
			AddMarker();
		}
	}

	DrawColorGradientMarks();

	MakerMouseDragingUpdate();

}

void ColorGradientBar::DrawAlpha(ImVec2 pos, float height, float width, DUOLMath::Vector4* markerArr, int arrSize)
{
	_pos = pos; // 그릴 위치의 좌표 값.

	_height = height; // ColorGradientBar의 높이

	_width = width; // ColorGradientBar의 너비

	if (_markerArr != markerArr)
	{
		_markerArr = markerArr; // 마커의 배열.

		_arrSize = arrSize;

		_selectMark = -1;

		_isDraging = false;
	}

	DrawAlphaGradientBar();

	ImGui::SetCursorScreenPos(ImVec2(_pos.x, _pos.y));

	ImGui::InvisibleButton((std::string("GrdientBar##Alpha") + _strID).c_str(), ImVec2(_width, _height - 5));

	if (ImGui::IsItemHovered())
	{
		if (ImGui::IsMouseClicked(0))
		{
			AddMarker();
		}
	}

	DrawAlphaGradientMarks();

	MakerMouseDragingUpdate();
}

void ColorGradientBar::DrawColorGradientBar()
{
	ImVec4 colorA = { 1,1,1,1 };
	ImVec4 colorB = { 1,1,1,1 };
	float prevX = _pos.x;
	float barBottom = _pos.y + _height;

	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	draw_list->AddRectFilled(ImVec2(_pos.x - 2, _pos.y - 2),
		ImVec2(_pos.x + _width + 2, barBottom + 2),
		IM_COL32(100, 100, 100, 255));

	ImU32 colorAU32 = 0;
	ImU32 colorBU32 = 0;

	for (int i = 0; i < _arrSize; i++)
	{
		float from = prevX;
		float to = prevX = _pos.x + _markerArr[i].w * _width;

		if (i == 0)
		{
			colorA.x = _markerArr[i].x;
			colorA.y = _markerArr[i].y;
			colorA.z = _markerArr[i].z;
		}
		else
		{
			colorA.x = _markerArr[i - 1].x;
			colorA.y = _markerArr[i - 1].y;
			colorA.z = _markerArr[i - 1].z;
		}

		colorB.x = _markerArr[i].x;
		colorB.y = _markerArr[i].y;
		colorB.z = _markerArr[i].z;

		colorAU32 = ImGui::ColorConvertFloat4ToU32(colorA);
		colorBU32 = ImGui::ColorConvertFloat4ToU32(colorB);

		draw_list->AddRectFilledMultiColor(ImVec2(from, _pos.y),
			ImVec2(to, barBottom),
			colorAU32, colorBU32, colorBU32, colorAU32);

	}
	float from = _pos.x + _markerArr[_arrSize - 1].w * _width;
	float to = prevX = _pos.x + _width;

	colorB.x = _markerArr[_arrSize - 1].x;
	colorB.y = _markerArr[_arrSize - 1].y;
	colorB.z = _markerArr[_arrSize - 1].z;

	colorAU32 = ImGui::ColorConvertFloat4ToU32(colorB);
	colorBU32 = ImGui::ColorConvertFloat4ToU32(colorB);

	draw_list->AddRectFilledMultiColor(ImVec2(from, _pos.y),
		ImVec2(to, barBottom),
		colorAU32, colorBU32, colorBU32, colorAU32);

	ImGui::SetCursorScreenPos(ImVec2(_pos.x, _pos.y + _height + 10.0f));
}

void ColorGradientBar::DrawColorGradientMarks()
{
	ImVec4 color = { 1,1,1,1 };

	float barBottom = _pos.y + _height;

	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	ImU32 colorU32 = 0;

	for (int i = 0; i < _arrSize; i++)
	{
		if (_markerArr[i].w >= 1.0f)
			continue;

		float to = _pos.x + _markerArr[i].w * _width;


		color.x = _markerArr[i].x;
		color.y = _markerArr[i].y;
		color.z = _markerArr[i].z;

		colorU32 = ImGui::ColorConvertFloat4ToU32(color);

		draw_list->AddTriangleFilled(ImVec2(to, _pos.y + (_height - 6)),
			ImVec2(to - 6, barBottom),
			ImVec2(to + 6, barBottom), IM_COL32(100, 100, 100, 255));

		draw_list->AddRectFilled(ImVec2(to - 6, barBottom),
			ImVec2(to + 6, _pos.y + (_height + 12)),
			IM_COL32(100, 100, 100, 255), 1.0f, 1.0f);

		draw_list->AddRectFilled(ImVec2(to - 5, _pos.y + (_height + 1)),
			ImVec2(to + 5, _pos.y + (_height + 11)),
			IM_COL32(0, 0, 0, 255), 1.0f, 1.0f);

		if (_selectMark == i)
		{
			draw_list->AddTriangleFilled(ImVec2(to, _pos.y + (_height - 3)),
				ImVec2(to - 4, barBottom + 1),
				ImVec2(to + 4, barBottom + 1), IM_COL32(0, 255, 0, 255));

			draw_list->AddRect(ImVec2(to - 5, _pos.y + (_height + 1)),
				ImVec2(to + 5, _pos.y + (_height + 11)),
				IM_COL32(0, 255, 0, 255), 1.0f, 1.0f);
		}

		draw_list->AddRectFilledMultiColor(ImVec2(to - 3, _pos.y + (_height + 3)),
			ImVec2(to + 3, _pos.y + (_height + 9)),
			colorU32, colorU32, colorU32, colorU32);

		ImGui::SetCursorScreenPos(ImVec2(to - 6, barBottom));
		ImGui::InvisibleButton((std::string("Mark") + _strID).c_str(), ImVec2(12, 12));

		if (ImGui::IsItemHovered())
		{
			if (ImGui::IsMouseClicked(0))
			{
				_selectMark = i;
				_isDraging = true;
			}
			if (ImGui::IsMouseClicked(1))
			{
				_selectMark = i;
				_markerArr[i].w = 0.999999f;
				std::sort(_markerArr, _markerArr + 8, cmp);
			}
		}
	}

	if (ImGui::IsMouseDown)
		ImGui::SetCursorScreenPos(ImVec2(_pos.x, _pos.y + _height + 20.0f));

	ImGui::SetNextItemWidth(_width);

	if (_selectMark != -1)
		ImGui::ColorPicker3((std::string("SelectMakrColor") + _strID).c_str(), reinterpret_cast<float*>(&_markerArr[_selectMark]));
}
void ColorGradientBar::MakerMouseDragingUpdate()
{
	if (Muscle::KeyBoard::Get()->KeyUp(VK_LBUTTON))
		_isDraging = false;

	if (ImGui::IsMouseDragging(0) && _isDraging)
	{
		auto cmp = [](const DUOLMath::Vector4& a, const DUOLMath::Vector4& b)->bool {
			return a.w > b.w;
		};

		for (int i = _selectMark - 1; i < _selectMark + 1; i++)
		{
			if (i < 0) continue;
			if (i == _arrSize - 1) break;

			if (cmp(_markerArr[i], _markerArr[i + 1]))
			{
				std::swap(_markerArr[i], _markerArr[i + 1]);

				if (_selectMark != i + 1)
					_selectMark = i + 1;
				else
					_selectMark = i;
				break;
			}
		}

		float increment = ImGui::GetIO().MouseDelta.x / _width;
		bool insideZone = ImGui::GetIO().MousePos.x > _pos.x;

		if (increment != 0.0f && insideZone)
		{
			_markerArr[_selectMark].w += increment;
		}
		if (_markerArr[_selectMark].w > 1.0f)
		{
			_markerArr[_selectMark].x = 1.0f;
			_markerArr[_selectMark].y = 1.0f;
			_markerArr[_selectMark].z = 1.0f;
			_markerArr[_selectMark].w = 1.0f;
			_selectMark = -1;
			_isDraging = false;
		}
	}
}
void ColorGradientBar::AddMarker()
{
	for (int i = 0; i < _arrSize; i++)
	{
		if (_markerArr[i].w >= 1.0f)
		{
			_markerArr[i] = DUOLMath::Vector4(1.0f, 1.0f, 1.0f, 0.0f);


			std::sort(_markerArr, _markerArr + 8, cmp);
			break;
		}

	}
}
void ColorGradientBar::DrawAlphaGradientBar()
{
	ImVec4 colorA = { 1,1,1,1 };
	ImVec4 colorB = { 1,1,1,1 };
	float prevX = _pos.x;
	float barBottom = _pos.y + _height;

	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	draw_list->AddRectFilled(ImVec2(_pos.x - 2, _pos.y - 2),
		ImVec2(_pos.x + _width + 2, barBottom + 2),
		IM_COL32(0, 0, 0, 255));

	ImU32 colorAU32 = 0;
	ImU32 colorBU32 = 0;

	for (int i = 0; i < _arrSize; i++)
	{

		float from = prevX;
		float to = prevX = _pos.x + _markerArr[i].w * _width;

		if (i == 0)
			colorA.w = _markerArr[i].x;
		else
		{
			colorA.w = _markerArr[i - 1].x;
		}

		colorB.w = _markerArr[i].x;

		colorAU32 = ImGui::ColorConvertFloat4ToU32(colorA);
		colorBU32 = ImGui::ColorConvertFloat4ToU32(colorB);

		draw_list->AddRectFilledMultiColor(ImVec2(from, _pos.y),
			ImVec2(to, barBottom),
			colorAU32, colorBU32, colorBU32, colorAU32);
	}
	float from = _pos.x + _markerArr[_arrSize - 1].w * _width;
	float to = prevX = _pos.x + _width;

	colorB.w = _markerArr[_arrSize - 1].x;

	colorAU32 = ImGui::ColorConvertFloat4ToU32(colorB);
	colorBU32 = ImGui::ColorConvertFloat4ToU32(colorB);

	draw_list->AddRectFilledMultiColor(ImVec2(from, _pos.y),
		ImVec2(to, barBottom),
		colorAU32, colorBU32, colorBU32, colorAU32);

	ImGui::SetCursorScreenPos(ImVec2(_pos.x, _pos.y + _height + 10.0f));
}

void ColorGradientBar::DrawAlphaGradientMarks()
{
	ImVec4 color = { 1,1,1,1 };

	float barBottom = _pos.y + _height;

	ImDrawList* draw_list = ImGui::GetWindowDrawList();

	ImU32 colorU32 = 0;

	for (int i = 0; i < _arrSize; i++)
	{
		if (_markerArr[i].w >= 1.0f)
			continue;

		float to = _pos.x + _markerArr[i].w * _width;


		color.w = _markerArr[i].x;

		colorU32 = ImGui::ColorConvertFloat4ToU32(color);

		draw_list->AddTriangleFilled(ImVec2(to, _pos.y + (_height - 6)),
			ImVec2(to - 6, barBottom),
			ImVec2(to + 6, barBottom), IM_COL32(100, 100, 100, 255));

		draw_list->AddRectFilled(ImVec2(to - 6, barBottom),
			ImVec2(to + 6, _pos.y + (_height + 12)),
			IM_COL32(100, 100, 100, 255), 1.0f, 1.0f);

		draw_list->AddRectFilled(ImVec2(to - 5, _pos.y + (_height + 1)),
			ImVec2(to + 5, _pos.y + (_height + 11)),
			IM_COL32(0, 0, 0, 255), 1.0f, 1.0f);

		if (_selectMark == i)
		{
			draw_list->AddTriangleFilled(ImVec2(to, _pos.y + (_height - 3)),
				ImVec2(to - 4, barBottom + 1),
				ImVec2(to + 4, barBottom + 1), IM_COL32(0, 255, 0, 255));

			draw_list->AddRect(ImVec2(to - 5, _pos.y + (_height + 1)),
				ImVec2(to + 5, _pos.y + (_height + 11)),
				IM_COL32(0, 255, 0, 255), 1.0f, 1.0f);
		}

		draw_list->AddRectFilledMultiColor(ImVec2(to - 3, _pos.y + (_height + 3)),
			ImVec2(to + 3, _pos.y + (_height + 9)),
			colorU32, colorU32, colorU32, colorU32);

		ImGui::SetCursorScreenPos(ImVec2(to - 6, barBottom));


		ImGui::InvisibleButton((std::string("AlphaMark") + _strID).c_str(), ImVec2(12, 12));

		if (ImGui::IsItemHovered())
		{
			if (ImGui::IsMouseClicked(0))
			{
				_selectMark = i;
				_isDraging = true;
			}
			if (ImGui::IsMouseClicked(1))
			{
				_selectMark = i;
				_markerArr[i].w = 0.999999f;
				std::sort(_markerArr, _markerArr + 8, cmp);
			}
		}
	}

	if (ImGui::IsMouseDown)
		ImGui::SetCursorScreenPos(ImVec2(_pos.x, _pos.y + _height + 20.0f));

	ImGui::SetNextItemWidth(_width);

	if (_selectMark != -1)
		ImGui::DragFloat((std::string("SelectMakrAlpha") + _strID).c_str(), reinterpret_cast<float*>(&_markerArr[_selectMark].x), 0.01, 0.0f, 1.0f);
}