/**

    @file      RendererBase.h
    @brief     화면 상에 모델을 그리기 위해 필요한 컴포넌트의 기본 클래스
    @details   ~
    @author    COMETLEE
    @date      19.10.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include "DUOLCommon/Event/Event.h"
#include "DUOLGameEngine/ECS/Component/BehaviourBase.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/RenderObject.h"



namespace DUOLGameEngine
{
    class Material;
    enum class SurfaceType;
}

namespace DUOLGameEngine
{

	/**
	 * \brief Renderer 관련 컴포넌트들의 기본 클래스.
	 */
	class DUOL_GAMEENGINE_API RendererBase : public DUOLGameEngine::BehaviourBase
	{
	public:
        RendererBase();

        RendererBase(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name = TEXT("RendererBase"));

        virtual ~RendererBase() override;

	protected:
        /**
         * \brief 그래픽스 엔진에 Render 요청할 때 사용하는 구조체
         */
        DUOLGraphicsEngine::Transform _transformInfo;

        /**
         * \brief Graphics Engine에 제출하기 위한 Material의 배열
         */
        std::vector<DUOLGraphicsEngine::Material*> _primitiveMaterials;

        /**
         * \brief Game engine에서 관리되는 object material instanced vector.
         */
        std::vector<DUOLGameEngine::Material*> _materials;

        /**
         * \brief event handler id.
         */
        DUOLCommon::EventListenerID _renderEventHandlerIDForGraphics;

        /**
         * \brief show selected material for editor
         */
        unsigned int _currentSelectedMaterial;

	public:
        std::vector<DUOLGameEngine::Material*> GetMaterials();

        void AddMaterial(DUOLGameEngine::Material* material);

        void DeleteBackMaterial();

        void DeleteAllMaterial();
	public:
        virtual void OnEnable() override;

        virtual void OnDisable() override;

        virtual void SetSurfaceType(SurfaceType surfaceType);

        virtual SurfaceType GetSurfaceType();

        virtual void Render();

        void SetCurrentSelectedMaterial(unsigned currentSelectedMaterial);

        void SetAlbedo(DUOLMath::Vector4 albedo);

        void SetMetallic(float value);

        void SetRoughness(float value);

        void SetGPUInstancing(bool value);

        const DUOLCommon::tstring& GetMaterialName();

        DUOLMath::Vector4 GetAlbedo();

        float GetMetallic();

        float GetRoughness();

        bool GetGpuInstancing();

		RTTR_ENABLE(DUOLGameEngine::BehaviourBase)

        RTTR_REGISTRATION_FRIEND
	};

}