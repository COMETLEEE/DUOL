#pragma once
#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "DUOLGameEngine/ECS/Component/ComponentBase.h"
#include "DUOLGraphicsLibrary/FontEngine/IFontEngine.h"
#include "DUOLMath/DUOLMath.h"

#include "rttr/registration_friend.h"

namespace DUOLGameEngine
{
	/**

		@class   RectTransform
		@brief
		@details ~
		@author  KyungMin Oh
			   -> YeonJu Park

	**/
	class DUOL_GAMEENGINE_API RectTransform final : public std::enable_shared_from_this<RectTransform>, public DUOLGameEngine::Transform
	{
		enum class AnchorPreset
		{
		};
	public:
		RectTransform();

		RectTransform(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name = DUOLCommon::StringHelper::ToTString("RectTransform"));

		virtual ~RectTransform() override;


	public:
		DUOLGraphicsLibrary::Rect CalculateRect(DUOLMath::Vector2 _rectpos);


	public:
		/// <summary>
		///  Getter
		/// </summary>
			inline const DUOLMath::Vector2& GetPivot() { return _pivot; }

			inline DUOLMath::Vector2& GetAnchorMin() { return _anchorMin; }
			
			inline DUOLMath::Vector2& GetAnchorMax() { return _anchorMax; }
			
			inline const DUOLMath::Vector4& GetRect() { return _rect; }

			inline const DUOLGraphicsLibrary::Rect& GetCalculateRect() { return _calculateRect; }

			inline const DUOLMath::Vector3& GetRotation() { return _rotation; }
			
			inline const DUOLMath::Vector3& GetScale() { return _scale; }

			inline const float& GetPosX() { return _rect.x; }

			inline const float& GetPosY() { return _rect.y; }

			inline const float& GetPosZ() { return _posZ; }

			inline const float& GetWidth() { return _rect.z; }

			inline const float& GetHeight() { return _rect.w; }


		public:
			/// <summary>
			/// Setter
			/// </summary>
				void SetAnchorMin(DUOLMath::Vector2& anchor_min);

				void SetAnchorMax(DUOLMath::Vector2& anchor_max);

				void SetPivot(const DUOLMath::Vector2& pivot);

				void SetRect(const DUOLMath::Vector4& rect);

				void SetRotate(const DUOLMath::Vector3& rotate);

				void SetScale(const DUOLMath::Vector3& scale);

				void SetRectX(const float& x);

				void SetRectY(const float& y);

				void SetPosZ(const float& posz);

				void SetRectZ(const float& z);

				void SetRectW(const float& w);

				void Scale(DUOLMath::Vector2& centerpoint);

			private:
				DUOLMath::Vector2 _anchorMin;

				DUOLMath::Vector2 _anchorMax;

				DUOLMath::Vector2 _pivot;

				float _posZ;
				// anchor의 값에 따라 적용되는 값이 다릅니다. {posx, posy, width, height} or {left, top, right, bottom}.
				// anchor MinMax의 차가 0일때는 전자입니다. 변환이 자유자재로 이루어져야 한다... 흠....
				DUOLMath::Vector4 _rect;
				// 로테이션 xyz.. 현재 적용되지 않습니다.
				DUOLMath::Vector3 _rotation;
				// 스케일 xyz.. 현재 적용되지 않습니다.
				DUOLMath::Vector3 _scale;

				DUOLGraphicsLibrary::Rect _calculateRect;

		#pragma region FRIEND_CLASS
				friend class GameObject;

				RTTR_ENABLE(DUOLGameEngine::ComponentBase, DUOLGameEngine::Transform)

				RTTR_REGISTRATION_FRIEND
		#pragma endregion

	};


}
