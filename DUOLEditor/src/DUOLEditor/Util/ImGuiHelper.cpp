#include "DUOLEditor/Util/ImGuiHelper.h"

#include "DUOLEditor/UI/Widgets/Drags/DragScalar.h"
#include "DUOLEditor/UI/Widgets/Layout/Container.h"
#include "DUOLEditor/UI/Widgets/Texts/TextColored.h"
#include "DUOLEditor/UI/Widgets/Selections/CheckBox.h"

#include <array>

#include "DUOLEditor/UI/Widgets/Edits/ColorEdit.h"
#include "DUOLEditor/UI/Widgets/InputFields/InputText.h"
#include "DUOLEditor/UI/Widgets/Texts/TextClickable.h"

// 형광 초록색
DUOLEditor::Color DUOLEditor::ImGuiHelper::TITLE_COLOR = { 0.35f, 0.85f, 0.65f, 1.f };

// 주황색
DUOLEditor::Color DUOLEditor::ImGuiHelper::SELECTED_COLOR = { 1.f, 0.5f, 0.f, 1.f };

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

	void ImGuiHelper::DrawBool(DUOLEditor::WidgetGroupBase* rootWidget, const DUOLCommon::tstring& name,
		std::function<bool()> gatherer, std::function<void(bool)> provider)
	{
		DrawTitle(rootWidget, name);

		auto boolWidget = rootWidget->AddWidget<DUOLEditor::CheckBox>
			(true, TEXT(""));

		boolWidget->RegisterGatherer([gatherer]()
			{
				bool value = gatherer();

				return value;
			});

		boolWidget->RegisterProvider([provider](bool* value)
			{
				provider(*value);
			});
	}

	void ImGuiHelper::DrawFloat(DUOLEditor::WidgetGroupBase* rootWidget, const DUOLCommon::tstring& name,
		std::function<float()> gatherer, std::function<void(float)> provider, float speed, float min, float max)
	{
		DrawTitle(rootWidget, name);

		auto floatWidget = rootWidget->AddWidget<DragScalar<float, 1>>
			(min, max, 0.f, speed, TEXT(""), TEXT("%.3f"));

		floatWidget->RegisterGatherer([gatherer]()
			{
				float value = gatherer();

				return reinterpret_cast<const std::array<float, 1>&>(value);
			});

		floatWidget->RegisterProvider([provider](std::array<float, 1>* value)
			{
				provider(reinterpret_cast<float&>(*value));
			});
	}

	void ImGuiHelper::DrawFloat2(DUOLEditor::WidgetGroupBase* rootWidget, const DUOLCommon::tstring& name,
		std::function<DUOLMath::Vector2()> gatherer, std::function<void(DUOLMath::Vector2)> provider, float speed,
		float min, float max)
	{
		DrawTitle(rootWidget, name);

		auto floatWidget = rootWidget->AddWidget<DragScalar<float, 2>>
			(min, max, 0.f, speed, TEXT(""), TEXT("%.3f"));

		floatWidget->RegisterGatherer([gatherer]()
			{
				DUOLMath::Vector2 value = gatherer();

				return reinterpret_cast<const std::array<float, 2>&>(value);
			});

		floatWidget->RegisterProvider([provider](std::array<float, 2>* value)
			{
				provider(reinterpret_cast<DUOLMath::Vector2&>(*value));
			});
	}

	void ImGuiHelper::DrawFloat3(DUOLEditor::WidgetGroupBase* rootWidget, const DUOLCommon::tstring& name,
		std::function<DUOLMath::Vector3()> gatherer, std::function<void(DUOLMath::Vector3)> provider, float speed, float min, float max)
	{
		DrawTitle(rootWidget, name);

		auto vectorWidget = rootWidget->AddWidget<DragScalar<float, 3>>
			(min, max, 0.f, speed, TEXT(""), TEXT("%.3f"));

		vectorWidget->RegisterGatherer([gatherer]()
			{
				DUOLMath::Vector3 vector = gatherer();

				return reinterpret_cast<const std::array<float, 3>&>(vector);
			});

		vectorWidget->RegisterProvider([provider](std::array<float, 3>* value)
			{
				provider(reinterpret_cast<DUOLMath::Vector3&>(*value));
			});
	}

	void ImGuiHelper::DrawFloat4(DUOLEditor::WidgetGroupBase* rootWidget, const DUOLCommon::tstring& name,
		std::function<DUOLMath::Vector4()> gatherer, std::function<void(DUOLMath::Vector4)> provider, float speed,
		float min, float max)
	{
		DrawTitle(rootWidget, name);

		auto vectorWidget = rootWidget->AddWidget<DragScalar<float, 4>>
			(min, max, 0.f, speed, TEXT(""), TEXT("%.3f"));

		vectorWidget->RegisterGatherer([gatherer]()
			{
				DUOLMath::Vector4 vector = gatherer();

				return reinterpret_cast<const std::array<float, 4>&>(vector);
			});

		vectorWidget->RegisterProvider([provider](std::array<float, 4>* value)
			{
				provider(reinterpret_cast<DUOLMath::Vector4&>(*value));
			});
	}

	void ImGuiHelper::DrawInt(DUOLEditor::WidgetGroupBase* rootWidget, const DUOLCommon::tstring& name,
		std::function<int()> gatherer, std::function<void(int)> provider, int speed, int min, int max)
	{
		DrawTitle(rootWidget, name);

		auto floatWidget = rootWidget->AddWidget<DragScalar<int, 1>>
			(min, max, 0, speed, TEXT(""), TEXT("%d"));

		floatWidget->RegisterGatherer([gatherer]()
			{
				int value = gatherer();

				return reinterpret_cast<const std::array<int, 1>&>(value);
			});

		floatWidget->RegisterProvider([provider](std::array<int, 1>* value)
			{
				provider(reinterpret_cast<int&>(*value));
			});
	}

	void ImGuiHelper::DrawString(DUOLEditor::WidgetGroupBase* rootWidget, const DUOLCommon::tstring& name,
		std::function<DUOLCommon::tstring()> gatherer, std::function<void(DUOLCommon::tstring)> provider)
	{
		DrawTitle(rootWidget, name);

		auto&& textWidget = rootWidget->AddWidget<DUOLEditor::InputText>();

		textWidget->RegisterGatherer([gatherer]()
			{
				return gatherer();
			});

		textWidget->RegisterProvider([provider](DUOLCommon::tstring* string)
			{
				provider(*string);
			});
	}

	DUOLEditor::TextClickable* ImGuiHelper::DrawStringNoInput(DUOLEditor::WidgetGroupBase* rootWidget, const DUOLCommon::tstring& name,
		std::function<DUOLCommon::tstring()> gatherer, std::function<void(DUOLCommon::tstring)> provider, std::function<void(void)> callbackAfter, bool title)
	{
		if (title)
			DrawTitle(rootWidget, name);

		auto&& textWidget = rootWidget->AddWidget<DUOLEditor::TextClickable>(TEXT(""), callbackAfter);

		textWidget->RegisterGatherer([gatherer]()
			{
				return gatherer();
			});

		textWidget->RegisterProvider([provider](DUOLCommon::tstring* string)
			{
				provider(*string);
			});

		return textWidget;
	}

	void ImGuiHelper::DrawColor3(DUOLEditor::WidgetGroupBase* rootWidget, const DUOLCommon::tstring& name,
		std::function<DUOLMath::Vector3()> gatherer, std::function<void(DUOLMath::Vector3)> provider)
	{
		DrawTitle(rootWidget, name);

		auto&& colorWidget = rootWidget->AddWidget<DUOLEditor::ColorEdit>(false, DUOLEditor::Color::Green);

		colorWidget->RegisterGatherer([gatherer]()
			{
				DUOLMath::Vector3 colorVec = gatherer();

				return reinterpret_cast<Color&>(colorVec);
			});

		colorWidget->RegisterProvider([provider](DUOLEditor::Color* color)
			{
				provider(reinterpret_cast<DUOLMath::Vector3&>(*color));
			});
	}

	void ImGuiHelper::DrawTitle(DUOLEditor::WidgetGroupBase* rootWidget, const DUOLCommon::tstring& name)
	{
		rootWidget->AddWidget<DUOLEditor::TextColored>(name, TITLE_COLOR);
	}
}