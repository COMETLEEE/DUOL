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

        void DrawGameObjectInformation();

        void DrawTransformInformation(DUOLGameEngine::Transform* transform);

        void DrawComponentInformation(DUOLGameEngine::ComponentBase* component);

        /**
         * \brief 해당 프로퍼티가 시리얼라이저블한지 체크합니다.
         * \param property 해당 Component*가 Serializable인지 체크합니다.
         * \return 만약 시리얼라이저블하면 true, 아니라면 false.
         */
        bool IsSerializable(rttr::property property);

        /**
         * \brief 만약 해당 위젯을 'Vector3' 로 그려야 한다면 그립니다.
         * \param rootWidget 해당 'Vector3' 를 그릴 Root Widget
         * \param property 인스펙터에서 Vector3로 표현될 클래스 속성
         * \param obj 해당 인스펙터에 뜰 객체의 참조
         * \return 만약 그릴 수 있다면 true, 아니라면 false.
         */
        bool DrawVector3_If(DUOLEditor::WidgetGroupBase* rootWidget, rttr::property property, rttr::instance obj);

	public:
        void SetInspectedGameObject(DUOLGameEngine::GameObject* gameObject);

        void UnsetInspectedGameObject();
	};
}