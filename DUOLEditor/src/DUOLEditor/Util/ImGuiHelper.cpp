#include "DUOLEditor/Util/ImGuiHelper.h"

#include "DUOLEditor/UI/Widgets/Drags/DragScalar.h"
#include "DUOLEditor/UI/Widgets/Layout/Container.h"
#include "DUOLEditor/UI/Widgets/Texts/TextColored.h"

#include <array>

const DUOLEditor::Color TitleColor = { 0.35f, 0.85f, 0.65f, 1.f };

namespace DUOLEditor
{
	ImVec4 ImGuiHelper::ToImVec4(const DUOLMath::Vector4& vector)
	{
		ImVec4 ret;

		memcpy(&ret, &vector, sizeof(ImVec4));

		return ret;
	}

	ImVec4 ImGuiHelper::ToImVec4(const Color& color)
	{
		ImVec4 ret;

		memcpy(&ret, &color, sizeof(ImVec4));

		return ret;
	}

	ImVec2 ImGuiHelper::ToImVec2(const DUOLMath::Vector2& vector)
	{
		ImVec2 ret;

		memcpy(&ret, &vector, sizeof(ImVec2));

		return ret;
	}

	DUOLMath::Vector4 ImGuiHelper::ToVector4(const ImVec4& imVec)
	{
		DUOLMath::Vector4 ret;

		memcpy(&ret, &imVec, sizeof(DUOLMath::Vector4));

		return ret;
	}

	DUOLMath::Vector2 ImGuiHelper::ToVector2(const ImVec2& imVec)
	{
		DUOLMath::Vector2 ret;

		memcpy(&ret, &imVec, sizeof(DUOLMath::Vector2));

		return ret;
	}

	Color ImGuiHelper::ToColor(const ImVec4& imVec)
	{
		DUOLEditor::Color ret;

		memcpy(&ret, &imVec, sizeof(Color));

		return ret;
	}

	void ImGuiHelper::DrawVector3(DUOLEditor::WidgetGroupBase* rootWidget, const DUOLCommon::tstring& name,
		std::function<DUOLMath::Vector3()> gatherer, std::function<void(DUOLMath::Vector3)> provider, float speed, float min, float max)
	{
		DrawTitle(rootWidget, name);

		auto vectorWidget = rootWidget->AddWidget<DragScalar<float, 3>>
			(min, max, 0.f, speed, TEXT(""), TEXT("%.3f"));

		vectorWidget->RegisterGatherer([gatherer]()
			{
				DUOLMath::Vector3 position = gatherer();

				return reinterpret_cast<const std::array<float, 3>&>(position);
			});

		vectorWidget->RegisterProvider([provider](std::array<float, 3>* value)
			{
				provider(reinterpret_cast<DUOLMath::Vector3&>(*value));
			});
	}

	void ImGuiHelper::DrawTitle(DUOLEditor::WidgetGroupBase* rootWidget, const DUOLCommon::tstring& name)
	{
		rootWidget->AddWidget<DUOLEditor::TextColored>(name, TitleColor);
	}
}