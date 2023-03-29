#include "DUOLGameEngine/ECS/Component/RectTransform.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/Manager/SceneManagement/Scene.h"

#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"
#include "DUOLGameEngine/Manager/GraphicsManager.h"

using namespace rttr;

RTTR_PLUGIN_REGISTRATION
{
	rttr::registration::class_<DUOLGameEngine::RectTransform>("RectTransform")
	.constructor()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.constructor<DUOLGameEngine::GameObject*, const DUOLCommon::tstring&>()
	(
		rttr::policy::ctor::as_raw_ptr
	)
	.property("PosX", &DUOLGameEngine::RectTransform::GetPosX,&DUOLGameEngine::RectTransform::SetRectX)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		,metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
	)
	.property("PosY", &DUOLGameEngine::RectTransform::GetPosY,&DUOLGameEngine::RectTransform::SetRectY)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		,metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
	)
	.property("PosZ", &DUOLGameEngine::RectTransform::GetPosZ,&DUOLGameEngine::RectTransform::SetPosZ)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		,metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
	)
	.property("Width", &DUOLGameEngine::RectTransform::GetWidth,&DUOLGameEngine::RectTransform::SetRectZ)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		,metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)

	)
	.property("Height", &DUOLGameEngine::RectTransform::GetHeight,&DUOLGameEngine::RectTransform::SetRectW)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		,metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)

	)
	.property("Min", &DUOLGameEngine::RectTransform::GetAnchorMin, &DUOLGameEngine::RectTransform::SetAnchorMin)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		,metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float2)

	)
	.property("Max", &DUOLGameEngine::RectTransform::GetAnchorMax, &DUOLGameEngine::RectTransform::SetAnchorMax)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		,metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float2)
	)
	.property("Pivot", &DUOLGameEngine::RectTransform::GetPivot, &DUOLGameEngine::RectTransform::SetPivot)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		,metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float2)

	)
	.property("Rotation.", &DUOLGameEngine::RectTransform::GetRotation, &DUOLGameEngine::RectTransform::SetRotate)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		,metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float3)
	)
	.property("Scale.", &DUOLGameEngine::RectTransform::GetScale, &DUOLGameEngine::RectTransform::SetScale)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
		,metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float3)
	);
}


namespace DUOLGameEngine
{
	RectTransform::RectTransform() :
		Transform()
		, _pivot(0.5f, 0.5f)
		, _anchorMin(0.5f, 0.5f)
		, _anchorMax(0.5f, 0.5f)
		, _rect(0.f, 0.f, 100.f, 100.f)
		, _posZ(0.f)
		, _rotation(0.f, 0.f, 0.f)
		, _scale(1.f, 1.f, 1.f)
	{
	}

	RectTransform::RectTransform(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name) :
		Transform(owner, name)
		, _pivot(0.5f, 0.5f)
		, _anchorMin(0.5f, 0.5f)
		, _anchorMax(0.5f, 0.5f)
		, _rect(0.f, 0.f, 100.f, 100.f)
		, _posZ(0.f)
		, _rotation(0.f, 0.f, 0.f)
		, _scale(1.f, 1.f, 1.f)
	{
	}

	DUOLGameEngine::RectTransform::~RectTransform()
	{
	}

