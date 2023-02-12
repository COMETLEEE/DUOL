#include "DUOLGameEngine/ECS/Component/RectTransform.h"

namespace DUOLGameEngine
{
	RectTransform::RectTransform(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name):
		ComponentBase(owner, name)
		, _pivot(0.5f, 0.5f)
		, _anchorMin(0.5f, 0.5f)
		, _anchorMax(0.5f, 0.5f)
		, _rect(0.f, 0.f, 100.f, 100.f)
		, _rotation(0.f, 0.f, 0.f)
		, _scale(1.f, 1.f, 1.f)
	{
	}

	DUOLGameEngine::RectTransform::~RectTransform()
	{
	}

	DUOLGraphicsLibrary::Rect DUOLGameEngine::RectTransform::CalculateRect(DUOLMath::Vector2 screenSize)
	{
		DUOLGraphicsLibrary::Rect ret;

		//혹시모르니 일단 앵커값을 clamp해줍니다. 나중에 바깥에서 처리해준다면 필요없는 코드입니다
		//todo :: 외부에서 SetAnchor일때 처리해주고 코드를 빼자
		//DUOLMath::Vector2 min{0.f, 0.f} ;
		//DUOLMath::Vector2 max{1.f, 1.f} ;

		//_anchorMin.Clamp(min, max);
		//_anchorMax.Clamp(min, max);

		// 오차가 대충 0과 가까울 경우는 rect의 left와 right을 posX와 Width로 계산합니다.
		if ((_anchorMin.x - _anchorMax.x) < 0.0000000001f)
		{
			//앵커를 기준으로 posX만큼 옮긴 값과 right엔 width를 더해줍니다.
			float anchorX = screenSize.x * _anchorMin.x;

			ret.left = anchorX + _rect.x;
			ret.right = ret.left + _rect.z;
		}
		else
		{
			//절대적인 값인 left와 right로 적용합니다.
			//todo:: 하지만 앵커값에 따라 rect min max를 적용해 줘야합니다

			ret.left = _rect.x;
			ret.right = _rect.z;
		}

		//유니티 UI 좌표계는 Max값이 top에 영향을 받습니다. 즉 뒤집어 줘야합니다.
		// 오차가 대충 0과 가까울 경우는 rect의 posy와 Height로 계산합니다.
		if ((_anchorMin.y - _anchorMax.y) < 0.0000000001f)
		{
			float anchorY = (1.0f - _anchorMin.y) * screenSize.y;
			
			ret.top = anchorY - _rect.y;
			ret.bottom = ret.top + _rect.w;
		}
		else
		{
			//절대적인 값인 top와 bottom로 적용합니다.
			//todo:: 하지만 앵커값에 따라 rect min max를 적용해 줘야합니다
			ret.top =  _rect.y;
			ret.bottom = _rect.w;
		}

		return ret;
	}

	DUOLMath::Vector2 RectTransform::GetPivot() const
	{
		return _pivot;
	}

	DUOLMath::Vector2 RectTransform::GetAnchorMin() const
	{
		return _anchorMin;
	}

	void RectTransform::SetAnchorMin(const DUOLMath::Vector2& anchor_min)
	{
		_anchorMin = anchor_min;
	}

	DUOLMath::Vector2 RectTransform::GetAnchorMax() const
	{
		return _anchorMax;
	}

	void RectTransform::SetAnchorMax(const DUOLMath::Vector2& anchor_max)
	{
		_anchorMax = anchor_max;
	}

	void RectTransform::SetPivot(const DUOLMath::Vector2& pivot)
	{
		_pivot = pivot;
	}

	DUOLMath::Vector4 RectTransform::GetRect() const
	{
		return _rect;
	}

	void RectTransform::SetRect(const DUOLMath::Vector4& rect)
	{
		_rect = rect;
	}
}
