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
		 * \brief 컨텐츠 브라우저를 구성하는 에셋을 다시 그립니다. Current Directory 가 바뀔 때 호출됩니다.
		 */
		void Refresh();
	};
}