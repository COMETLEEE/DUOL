#include "DUOLGameEngine/ECS/Component/RectTransform.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/Manager/SceneManagement/Scene.h"

#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"

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
		metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
	)
	.property("PosY", &DUOLGameEngine::RectTransform::GetPosY,&DUOLGameEngine::RectTransform::SetRectY)
	(
		metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
	)
	.property("PosZ", &DUOLGameEngine::RectTransform::GetPosZ,&DUOLGameEngine::RectTransform::SetPosZ)
	(
		metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)
	)
	.property("Width", &DUOLGameEngine::RectTransform::GetWidth,&DUOLGameEngine::RectTransform::SetRectZ)
	(
		metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)

	)
	.property("Height", &DUOLGameEngine::RectTransform::GetHeight,&DUOLGameEngine::RectTransform::SetRectW)
	(
		metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float)

	)
	.property("Min", &DUOLGameEngine::RectTransform::GetAnchorMin, &DUOLGameEngine::RectTransform::SetAnchorMin)
	(
		metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float2)

	)
	.property("Max", &DUOLGameEngine::RectTransform::GetAnchorMax, &DUOLGameEngine::RectTransform::SetAnchorMax)
	(
		metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float2)
	)
	.property("Pivot", &DUOLGameEngine::RectTransform::GetPivot, &DUOLGameEngine::RectTransform::SetPivot)
	(
		metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float2)

	)
	.property("Rotation", &DUOLGameEngine::RectTransform::GetRotation, &DUOLGameEngine::RectTransform::SetRotate)
	(
		metadata(DUOLCommon::MetaDataType::Inspectable, true)
		, metadata(DUOLCommon::MetaDataType::InspectType, DUOLCommon::InspectType::Float3)
	)
	.property("Scale", &DUOLGameEngine::RectTransform::GetScale, &DUOLGameEngine::RectTransform::SetScale)
	(
		metadata(DUOLCommon::MetaDataType::Inspectable, true)
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
		Transform(owner,name)
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

	DUOLGraphicsLibrary::Rect DUOLGameEngine::RectTransform::CalculateRect(DUOLMath::Vector2 screenSize)
	{
		DUOLGraphicsLibrary::Rect ret;

		//Ȥ�ø𸣴� �ϴ� ��Ŀ���� clamp���ݴϴ�. ���߿� �ٱ����� ó�����شٸ� �ʿ���� �ڵ��Դϴ�
		//todo :: �ܺο��� SetAnchor�϶� ó�����ְ� �ڵ带 ����
		//DUOLMath::Vector2 min{0.f, 0.f} ;
		//DUOLMath::Vector2 max{1.f, 1.f} ;

		//_anchorMin.Clamp(min, max);
		//_anchorMax.Clamp(min, max);

		// ������ ���� 0�� ����� ���� rect�� left�� right�� posX�� Width�� ����մϴ�.
		if ((_anchorMin.x - _anchorMax.x) < 0.0000000001f)
		{
			//��Ŀ�� �������� posX��ŭ �ű� ���� right�� width�� �����ݴϴ�.
			float anchorX = screenSize.x * _anchorMin.x;

			ret.left = anchorX + _rect.x;
			ret.right = ret.left + _rect.z;
		}
		else
		{
			//�������� ���� left�� right�� �����մϴ�.
			//todo:: ������ ��Ŀ���� ���� rect min max�� ������ ����մϴ�

			ret.left = _rect.x;
			ret.right = _rect.z;
		}

		//����Ƽ UI ��ǥ��� Max���� top�� ������ �޽��ϴ�. �� ������ ����մϴ�.
		// ������ ���� 0�� ����� ���� rect�� posy�� Height�� ����մϴ�.
		if ((_anchorMin.y - _anchorMax.y) < 0.0000000001f)
		{
			float anchorY = (1.0f - _anchorMin.y) * screenSize.y;

			ret.top = anchorY - _rect.y;
			ret.bottom = ret.top + _rect.w;
		}
		else
		{
			//�������� ���� top�� bottom�� �����մϴ�.
			//todo:: ������ ��Ŀ���� ���� rect min max�� ������ ����մϴ�
			ret.top = _rect.y;
			ret.bottom = _rect.w;
		}

		return ret;
	}

	void RectTransform::SetAnchorMin(const DUOLMath::Vector2& anchor_min)
	{
		_anchorMin = anchor_min;
	}

	void RectTransform::SetAnchorMax(const DUOLMath::Vector2& anchor_max)
	{
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
