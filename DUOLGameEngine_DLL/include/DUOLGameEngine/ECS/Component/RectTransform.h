#pragma once
#include "DUOLGameEngine/ECS/Component/ComponentBase.h"
#include "DUOLGraphicsLibrary/FontEngine/IFontEngine.h"
#include "DUOLMath/DUOLMath.h"

namespace DUOLGameEngine
{
 /**

     @class   RectTransform
     @brief   
     @details ~
     @author  KyungMin Oh

 **/
	class DUOL_GAMEENGINE_API RectTransform final : public DUOLGameEngine::ComponentBase, public std::enable_shared_from_this<RectTransform>
	{
		enum class AnchorPreset
		{
		};
	public:
		RectTransform(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name = DUOLCommon::StringHelper::ToTString("RectTransform"));

		~RectTransform() override;

	public:
		DUOLGraphicsLibrary::Rect CalculateRect(DUOLMath::Vector2 screenSize);

		DUOLMath::Vector2 GetPivot() const;

		DUOLMath::Vector2 GetAnchorMin() const;

		void SetAnchorMin(const DUOLMath::Vector2& anchor_min);

		DUOLMath::Vector2 GetAnchorMax() const;

		void SetAnchorMax(const DUOLMath::Vector2& anchor_max);

		void SetPivot(const DUOLMath::Vector2& pivot);

		DUOLMath::Vector4 GetRect() const;

		void SetRect(const DUOLMath::Vector4& rect);

	private:
		DUOLMath::Vector2 _anchorMin;

		DUOLMath::Vector2 _anchorMax;

		DUOLMath::Vector2 _pivot;

		// anchor�� ���� ���� ����Ǵ� ���� �ٸ��ϴ�. {posx, posy, width, height} or {left, top, right, bottom}.
		// anchor MinMax�� ���� 0�϶��� �����Դϴ�. ��ȯ�� ��������� �̷������ �Ѵ�... ��.... 
		DUOLMath::Vector4 _rect;
		// �����̼� xyz.. ���� ������� �ʽ��ϴ�.
		DUOLMath::Vector3 _rotation; 
		// ������ xyz.. ���� ������� �ʽ��ϴ�.
		DUOLMath::Vector3 _scale;

	};
}
