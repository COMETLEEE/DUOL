#include "DUOLEditor/Util/ImGuiHelper.h"

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
}