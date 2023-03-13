#include "DUOLEditor/Util/FileDialog/WriteFileDialog.h"

#include <filesystem>

namespace DUOLEditor
{
	WriteFileDialog::WriteFileDialog(const DUOLCommon::tstring diglogName) :
		FileDialog(GetSaveFileName, diglogName)
	{
	}

	void WriteFileDialog::ShowDialog(ExplorerFlags flags)
	{
		FileDialog::ShowDialog(flags);

		if (_isSucceeded)
			AddExtensionToFilePathAndName();
	}

	void WriteFileDialog::DefineExtension(const DUOLCommon::tstring& label, const DUOLCommon::tstring& extension)
	{
		_filter = label + TEXT('\0') + TEXT('*') + extension + TEXT('\0');
		_extension = extension;
	}

	void WriteFileDialog::AddExtensionToFilePathAndName()
	{
		if (!std::filesystem::path(_fileName).has_extension())
			_fileName += _extension;

		if (!std::filesystem::path(_filePath).has_extension())
			_filePath += _extension;
	}
}