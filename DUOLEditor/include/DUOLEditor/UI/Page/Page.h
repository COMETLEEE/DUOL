/**

    @file      Page.h
    @brief     GUI 기본 컴포넌트 Panels group.
    @details   ~
    @author    COMETLEE
    @date      15.12.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include <list>

#include "DUOLEditor/UI/GUIBase.h"

namespace DUOLEditor
{
	class PanelBase;
}

namespace DUOLEditor
{
	/**
	 * \brief UI rendering에 대해 현재 프레임을 나타냅니다. 패널에 대한 소유권을 가집니다.
	 */
	class Page : public DUOLEditor::GUIBase
	{
	public:
		Page();

		virtual ~Page() override;

		/**
		 * \brief Add panel in page.
		 * \tparam TPanel Panel implemented class.
		 * \tparam Args arguments need to create panel.
		 * \param args arguments need to create panel.
		 * \return Panel added.
		 */
		template <typename TPanel, typename ...Args>
		TPanel* AddPanel(Args&&... args) requires std::derived_from<TPanel, PanelBase>;

		/**
		 * \brief Get panel in page.
		 * \tparam TPanel Panel implemented class.
		 * \return Panel in page.
		 */
		template <typename TPanel>
		TPanel* GetPanel() requires std::derived_from<TPanel, PanelBase>;
		
		/**
		 * \brief Remove Panel in page.
		 * \param panel panel to remove.
		 */
		void RemovePanel(DUOLEditor::PanelBase* panel);

		/**
		 * \brief Remove all panels in page.
		 */
		void RemoveAllPanels();

		/**
		 * \brief Set page as dock space if value is true.
		 * \param value bool
		 */
		void SetIsDockable(bool value);

		/**
		 * \brief Get page is dock space or not.
		 * \return true => dock space / false => not dock space.
		 */
		bool GetIsDockable();

		/**
		 * \brief Draw all enabled panels in page.
		 */
		virtual void Draw() override;

	private:
		bool _isDockable;

		std::list<std::shared_ptr<DUOLEditor::PanelBase>> _panels;
	};

	template <typename TPanel, typename ...Args>
	TPanel* Page::AddPanel(Args&&... args) requires std::derived_from<TPanel, PanelBase>
	{
		_panels.emplace_back(std::make_shared<TPanel>(new TPanel(args...)));

		return _panels.back();
	}

	template <typename TPanel>
	TPanel* Page::GetPanel() requires std::derived_from<TPanel, PanelBase>
	{
		for (auto& panel : _panels)
		{
			if (std::dynamic_pointer_cast<TPanel>(panel) != nullptr)
				return panel.get();
		}

		return nullptr;
	}
}