	DUOLGraphicsLibrary::Rect DUOLGameEngine::RectTransform::CalculateRect(DUOLMath::Vector2 _rectpos)
	{
		_calculateRect;

		auto screensize = DUOLGameEngine::GraphicsManager::GetInstance()->GetScreenSize();

		//혹시모르니 일단 앵커값을 clamp해줍니다. 나중에 바깥에서 처리해준다면 필요없는 코드입니다
		//todo :: 외부에서 SetAnchor일때 처리해주고 코드를 빼자
		//DUOLMath::Vector2 min{0.f, 0.f} ;
		//DUOLMath::Vector2 max{1.f, 1.f} ;

		//_anchorMin.Clamp(min, max);
		//_anchorMax.Clamp(min, max);

		// 오차가 대충 0과 가까울 경우는 rect의 left와 right을 posX와 Width로 계산합니다.
		if (std::abs(_anchorMin.x - _anchorMax.x) < 0.0000000001f)
		{
			//앵커를 기준으로 posX만큼 옮긴 값과 right엔 width를 더해줍니다.
			//float anchorX = _rectpos.x * _anchorMin.x;
			float anchorX = (screensize.x / 10) * _anchorMin.x;

			//_calculateRect.left = anchorX + _rect.x;
			//_calculateRect.right = _calculateRect.left + _rect.z;

			_calculateRect.left = (_rect.x * _anchorMin.x) + _rectpos.x - (screensize.x * _anchorMin.x);
			_calculateRect.right = _calculateRect.left + _rect.w;
		}
		else
		{
			//절대적인 값인 left와 right로 적용합니다.
			//todo:: 하지만 앵커값에 따라 rect min max를 적용해 줘야합니다

			//auto rectX = (screensize.x / 10) * _anchorMin.x;
			//auto rectZ= (screensize.y / 10) * _anchorMax.x + _rectpos.x;
			//float anchorX = (screensize.x / 10) * 0.5f;

			/*	_calculateRect.left = _rect.x;
				_calculateRect.right = _rect.z;*/

				//_calculateRect.left = (anchorX + _rect.x) - rectX;
				//_calculateRect.right = _calculateRect.left + _rect.z + rectX;

			_calculateRect.left = (_rect.x * _anchorMin.x) + _rectpos.x - (screensize.x * _anchorMin.x);
			_calculateRect.right = (_rect.x * _anchorMax.x) + _rectpos.x - (screensize.x * _anchorMax.x);
		}

		//유니티 UI 좌표계는 Max값이 top에 영향을 받습니다. 즉 뒤집어 줘야합니다.
		// 오차가 대충 0과 가까울 경우는 rect의 posy와 Height로 계산합니다.
		if (std::abs(_anchorMin.y - _anchorMax.y) < 0.0000000001f)
		{
			//float anchorY = (1.0f - _anchorMin.y) * _rectpos.y;
			float anchorY = (screensize.y / 10) * _anchorMin.y;

			_calculateRect.top = (_rect.y * _anchorMin.y) + _rectpos.y - (screensize.y * _anchorMin.y);
			_calculateRect.bottom = _calculateRect.top + _rect.w;
		}
		else
		{

			//절대적인 값인 top와 bottom로 적용합니다.
			//todo:: 하지만 앵커값에 따라 rect min max를 적용해 줘야합니다
			/*_calculateRect.top = _rect.y;
			_calculateRect.bottom = _rect.w;*/

			_calculateRect.top = (_rect.y * _anchorMin.y) + _rectpos.y - (screensize.y * _anchorMin.y);
			_calculateRect.bottom = (_rect.y * _anchorMax.y) + _rectpos.y - (screensize.y * _anchorMax.y);

		}

		if (_calculateRect.top < 0.f)
			_calculateRect.top = 0.f;
		if (_calculateRect.bottom < 0.f)
			_calculateRect.bottom = 0.f;

		// Scale 적용
		float scaleWidth = (_calculateRect.right - _calculateRect.left) * _scale.x;
		float scaleHight = (_calculateRect.bottom - _calculateRect.top) * _scale.y;

		_calculateRect.left = _calculateRect.left - (scaleWidth / 2.0f);
		_calculateRect.right = _calculateRect.right + (scaleWidth / 2.0f);
		_calculateRect.top = _calculateRect.top - (scaleHight / 2.0f);
		_calculateRect.bottom = _calculateRect.bottom + (scaleHight / 2.0f);

		return _calculateRect;
	}

	void RectTransform::Scale(DUOLMath::Vector2& centerpoint)
	{
		
	}

	void RectTransform::SetAnchorMin(DUOLMath::Vector2& anchor_min)
	{
		if (anchor_min.x < 0.f)
			anchor_min.x = 0.f;

		if (1.f < anchor_min.x)
			anchor_min.x = 1.f;

		if (anchor_min.y < 0.f)
			anchor_min.y = 0.f;

		if (1.f < anchor_min.y)
			anchor_min.y = 1.f;

		_anchorMin = anchor_min;
	}

	void RectTransform::SetAnchorMax(DUOLMath::Vector2& anchor_max)
	{
		if (anchor_max.x < 0.f)
			anchor_max.x = 0.f;

		if (1.f < anchor_max.x)
			anchor_max.x = 1.f;

		if (anchor_max.y < 0.f)
			anchor_max.y = 0.f;

		if (1.f < anchor_max.y)
			anchor_max.y = 1.f;

		_anchorMax = anchor_max;
	}

	void RectTransform::SetPivot(const DUOLMath::Vector2& pivot)
	{
		_pivot = pivot;
	}


	void RectTransform::SetRect(const DUOLMath::Vector4& rect)
	{
		_rect = rect;
	}

	void RectTransform::SetRectX(const float& x)
	{
		_rect.x = x;
	}

	void RectTransform::SetRectY(const float& y)
	{
		_rect.y = y;
	}

	void RectTransform::SetPosZ(const float& posz)
	{
		_posZ = posz;
	}

	void RectTransform::SetRectZ(const float& z)
	{
		_rect.z = z;
	}

	void RectTransform::SetRectW(const float& w)
	{
		_rect.w = w;
	}

	void RectTransform::SetRotate(const DUOLMath::Vector3& rotate)
	{
		_rotation = rotate;
	}

	void RectTransform::SetScale(const DUOLMath::Vector3& scale)
	{
		_scale = scale;
	}

}
