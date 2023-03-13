#pragma once
#include "FileDialog.h"

namespace DUOLEditor
{
	class ReadFileDialog : public FileDialog
	{
	public:
		ReadFileDialog(const DUOLCommon::tstring& dialogTitle);

		void AddFileType(const DUOLCommon::tstring& label, const DUOLCommon::tstring& filter);
	};
}