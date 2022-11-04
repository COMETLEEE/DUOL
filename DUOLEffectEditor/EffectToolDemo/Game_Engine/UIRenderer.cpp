#include "pch.h"
#include "UIRenderer.h"

namespace Muscle
{
	Muscle::UIRenderer::UIRenderer(std::shared_ptr<GameObject> _GameObject) :IComponents(_GameObject)
	{
		_uiData = std::make_shared<RenderingData_UI>();
		_rectFill = 1.f;
	}

	Muscle::UIRenderer::~UIRenderer()
	{
		// 삭제하는 코드 추가
		_uiData.reset();
	}

	void Muscle::UIRenderer::Start()
	{
	}

	void Muscle::UIRenderer::Update()
	{

	}

	void Muscle::UIRenderer::Render()
	{
		float right = _initialRight * _rectFill;

		_uiData->_ndcInfo->_rightBottom.x = _uiData->_ndcInfo->_leftTop.x + right;

		MuscleEngine::Get()->GetGraphicsManager()->PostRenderingData_UI(_uiData);
	}

	const DirectX::SimpleMath::Vector4&& UIRenderer::GetDrawNDCPosition()
	{
		Vector2 leftTop = _uiData->_ndcInfo->_leftTop;
		Vector2 rightBottom = _uiData->_ndcInfo->_rightBottom;

		return Vector4(leftTop.x, leftTop.y, rightBottom.x, rightBottom.y);
	}

	void UIRenderer::SetDrawNDCPosition(const Vector2& leftTop, const Vector2& rightBottom)
	{
		_uiData->_ndcInfo->_leftTop = leftTop;

		_uiData->_ndcInfo->_rightBottom = rightBottom;

		_initialRight = _uiData->_ndcInfo->_rightBottom.x - _uiData->_ndcInfo->_leftTop.x;
	}
}