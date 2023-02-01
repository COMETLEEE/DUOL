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
        MeshRenderer(const std::weak_ptr<DUOLGameEngine::GameObject>& owner, const DUOLCommon::tstring& name = TEXT("MeshRenderer"));

        virtual ~MeshRenderer() override;

    private:
        DUOLGraphicsEngine::RenderObject _renderObjectInfo;

        DUOLGraphicsEngine::MeshInfo _meshInfo;

        DUOLGameEngine::MeshFilter* _meshFilter;

        DUOLCommon::EventListenerID _renderEventHandlerIDForGraphics;

	public:
        virtual void Render() override;

        RTTR_ENABLE(DUOLGameEngine::RendererBase)

#pragma region FRIEND_CLASS
        friend class MeshFilter;
#pragma endregion
	};
}