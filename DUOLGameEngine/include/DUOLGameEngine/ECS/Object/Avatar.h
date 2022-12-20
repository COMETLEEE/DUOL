/**

    @file      Avatar.h
    @brief     Skinned Mesh Rendering 시 Bone 의 하이어라키와 관련된 정보를 가지고 있는 리소스
    @details   ~
    @author    COMETLEE
    @date      19.12.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include "DUOLGameEngine/ECS/ObjectBase.h"

namespace DUOLGraphicsEngine
{
	struct Bone;
}

namespace DUOLGameEngine
{
	class Avatar : public DUOLGameEngine::ObjectBase
	{
	public:
		Avatar(const DUOLCommon::tstring& name = TEXT("Avatar"));

		virtual ~Avatar() override;

	private:
		/**
		 * \brief 로드된 Model 의 Bones.
		 */
		std::vector<DUOLGraphicsEngine::Bone>* _bones;

	private:
		inline const std::vector<DUOLGraphicsEngine::Bone>& GetPrimitiveBones() const { return *_bones; }

		void SetPrimitiveBones(std::vector<DUOLGraphicsEngine::Bone>* bones);

#pragma region FRIEND_CLASS
		friend class ResourceManager;

		friend class Animator;
#pragma endregion
	};
}