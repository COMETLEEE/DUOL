#include "DUOLEditor/UI/Panels/PanelTransformableBase.h"

namespace DUOLEditor
{
	PanelTransformableBase::PanelTransformableBase(const DUOLMath::Vector2& defaultPosition,
		const DUOLMath::Vector2& defaultSize, HorizontalAlignment defaultHAlignment,
		VerticalAlignment defaultVAlignment, bool ignoreConfigFile) :
		_defaultPosition(defaultPosition)
		, _defaultSize(defaultSize)
		, _defaultHorizontalAlignment(defaultHAlignment)
		, _defaultVerticalAlignment(defaultVAlignment)
		, _ignoreConfigFile(ignoreConfigFile)
	{
	}

	void PanelTransformableBase::SetPosition(const DUOLMath::Vector2& position)
	{
		_position = position;

		_positionChanged = true;
	}

	void PanelTransformableBase::SetSize(const DUOLMath::Vector2& size)
	{
		_size = size;

		_sizeChanged = true;
	}

	void PanelTransformableBase::SetAlignment(HorizontalAlignment horizontalAlignment,
		VerticalAlignment verticalAlignment)
	{
		_horizontalAlignment = horizontalAlignment;

		_verticalAlignment = verticalAlignment;

		_alignmentChanged = true;
	}

	const DUOLMath::Vector2& PanelTransformableBase::GetPosition() const
	{
		return _position;
	}

	const DUOLMath::Vector2& PanelTransformableBase::GetSize() const
	{
		return _size;
	}

	HorizontalAlignment PanelTransformableBase::GetHorizontalAlignment() const
	{
		return _horizontalAlignment;
	}

	VerticalAlignment PanelTransformableBase::GetVerticalAlignment() const
	{
		return _verticalAlignment;
	}

	void PanelTransformableBase::Update()
	{
		if (!_autoSize)
			UpdateSize();

		CopySize();

		UpdatePosition();

		CopyPosition();
	}

	DUOLMath::Vector2 PanelTransformableBase::CalcPositionAlignmentOffset(bool useDefault)
	{
		DUOLMath::Vector2 result(0.f, 0.f);

		switch (useDefault ? _defaultHorizontalAlignment : _horizontalAlignment)
		{
			case HorizontalAlignment::Center:
			{
				result.x -= _size.x / 2.f;

				break;
			}

			case HorizontalAlignment::Right:
			{
				result.x -= _size.x;

				break;
			}
		}

		switch (useDefault ? _defaultVerticalAlignment : _verticalAlignment)
		{
			case VerticalAlignment::Middle:
			{
				result.y -= _size.y / 2.f;

				break;
			}

			case VerticalAlignment::Bottom:
			{
				result.y -= _size.y;

				break;
			}
		}

		return result;
	}

	void PanelTransformableBase::UpdatePosition()
	{
		// 디폴트 포지션을 사용한다면.
		if (_defaultPosition.x != -1.f && _defaultPosition.y != -1.f)
		{
			DUOLMath::Vector2 offsetedDefaultPos = _defaultPosition + CalcPositionAlignmentOffset(true);
			ImGui::SetWindowPos(ImGuiHelper::ToImVec2(offsetedDefaultPos), _ignoreConfigFile ? ImGuiCond_Once : ImGuiCond_FirstUseEver);
		}

		// 위치 또는 배치가 바뀌었다면 ..
		if (_positionChanged || _alignmentChanged)
		{
			DUOLMath::Vector2 offset = CalcPositionAlignmentOffset(false);

			DUOLMath::Vector2 offsetedPos = _position + offset;

			ImGui::SetWindowPos(ImGuiHelper::ToImVec2(offsetedPos), ImGuiCond_Always);

			_positionChanged = false;
			_alignmentChanged = false;
		}
	}

	void PanelTransformableBase::UpdateSize()
	{
		if (_sizeChanged)
		{
			ImGui::SetWindowSize(ImGuiHelper::ToImVec2(_size), ImGuiCond_Always);
			_sizeChanged = false;
		}
	}

	void PanelTransformableBase::CopyPosition()
	{
		_position = ImGuiHelper::ToVector2(ImGui::GetWindowPos());
	}

	void PanelTransformableBase::CopySize()
	{
		_size = ImGuiHelper::ToVector2(ImGui::GetWindowSize());
	}
}
