/**

    @file      Animator.h
    @brief     Animation system component.
    @details   ~
    @author    COMETLEE
    @date      12.12.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include "DUOLGameEngine/ECS/Component/BehaviourBase.h"
#include "DUOLMath/DUOLMath.h"

namespace DUOLGameEngine
{
	class Avatar;

	class AnimationClip;
}

namespace DUOLGameEngine
{
	struct BoneLocalTransform
	{
		DUOLMath::Vector3 localPosition;

		DUOLMath::Quaternion localRotation;

		DUOLMath::Vector3 localScale;
	};

	/**
	 * \brief Interface to control the Mecanim animation system.
	 */
	class Animator : public DUOLGameEngine::BehaviourBase
	{
	public:
		Animator(const std::weak_ptr<DUOLGameEngine::GameObject>& owner, const DUOLCommon::tstring& name = TEXT("Animator"));

		virtual ~Animator() override;

		/**
		 * \brief TODO : 현재의 애니메이션 클립이 있을 것이 아니라 Animation Controller 가 들어가서 트랜지션, 블렌딩 등을 담당해주
		 */
		DUOLGameEngine::AnimationClip* _currentAnimationClip;

		/**
		 * \brief 사용하는 뼈대
		 */
		DUOLGameEngine::Avatar* _currentAvatar;

	private:
		/**
		 * \brief 현재 프레임입니다. deltaTime과의 관계를 짓기 위해 float로 선언합니다. 
		 */
		float _currentFrame;

		/**
		 * \brief 현재 애니메이션 클립의 최대 프레임입니다.
		 */
		int _maxFrame;

		/**
		 * \brief true == play 중 ..
		 */
		bool _isPlaying;

		/**
		 * \brief Transform matrix update 할 본들의 배열 (일치하지 않으면 폭발함)
		 */
		std::vector<DUOLGameEngine::GameObject*> _boneGameObjects;

		/**
		 * \brief Shader에 직접 넣을 bone matrix pallet.
		 */
		std::vector<DUOLMath::Matrix> _boneMatrixList;

		/**
		 * \brief offset matrix list.
		 */
		std::vector<DUOLMath::Matrix> _boneOffsetMatrixList;

	private:
		/**
		 * \brief Calc Animation Clip's Bone Matrix in current frame.
		 */
		void Play(float deltaTime);

		/**
		 * \brief 애니메이션은 게임 로직이 업데이트된 후 최종적으로 업데이트합니다.
		 * \param deltaTime deltaTime of current frame.
		 */
		void OnLateUpdate(float deltaTime) override;

	public:
		void SetAnimationClip(DUOLGameEngine::AnimationClip* animationClip);

		DUOLGameEngine::AnimationClip* GetCurrentAnimationClip() const;

		void SetBoneGameObjects(const std::vector<DUOLGameEngine::GameObject*>& boneObjects);

		const std::vector<DUOLGameEngine::GameObject*>& GetBoneGameObjects();

		void SetBoneOffsetMatrices(const std::vector<DUOLMath::Matrix>& matrices);

		const std::vector<DUOLMath::Matrix>& GetBoneOffsetMatrices();

		std::vector<DUOLMath::Matrix>* GetBoneMatrices();

		bool GetIsPlaying();

		void SetIsPlaying(bool value);

#pragma region FRIEND_CLASS
		friend class SkinnedMeshRenderer;
#pragma endregion
	};
}