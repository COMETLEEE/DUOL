#include "DUOLEditor/Util/FileDialog/ReadFileDialog.h"

namespace DUOLEditor
{
	ReadFileDialog::ReadFileDialog(const DUOLCommon::tstring& dialogTitle) :
		FileDialog(GetOpenFileName, dialogTitle)
	{
	}

	void ReadFileDialog::AddFileType(const DUOLCommon::tstring& label, const DUOLCommon::tstring& filter)
	{
		_filter += label + TEXT('\0') + filter + TEXT('\0');
	}
}