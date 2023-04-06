/**

	@file      SkinnedMeshRenderer.h
	@brief     Skinned mesh 를 그리기 위한 컴포넌트. (== Mesh Filter ver.Skinned)
	@details   ~
	@author    COMETLEE
	@date      12.12.2022
	@copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include "DUOLGameEngine/ECS/Component/RendererBase.h"

namespace DUOLGameEngine
{
	class Mesh;
	class Animator;
}

namespace DUOLGameEngine
{
	/**
	 * \brief The Skinned Mesh filter.
	 */
	class DUOL_GAMEENGINE_API SkinnedMeshRenderer final : public DUOLGameEngine::RendererBase
	{
	public:
		SkinnedMeshRenderer();

		SkinnedMeshRenderer(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name = TEXT("SkinnedMeshRenderer"));

		virtual ~SkinnedMeshRenderer() override;

	private:
		DUOLGraphicsEngine::RenderObject _renderObjectInfo;

		DUOLGraphicsEngine::SkinnedMeshInfo _skinnedMeshInfo;

		/**
		 * \brief Skinning animation에 사용하는 Mesh
		 */
		DUOLGameEngine::Mesh* _skinnedMesh;

		/**
		 * \brief 해당 Skinned Mesh가 적용될 본 중 Root !
		 */
		DUOLGameEngine::Transform* _rootBone;

		/**
		* \brief 해당 Skinned Mesh가 적용될 본 중 Root !
		*/
		DUOLGameEngine::Animator* _animator;
	private:
		Animator* SearchAnimatorComponent(DUOLGameEngine::GameObject*);

	public:
		DUOLGameEngine::Mesh* GetSkinnedMesh();

		void SetSkinnedMesh(DUOLGameEngine::Mesh* mesh);

		DUOLGameEngine::Transform* GetRootBone();

		void SetRootBone(DUOLGameEngine::Transform* rootBone);

		virtual void OnStart() override;

		virtual void Render() override;

		RTTR_ENABLE(DUOLGameEngine::RendererBase)

			RTTR_REGISTRATION_FRIEND
	};
}