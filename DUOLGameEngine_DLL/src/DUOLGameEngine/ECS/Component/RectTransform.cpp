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
