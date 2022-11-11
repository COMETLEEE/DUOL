/**

    @file      RendererBase.h
    @brief     화면 상에 모델을 그리기 위해 필요한 컴포넌트의 기본 클래스
    @details   ~
    @author    COMETLEE
    @date      19.10.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include "DUOLGameEngine/ECS/Component/BehaviourBase.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/RenderObject.h"

namespace DUOLGameEngine
{
	/**
	 * \brief Renderer 관련 컴포넌트들의 기본 클래스
	 */
	class RendererBase : public DUOLGameEngine::BehaviourBase
	{
	public:
        RendererBase(const std::weak_ptr<DUOLGameEngine::GameObject>& owner, const DUOLCommon::tstring& name = TEXT("RendererBase"));

        virtual ~RendererBase() override;

	protected:
		// RenderObject 같은 것 ..?
		// 머터리얼, 라이트 프로브, 라이트 맵, 바운드 등의 변수가 여기 들어갈 수 있다.

        /**
         * \brief Render 요청할 때 사용하는 구조체
         */
        DUOLGraphicsEngine::Transform _transformInfo;

        std::vector<DUOLGraphicsEngine::Material*> _materials;

	public:
        void SetMaterial(const DUOLCommon::tstring& name);

	public:
        virtual void Render() abstract;
	};
}