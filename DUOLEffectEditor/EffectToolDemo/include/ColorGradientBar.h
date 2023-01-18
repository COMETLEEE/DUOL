/**

	@file      ColorGradientBar.h
	@brief     ColorGradientBar를 만들어 보자....!
	@details   ImGui에는 GrdientBar가 내장되어 있지 않다.
	@author    SinSeongHyeon
	@date      04.01.2023
	@copyright © SinSeongHyeon, 2022. All right reserved.

**/
#pragma once

#include "../Common/Imgui/imgui.h"
#include "Export\RenderingData.h"

class ColorGradientBar
{
public:
	ColorGradientBar();

	~ColorGradientBar() = default;
public:
	void Draw(ImVec2 pos, float height, float width, DUOLMath::Vector4* markerArr, int arrSize);

	void DrawAlpha(ImVec2 pos, float height, float width, DUOLMath::Vector4* markerArr, int arrSize);
private:
	ImVec2 _pos; // 그릴 위치의 좌표 값.

	float _height; // ColorGradientBar의 높이

	float _width; // ColorGradientBar의 너비

	DUOLMath::Vector4* _markerArr; // 마커의 배열.

	int _arrSize;

	int _selectMark;

	bool _isDraging;

	static int _id;

	std::string _strID;
private:
	void DrawColorGradientBar();

	void DrawColorGradientMarks();

	void MakerMouseDragingUpdate();

	void AddMarker();
	// 일반화를 하고 싶었지만, 렌더링 데이터를 넘기는 방식때문에 일반화하지 못했다...! 나중에 더 좋은 방식을 생각해보자.
	void DrawAlphaGradientBar();

	void DrawAlphaGradientMarks();

};

