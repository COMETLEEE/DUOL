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

namespace DUOLGameEngine
{
	class GameObject;

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
        
	public:
        void SetInspectedGameObject(DUOLGameEngine::GameObject* gameObject);

        void UnsetInspectedGameObject();
	};
}
