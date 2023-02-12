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

		// anchor의 값에 따라 적용되는 값이 다릅니다. {posx, posy, width, height} or {left, top, right, bottom}.
		// anchor MinMax의 차가 0일때는 전자입니다. 변환이 자유자재로 이루어져야 한다... 흠.... 
		DUOLMath::Vector4 _rect;
		// 로테이션 xyz.. 현재 적용되지 않습니다.
		DUOLMath::Vector3 _rotation; 
		// 스케일 xyz.. 현재 적용되지 않습니다.
		DUOLMath::Vector3 _scale;

	};
}
