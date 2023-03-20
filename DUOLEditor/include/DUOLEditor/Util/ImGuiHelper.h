/**

    @file      ImGuiHelper.h
    @brief     ImGui를 사용하는데 도움을 주는 헬퍼 클래스
    @details   ~
    @author    COMETLEE
    @date      16.12.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include "DUOLCommon/ImGui/imgui.h"
#include "DUOLCommon/StringHelper.h"

#include "DUOLEditor/Util/Color.h"
#include "DUOLMath/DUOLMath.h"

namespace DUOLEditor
{
	class WidgetGroupBase;

	/**
	 * \brief ImGui 와 DUOLEngine 전반에서 사용하는 데이터 타입과
	 * Draw API의 조합을 담당하는 헬퍼 클래스
	 */
	class ImGuiHelper
	{
	public:
		static DUOLEditor::Color TITLE_COLOR;

		static DUOLEditor::Color SELECTED_COLOR;

	public:
		static ImVec4 ToImVec4(const DUOLMath::Vector4& vector);

		static ImVec4 ToImVec4(const Color& color);

		static ImVec2 ToImVec2(const DUOLMath::Vector2& vector);

		static DUOLMath::Vector4 ToVector4(const ImVec4& imVec);

		static DUOLMath::Vector2 ToVector2(const ImVec2& imVec);

		static Color ToColor(const ImVec4& imVec);

		static void DrawBool(DUOLEditor::WidgetGroupBase* rootWidget, const DUOLCommon::tstring& name,
			std::function<bool(void)> gatherer, std::function<void(bool)> provider);

		static void DrawFloat(DUOLEditor::WidgetGroupBase* rootWidget, const DUOLCommon::tstring& name,
			std::function<float(void)> gatherer, std::function<void(float)> provider, float speed, float min, float max);

		static void DrawFloat2(DUOLEditor::WidgetGroupBase* rootWidget, const DUOLCommon::tstring& name,
			std::function<DUOLMath::Vector2(void)> gatherer, std::function<void(DUOLMath::Vector2)> provider, float speed, float min, float max);

		static void DrawFloat3(DUOLEditor::WidgetGroupBase* rootWidget, const DUOLCommon::tstring& name, 
			std::function<DUOLMath::Vector3(void)> gatherer, std::function<void(DUOLMath::Vector3)> provider, float speed, float min, float max);

		static void DrawFloat4(DUOLEditor::WidgetGroupBase* rootWidget, const DUOLCommon::tstring& name,
			std::function<DUOLMath::Vector4(void)> gatherer, std::function<void(DUOLMath::Vector4)> provider, float speed, float min, float max);

		static void DrawInt(DUOLEditor::WidgetGroupBase* rootWidget, const DUOLCommon::tstring& name,
			std::function<int(void)> gatherer, std::function<void(int)> provider, int speed, int min, int max);

		static void DrawString(DUOLEditor::WidgetGroupBase* rootWidget, const DUOLCommon::tstring& name,
			std::function<DUOLCommon::tstring(void)> gatherer, std::function<void(DUOLCommon::tstring)> provider);

		static void DrawStringNoInput(DUOLEditor::WidgetGroupBase* rootWidget, const DUOLCommon::tstring& name,
			std::function<DUOLCommon::tstring(void)> gatherer, std::function<void(DUOLCommon::tstring)> provider, std::function<void()> callbackAfter = nullptr);

		static void DrawColor3(DUOLEditor::WidgetGroupBase* rootWidget, const DUOLCommon::tstring& name,
			std::function<DUOLMath::Vector3(void)> gatherer, std::function<void(DUOLMath::Vector3)> provider);

		static void DrawTitle(DUOLEditor::WidgetGroupBase* rootWidget, const DUOLCommon::tstring& name);
	};
}