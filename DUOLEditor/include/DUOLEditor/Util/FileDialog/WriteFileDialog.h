#pragma once
#include "DUOLEditor/Util/FileDialog/FileDialog.h"

namespace DUOLEditor
{
	/**
	* Dialog window that asks the user to save a file to the disk
	*/
	class WriteFileDialog : public FileDialog
	{
	public:
		WriteFileDialog(const DUOLCommon::tstring diglogName);

		virtual void ShowDialog(ExplorerFlags flags = ExplorerFlags::DONTADDTORECENT | ExplorerFlags::FILEMUSTEXIST | ExplorerFlags::HIDEREADONLY | ExplorerFlags::NOCHANGEDIR) override;

		void DefineExtension(const DUOLCommon::tstring& label, const DUOLCommon::tstring& extension);

	private:
		void AddExtensionToFilePathAndName();

	private:
		DUOLCommon::tstring _extension;
	};
}