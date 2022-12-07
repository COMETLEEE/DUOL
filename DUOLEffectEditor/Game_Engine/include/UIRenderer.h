#pragma once
#include "IComponents.h"
#include "Export\RenderingData.h"

namespace Muscle
{
	class UIRenderer : public IComponents
	{
	public:
		UIRenderer(std::shared_ptr<GameObject> _GameObject);

		~UIRenderer();

	private:
		std::shared_ptr<MuscleGrapics::RenderingData_UI> _uiData;

		float _initialRight;

		float _rectFill; // 얼마나 채울것인가..? 0 ~ 1 사이 값, 1이 가득 채운 상태.

	public:
		inline std::shared_ptr<MuscleGrapics::RenderingData_UI>& GetUIData() { return _uiData; }

	public:
		// Left, Top, Right, Bottom
		const DUOLMath::Vector4&& GetDrawNDCPosition();

		void SetDrawNDCPosition(const DUOLMath::Vector2& leftTop, const DUOLMath::Vector2& rightBottom);

		inline const uint64& GetTextureID() { return _uiData->_drawInfo->_textureID; }

		inline void SetTextureID(const uint64& textureID) { _uiData->_drawInfo->_textureID = textureID; }

		inline const uint32& GetSortOrder() { return _uiData->_drawInfo->_sortOrder; }

		inline void SetSortOrder(const uint32& sortOrder) { _uiData->_drawInfo->_sortOrder = sortOrder; }

		inline void SetUseAlphaBlend(bool value) { _uiData->_drawInfo->_useAlphaBlend = value; }

		inline void SetRectFill(float _fill) { _rectFill = _fill; }

	public:
		virtual void Start() override;

		virtual void Update() override;

		virtual void Render() override;
	};
}