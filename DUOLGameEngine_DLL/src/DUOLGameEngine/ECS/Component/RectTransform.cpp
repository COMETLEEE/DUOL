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
	)
	.method("SetActive", &DUOLGameEngine::RectTransform::IsSetActive)
	.method("SetUnActive", &DUOLGameEngine::RectTransform::SetUnActive)
	.method("SetActiveOn", &DUOLGameEngine::RectTransform::SetActive);
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
		, _dirtyFlagRotate(true)
		, _dirtyFlagScale(true)
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
		, _dirtyFlagRotate(true)
		, _dirtyFlagScale(true)
	{
	}

	DUOLGameEngine::RectTransform::~RectTransform()
	{
		_rect = _preRect;
	}

	// screensize�� �Ѱ��ݴϴ�. 
	DUOLGraphicsLibrary::Rect DUOLGameEngine::RectTransform::CalculateRect(DUOLMath::Vector2 _rectpos)
	{
		//Ȥ�ø𸣴� �ϴ� ��Ŀ���� clamp���ݴϴ�. ���߿� �ٱ����� ó�����شٸ� �ʿ���� �ڵ��Դϴ�
		DUOLMath::Vector2 min{ 0.f, 0.f };
		DUOLMath::Vector2 max{ 1.f, 1.f };

		_anchorMin.Clamp(min, max);
		_anchorMax.Clamp(min, max);

		//auto parenttransform = this->GetParent();
		//DUOLMath::Vector2 _rectPos;

		//// ���� �θ� �ִٸ�
		//if (parenttransform)
		//{
		//	auto parentobject = parenttransform->GetGameObject();
		//	if(parentobject->GetName() ==L"Canvas")
		//		_rectPos = _rectpos;
		//	else
		//	{
		//		auto parentrecttrasnform = parentobject->GetComponent<RectTransform>();
		//		if (parentrecttrasnform)
		//		{
		//			_rectPos.x = parentrecttrasnform->GetWidth();
		//			_rectPos.y =parentrecttrasnform->GetHeight();
		//		}
		//	}
		//}
		//else
		//	_rectPos = _rectpos;

		// Anchor�� �������� �������� ��ġ�� ������.
		// x�� �������� width�� ���� ��ġ
		// ��, Anchor�� ������ ��ġ
		if (_anchorMin.x == _anchorMax.x)
		{
			// �̹����� ��ġ�ϴ� ������ ��ǥ�� (_rectpos.x * _anchorMin.x) �̴�.
			// �׸� x��ǥ�� �����ش�.
			float middleposX = (_rectpos.x * _anchorMin.x);
			_calculateRect.left = middleposX - (_rect.z / 2) + _rect.x;
			_calculateRect.right = middleposX + (_rect.z / 2) + _rect.x;
		}
		// Anchor�� ���� ������
		// �������� ������ �����̴�.
		// ���⼭ x��  width �� Left�� Right�� �ٲ��. 
		else
		{
			_calculateRect.left = (_rectpos.x * _anchorMin.x) + _rect.x;
			_calculateRect.right = (_rectpos.x * _anchorMax.x) - _rect.z;
		}
		// y�� �������� Height�� ���� ��ġ
		if (_anchorMin.y == _anchorMax.y)
		{
			// ��밡 ���ϱ� ���ؼ� ���ϴ� �������� �����ش�.
			float middleposY = (_rectpos.y * (1 - _anchorMin.y));
			_calculateRect.top = middleposY - (_rect.w / 2) - _rect.y;
			_calculateRect.bottom = middleposY + (_rect.w / 2) - _rect.y;
		}
		else
		{
			_calculateRect.bottom = (_rectpos.y * (1 - _anchorMin.y)) - _rect.w;
			_calculateRect.top = (_rectpos.y * (1 - _anchorMax.y)) + _rect.y;
		}

		//float pivotWidth;
		//float pivotHeight;

		//pivotWidth = (_calculateRect.right - _calculateRect.left) * _pivot.x - (_calculateRect.right - _calculateRect.left) / 2;
		//pivotHeight = (_calculateRect.bottom - _calculateRect.top) * _pivot.y - (_calculateRect.bottom - _calculateRect.top) / 2;


		//_calculateRect.left = _calculateRect.left + pivotWidth;
		//_calculateRect.right = _calculateRect.right + pivotWidth;
		//_calculateRect.top = _calculateRect.top + pivotHeight;
		//_calculateRect.bottom = _calculateRect.bottom + pivotHeight;
//
//#pragma region Scale
//		float scaleWidth;
//		float scaleHight;
//
//		if (_scale.x == 0 || _scale.y == 0)
//		{
//			_calculateRect.left = 0.f;
//			_calculateRect.right = 0.f;
//		}
//		else
//		{
//			// �þ�ų� �پ���� ��ġ
//			scaleWidth = (_calculateRect.right - _calculateRect.left) * _scale.x - (_calculateRect.right - _calculateRect.left);
//			scaleHight = (_calculateRect.bottom - _calculateRect.top) * _scale.y - (_calculateRect.bottom - _calculateRect.top);
//
//			scaleWidth /= 2;
//			scaleHight /= 2;
//		}
//
//		_calculateRect.left = _calculateRect.left - scaleWidth;
//		_calculateRect.right = _calculateRect.right + scaleWidth;
//		_calculateRect.top = _calculateRect.top - scaleHight;
//		_calculateRect.bottom = _calculateRect.bottom + scaleHight;
//#pragma endregion 
//
//
//#pragma region Rotation
//		float centerX = _calculateRect.left + ((_calculateRect.right - _calculateRect.left) / 2);
//		float centerY = _calculateRect.top + ((_calculateRect.bottom - _calculateRect.top) / 2);
//
//		// ��� ������ ����
//		DUOLMath::Vector3 axis = DUOLMath::Vector3(0.0f, 1.0f, 0.0f);
//		float angle = DUOLMath::MathHelper::DegreeToRadian(_rotation.z);
//
//		DUOLMath::Matrix rotationMatrix = DUOLMath::Matrix::CreateFromAxisAngle(axis, angle);
//		DUOLMath::Matrix translationMatrix = DUOLMath::Matrix::CreateTranslation(-centerX, -centerY, 0.0f);
//		DUOLMath::Matrix reverseTranslationMatrix = DUOLMath::Matrix::CreateTranslation(centerX, centerY, 0.0f);
//
//		// �������� �ű�� ������ �ٽ� �ű�
//		DUOLMath::Matrix finalTransform = translationMatrix * rotationMatrix * reverseTranslationMatrix;
//
//		// rect�� ����
//		DUOLMath::Vector4 topleft = DUOLMath::Vector4(_calculateRect.left, _calculateRect.top, 0.0f, 1.0f);
//		DUOLMath::Vector4 bottomright = DUOLMath::Vector4(_calculateRect.right, _calculateRect.bottom, 0.0f, 1.0f);
//
//		topleft = DUOLMath::Vector4::Transform(topleft, finalTransform);
//		bottomright = DUOLMath::Vector4::Transform(bottomright, finalTransform);
//
//		_calculateRect.left = topleft.x;
//		_calculateRect.right = bottomright.x;
//		_calculateRect.top = topleft.y;
//		_calculateRect.bottom = bottomright.y;
//

#pragma endregion
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
		_preRect = _rect;
	}

	void RectTransform::SetRectX(const float& x)
	{
		_rect.x = x;
		_preRect.x = _rect.x;
	}

	void RectTransform::SetRectY(const float& y)
	{
		_rect.y = y;
		_preRect.y = _rect.y;
	}

	void RectTransform::SetPosZ(const float& posz)
	{
		_posZ = posz;
	}

	void RectTransform::SetRectZ(const float& z)
	{
		_rect.z = z;
		_preRect.z = _rect.z;
	}

	void RectTransform::SetRectW(const float& w)
	{
		_rect.w = w;
		_preRect.w = _rect.w;
	}

	void RectTransform::SetRotate(const DUOLMath::Vector3& rotate)
	{
		_dirtyFlagRotate = true;
		_rotation = rotate;
	}

	void RectTransform::SetScale(const DUOLMath::Vector3& scale)
	{
		_dirtyFlagScale = true;
		_scale = scale;
	}

	void DUOLGameEngine::RectTransform::IsSetActive()
	{
		// �����غôµ� �׳� �ڱ��ڽŸ� Ȱ��ȭ or��Ȱ��ȭ ����
		bool value = !this->GetTransform()->GetGameObject()->GetIsActive();
		this->GetTransform()->GetGameObject()->SetIsActiveSelf(value);
	}

	void RectTransform::SetUnActive()
	{
		this->GetTransform()->GetGameObject()->SetIsActiveSelf(false);
	}

	void RectTransform::SetActive()
	{
		this->GetTransform()->GetGameObject()->SetIsActiveSelf(true);
	}
}
