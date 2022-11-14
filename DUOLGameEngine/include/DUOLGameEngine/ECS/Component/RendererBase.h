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
    class Material;
}

namespace DUOLGameEngine
{
	/**
	 * \brief Renderer 관련 컴포넌트들의 기본 클래스.
	 */
	class RendererBase : public DUOLGameEngine::BehaviourBase
	{
	public:
        RendererBase(const std::weak_ptr<DUOLGameEngine::GameObject>& owner, const DUOLCommon::tstring& name = TEXT("RendererBase"));

        virtual ~RendererBase() override;

	protected:
        /**
         * \brief Render 요청할 때 사용하는 구조체
         */
        DUOLGraphicsEngine::Transform _transformInfo;

        /**
         * \brief Graphics Engine에 제출하기 위한 Material의 배열
         */
        std::vector<DUOLGraphicsEngine::Material*> _primitiveMaterials;

        /**
         * \brief Game engine에서 관리되는 object material instanced vector.
         */
        std::vector<std::shared_ptr<DUOLGameEngine::Material>> _materials;

	public:
        const std::vector<std::shared_ptr<DUOLGameEngine::Material>>& GetMaterials() { return _materials; }

        void AddMaterial(const std::shared_ptr<DUOLGameEngine::Material>& material);

	public:
        virtual void Render() abstract;
	};
}