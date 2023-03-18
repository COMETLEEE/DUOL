#pragma once
#include "DUOLEditor/UI/Panels/PanelWindow.h"

#include <filesystem>

namespace DUOLEditor
{
	/**
	 * \brief Content's Browsing object.
	 */
	class ContentsBrowser : public DUOLEditor::PanelWindow
	{
	public:
		ContentsBrowser(const DUOLCommon::tstring panelName, bool isOpened, PanelWindowSetting panelSettings);

		virtual ~ContentsBrowser() override;

		std::filesystem::path _rootDirectory;

		std::filesystem::path _currentDirectory;

		void* _directoryIcon;

		void* _fileIcon;

	private:
		/**
		 * \brief ������ �������� �����ϴ� ������ �ٽ� �׸��ϴ�. Current Directory �� �ٲ� �� ȣ��˴ϴ�.
		 */
		void Refresh();
	};
}