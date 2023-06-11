/**

	@file      MeshRenderer.h
	@brief     Static Mesh, 즉, 애니메이션과 연동되지 않은 메쉬의 렌더 콜을 담당하는 컴포넌트
	@details   ~
	@author    COMETLEE
	@date      7.11.2022
	@copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include "DUOLGameEngine/ECS/Component/RendererBase.h"

#include "DUOLCommon/Event/Event.h"

namespace DUOLGameEngine
{
	class MeshFilter;
}

namespace DUOLGameEngine
{
	class DUOL_GAMEENGINE_API MeshRenderer : public DUOLGameEngine::RendererBase
	{
	public:
		MeshRenderer();

		MeshRenderer(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name = TEXT("MeshRenderer"));

		virtual ~MeshRenderer() override;

	private:
		DUOLGraphicsEngine::RenderObject _renderObjectInfo;

		DUOLGraphicsEngine::MeshInfo _meshInfo;

		DUOLGameEngine::MeshFilter* _meshFilter;

		DUOLCommon::EventListenerID _renderEventHandlerIDForGraphics;

	public:
		virtual void OnEnable() override;

		virtual void Render() override;

		void SetOccluder(bool value) { _meshInfo.SetIsOccluder(value); }

		bool IsOccluder() { return _meshInfo.GetIsOccluder(); }

		void SetScreenSpaceReflection(bool value) { _meshInfo.SetScreenSpaceReflection(value); }

		bool IsEnableScreenSpaceReflection() { return _meshInfo.IsEnableScreenSpaceReflection(); }

		float GetRimPower();

		DUOLMath::Vector3 GetRimColor();

		void SetRimPower(float value);

		void SetRimColor(DUOLMath::Vector3 albedo);

		void SetPaperBurnColor(const DUOLMath::Vector4& firstColor, const DUOLMath::Vector4& secondColor);

		void SetOffset(float offset);

		virtual void SetSurfaceType(SurfaceType surfaceType) override;

		virtual SurfaceType GetSurfaceType() override;

		RTTR_ENABLE(DUOLGameEngine::RendererBase)

#pragma region FRIEND_CLASS
			friend class MeshFilter;

		// 스태틱 메쉬 오브젝트 공간 분할용.
		friend class Octree;

		friend class Quadtree;
#pragma endregion
	};
}