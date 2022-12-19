#pragma once
#include "DUOLEditor/UI/Panels/PanelBase.h"

namespace DUOLEditor
{
	/**
	 * \brief panel that is localized in the canvas.
	 */
	class PanelTransformableBase : public PanelBase
	{
	public:
		PanelTransformableBase(const DUOLMath::Vector2& defaultPosition = { -1.f, -1.f }, const DUOLMath::Vector2& defaultSize = { -1.f, -1.f },
			HorizontalAlignment defaultHAlignment = HorizontalAlignment::Left
			, VerticalAlignment defaultVAlignment = VerticalAlignment::Top
			, bool ignoreConfigFile = false);

		void SetPosition(const DUOLMath::Vector2& position);

		void SetSize(const DUOLMath::Vector2& size);

		void SetAlignment(HorizontalAlignment horizontalAlignment, VerticalAlignment verticalAlignment);

		const DUOLMath::Vector2& GetPosition() const;

		const DUOLMath::Vector2& GetSize() const;

		HorizontalAlignment GetHorizontalAlignment() const;

		VerticalAlignment GetVerticalAlignment() const;

	protected:
		void Update();

		// 이것도 가상 클래스. 상속을 위한 클래스입니다.
		virtual void Draw_Impl() override = 0;

	private:
		DUOLMath::Vector2 CalcPositionAlignmentOffset(bool useDefault = false);

		void UpdatePosition();

		void UpdateSize();

		void CopyPosition();

		void CopySize();

	public:
		bool _autoSize = true;

	protected:
		DUOLMath::Vector2 _defaultPosition;

		DUOLMath::Vector2 _defaultSize;

		HorizontalAlignment _defaultHorizontalAlignment;

		VerticalAlignment _defaultVerticalAlignment;

		DUOLMath::Vector2 _position = {0.f, 0.f};

		DUOLMath::Vector2 _size = { 0.f, 0.f };

		HorizontalAlignment _horizontalAlignment;

		VerticalAlignment _verticalAlignment;

		bool _ignoreConfigFile;

		bool _positionChanged = false;

		bool _sizeChanged = false;

		bool _alignmentChanged = false;
	};
}