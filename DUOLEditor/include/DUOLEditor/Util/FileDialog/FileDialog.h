#pragma once
#include <functional>
#include <windows.h>
#include <commdlg.h>


#include "DUOLCommon/StringHelper.h"

namespace DUOLEditor
{
	enum class ExplorerFlags
	{
		READONLY = 0x00000001,
		OVERWRITEPROMPT = 0x00000002,
		HIDEREADONLY = 0x00000004,
		NOCHANGEDIR = 0x00000008,
		SHOWHELP = 0x00000010,
		ENABLEHOOK = 0x00000020,
		ENABLETEMPLATE = 0x00000040,
		ENABLETEMPLATEHANDLE = 0x00000080,
		NOVALIDATE = 0x00000100,
		ALLOWMULTISELECT = 0x00000200,
		EXTENSIONDIFFERENT = 0x00000400,
		PATHMUSTEXIST = 0x00000800,
		FILEMUSTEXIST = 0x00001000,
		CREATEPROMPT = 0x00002000,
		SHAREAWARE = 0x00004000,
		NOREADONLYRETURN = 0x00008000,
		NOTESTFILECREATE = 0x00010000,
		NONETWORKBUTTON = 0x00020000,
		NOLONGNAMES = 0x00040000,	// force no long names for 4.x modules
		EXPLORER = 0x00080000,	// new look commdlg
		NODEREFERENCELINKS = 0x00100000,
		LONGNAMES = 0x00200000,	// force long names for 3.x modules
		ENABLEINCLUDENOTIFY = 0x00400000,	// send include message to callback
		ENABLESIZING = 0x00800000,
		DONTADDTORECENT = 0x02000000,
		FORCESHOWHIDDEN = 0x10000000	// Show All files including System and hidden files
	};

	inline ExplorerFlags operator~ (ExplorerFlags a) { return (ExplorerFlags)~(int)a; }
	inline ExplorerFlags operator| (ExplorerFlags a, ExplorerFlags b) { return (ExplorerFlags)((int)a | (int)b); }
	inline ExplorerFlags operator& (ExplorerFlags a, ExplorerFlags b) { return (ExplorerFlags)((int)a & (int)b); }
	inline ExplorerFlags operator^ (ExplorerFlags a, ExplorerFlags b) { return (ExplorerFlags)((int)a ^ (int)b); }
	inline ExplorerFlags& operator|= (ExplorerFlags& a, ExplorerFlags b) { return (ExplorerFlags&)((int&)a |= (int)b); }
	inline ExplorerFlags& operator&= (ExplorerFlags& a, ExplorerFlags b) { return (ExplorerFlags&)((int&)a &= (int)b); }
	inline ExplorerFlags& operator^= (ExplorerFlags& a, ExplorerFlags b) { return (ExplorerFlags&)((int&)a ^= (int)b); }

	/**
	 * \brief File dialog
	 */
	class FileDialog
	{
	public:
		FileDialog(std::function<int(OPENFILENAME*)> callBack, const DUOLCommon::tstring& dialogName);

		void SetInitializeDirectory(const DUOLCommon::tstring& initializeDirectory);

		virtual void ShowDialog(ExplorerFlags flags = ExplorerFlags::DONTADDTORECENT | ExplorerFlags::FILEMUSTEXIST | ExplorerFlags::HIDEREADONLY | ExplorerFlags::NOCHANGEDIR);

		bool IsSucceeded() const;

		DUOLCommon::tstring GetSelectedFileName();

		DUOLCommon::tstring GetSelectedFilePath();

		DUOLCommon::tstring GetErrorInfo();

		bool IsFileExisting() const;

	private:
		void HandleError();

	protected:
		std::function<int(OPENFILENAME*)> _callBack;

		DUOLCommon::tstring _error;

		DUOLCommon::tstring _initializeDirectory;

		DUOLCommon::tstring _dialogName;

		DUOLCommon::tstring _fileName;

		DUOLCommon::tstring _filter;

		DUOLCommon::tstring _filePath;

		bool _isSucceeded;
	};
}