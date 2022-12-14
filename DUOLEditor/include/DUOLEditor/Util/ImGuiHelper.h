/**

    @file      ImGuiHelper.h
    @brief     ImGui를 사용하는데 도움을 주는 헬퍼 클래스
    @details   ~
    @author    COMETLEE
    @date      16.12.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include "DUOLEditor/Util/Color.h"
#include "DUOLCommon/ImGui/imgui.h"
#include "DUOLMath/DUOLMath.h"

namespace DUOLEditor
{
	/**
	 * \brief ImGui 데이터 타입과 DUOLEngine에서 사용하는 데이터 타입 간의 컨버팅 헬퍼
	 */
	class ImGuiHelper
	{
	public:
		static ImVec4 ToImVec4(const DUOLMath::Vector4& vector);

		static ImVec4 ToImVec4(const Color& color);

		static ImVec2 ToImVec2(const DUOLMath::Vector2& vector);

		static DUOLMath::Vector4 ToVector4(const ImVec4& imVec);

		static DUOLMath::Vector2 ToVector2(const ImVec2& imVec);

		static Color ToColor(const ImVec4& imVec);
	};
}
