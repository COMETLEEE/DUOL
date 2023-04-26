#pragma once
#include <rttr/array_range.h>

#include "DUOLEditor/UI/Panels/PanelWindow.h"

namespace DUOLGameEngine
{
	class Scene;
}

namespace rttr
{
	class instance;
	class property;
}

namespace DUOLEditor
{

	class GraphicsSetting : public DUOLEditor::PanelWindow
	{
	public:
		GraphicsSetting(const DUOLCommon::tstring& title, bool isOpened, const DUOLEditor::PanelWindowSetting& panelWindowSetting);

		~GraphicsSetting();

		void DrawUIProperty();

		//For Draw UI
	public:
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

		void DrawEnumeration(DUOLEditor::WidgetGroupBase* rootWidget, rttr::property property, rttr::instance obj, bool title = true);

		void DrawStructure(DUOLEditor::WidgetGroupBase* rootWidget, rttr::property property, rttr::instance obj);

		void DrawFloat(DUOLEditor::WidgetGroupBase* rootWidget, rttr::property rootProperty, rttr::property property, rttr::instance obj);

		void DrawFloat2(DUOLEditor::WidgetGroupBase* rootWidget, rttr::property rootProperty, rttr::property property, rttr::instance obj);

		void DrawFloat3(DUOLEditor::WidgetGroupBase* rootWidget, rttr::property rootProperty, rttr::property property, rttr::instance obj);

		void DrawFloat4(DUOLEditor::WidgetGroupBase* rootWidget, rttr::property rootProperty, rttr::property property, rttr::instance obj);

		void DrawInt(DUOLEditor::WidgetGroupBase* rootWidget, rttr::property rootProperty, rttr::property property, rttr::instance obj);

		void DrawColor3(DUOLEditor::WidgetGroupBase* rootWidget, rttr::property rootProperty, rttr::property property, rttr::instance obj);



		void Draw(DUOLEditor::WidgetGroupBase* rootWidget, const rttr::array_range<rttr::property>& properties, rttr::instance obj) ;

		DUOLGameEngine::Scene* GetCurrentScene() const;

		void SetCurrentScene(DUOLGameEngine::Scene* currentScene);

	private:
		DUOLGameEngine::Scene* _currentScene;

		rttr::variant _var;
	};

}