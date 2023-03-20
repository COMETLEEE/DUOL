/**

    @file      Inspector.h
    @brief     GameObject Information panel.
    @details   ~
    @author    COMETLEE
    @date      17.01.2023
    @copyright © COMETLEE, 2023. All right reserved.

**/
#pragma once
#include "DUOLEditor/UI/Panels/PanelWindow.h"

namespace rttr
{
	class instance;
	class property;
}

namespace DUOLGameEngine
{
	class SkinnedMeshRenderer;
	class MeshFilter;
	class Animator;
	class RendererBase;
	class GameObject;

    class ComponentBase;

    class Transform;
}

namespace DUOLEditor
{
    class Container;
}

namespace DUOLEditor
{
	class Inspector : public DUOLEditor::PanelWindow
	{
	public:
        Inspector(const DUOLCommon::tstring& title, bool isOpened, const DUOLEditor::PanelWindowSetting& panelSetting);

        virtual ~Inspector() override;

	private:
        DUOLEditor::Container* _inspectorHeader;

        DUOLEditor::Container* _gameObjectInfo;

        DUOLGameEngine::GameObject* _selectedGameObject;

        DUOLCommon::EventListenerID _selectedGameObjectListenerID;

        void DrawGameObjectInformation();

        void DrawComponentInformation(DUOLGameEngine::ComponentBase* component);

        void DrawAddComponentInformation();

        /**
         * \brief 
         * \param property The property to inspect.
         * \return inspectable => true or false.
         */
        bool IsInspectable(rttr::property property);

        void DrawBool(DUOLEditor::WidgetGroupBase* rootWidget, rttr::property property, rttr::instance obj);

        void DrawFloat(DUOLEditor::WidgetGroupBase* rootWidget, rttr::property property, rttr::instance obj);

        void DrawFloat2(DUOLEditor::WidgetGroupBase* rootWidget, rttr::property property, rttr::instance obj);

        void DrawFloat3(DUOLEditor::WidgetGroupBase* rootWidget, rttr::property property, rttr::instance obj);

        void DrawFloat4(DUOLEditor::WidgetGroupBase* rootWidget, rttr::property property, rttr::instance obj);

        void DrawInt(DUOLEditor::WidgetGroupBase* rootWidget, rttr::property property, rttr::instance obj);

        void DrawString(DUOLEditor::WidgetGroupBase* rootWidget, rttr::property property, rttr::instance obj);

        void DrawColor3(DUOLEditor::WidgetGroupBase* rootWidget, rttr::property property, rttr::instance obj);

        void DrawEnumeration(DUOLEditor::WidgetGroupBase* rootWidget, rttr::property property, rttr::instance obj);

#pragma region SPECIAL_CASE
        // Animator Controller
        void DrawAnimatorController(DUOLEditor::WidgetGroupBase* rootWidget, rttr::property property, rttr::instance obj, DUOLGameEngine::Animator* animator);

        void DrawAllAnimatorControllerInformation();

        // Static Mesh
        void DrawMesh(DUOLEditor::WidgetGroupBase* rootWidget, rttr::property property, rttr::instance obj, DUOLGameEngine::MeshFilter* meshFilter);

        void DrawAllStaticMeshInformation();

        // Skinned Mesh
        void DrawMesh(DUOLEditor::WidgetGroupBase* rootWidget, rttr::property property, rttr::instance obj, DUOLGameEngine::SkinnedMeshRenderer* skinnedMeshRenderer);

        void DrawAllSkinnedMeshInformation();
#pragma endregion

	public:
        void SetInspectedGameObject(DUOLGameEngine::GameObject* gameObject);

        void UnsetInspectedGameObject();

        void SetInspectedSameGameObject();

        void UnsetInspectedSameGameObject();
	};

}
