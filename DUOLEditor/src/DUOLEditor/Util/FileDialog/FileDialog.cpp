#include "DUOLEditor/Util/FileDialog/FileDialog.h"

#include <cderr.h>
#include <filesystem>

namespace DUOLEditor
{
	FileDialog::FileDialog(std::function<int(OPENFILENAME*)> callBack, const DUOLCommon::tstring& dialogName) :
		_callBack(callBack)
		, _dialogName(dialogName)
		, _initializeDirectory(TEXT(""))
	{
	}

	void FileDialog::SetInitializeDirectory(const DUOLCommon::tstring& initializeDirectory)
	{
		_initializeDirectory = initializeDirectory;
	}

	void FileDialog::ShowDialog(ExplorerFlags flags)
	{
		OPENFILENAME ofn;

		if (!_initializeDirectory.empty())
			_filePath = _initializeDirectory;

		_filePath.resize(MAX_PATH);

		ZeroMemory(&ofn, sizeof(ofn));
		ofn.lStructSize = sizeof(ofn);
		ofn.hwndOwner = NULL;
		ofn.lpstrFilter = _filter.c_str();
		ofn.lpstrFile = _filePath.data();
		ofn.nMaxFile = MAX_PATH;
		ofn.lpstrTitle = _dialogName.c_str();
		ofn.nFilterIndex = 1;

		if (!_initializeDirectory.empty())
			ofn.lpstrInitialDir = _initializeDirectory.c_str();

		ofn.Flags = static_cast<DWORD>(flags);

		// 다이얼로그 실행
		_isSucceeded = _callBack(&ofn);

		if (!_isSucceeded)
			HandleError();
		else
			_filePath = _filePath.c_str();

		_fileName = std::filesystem::path(_filePath).stem();
	}

	bool FileDialog::IsSucceeded() const
	{
		return _isSucceeded;
	}

	DUOLCommon::tstring FileDialog::GetSelectedFileName()
	{
		return _fileName;
	}

	DUOLCommon::tstring FileDialog::GetSelectedFilePath()
	{
		return _filePath;
	}

	DUOLCommon::tstring FileDialog::GetErrorInfo()
	{
		return _error;
	}

	bool FileDialog::IsFileExisting() const
	{
		return std::filesystem::exists(_filePath);
	}

	void FileDialog::HandleError()
	{
		switch (CommDlgExtendedError())
		{
			case CDERR_DIALOGFAILURE:	_error = TEXT("CDERR_DIALOGFAILURE");   break;
			case CDERR_FINDRESFAILURE:	_error = TEXT("CDERR_FINDRESFAILURE");  break;
			case CDERR_INITIALIZATION:	_error = TEXT("CDERR_INITIALIZATION");  break;
			case CDERR_LOADRESFAILURE:	_error = TEXT("CDERR_LOADRESFAILURE");  break;
			case CDERR_LOADSTRFAILURE:	_error = TEXT("CDERR_LOADSTRFAILURE");  break;
			case CDERR_LOCKRESFAILURE:	_error = TEXT("CDERR_LOCKRESFAILURE");  break;
			case CDERR_MEMALLOCFAILURE: _error = TEXT("CDERR_MEMALLOCFAILURE"); break;
			case CDERR_MEMLOCKFAILURE:	_error = TEXT("CDERR_MEMLOCKFAILURE");  break;
			case CDERR_NOHINSTANCE:		_error = TEXT("CDERR_NOHINSTANCE");     break;
			case CDERR_NOHOOK:			_error = TEXT("CDERR_NOHOOK");          break;
			case CDERR_NOTEMPLATE:		_error = TEXT("CDERR_NOTEMPLATE");      break;
			case CDERR_STRUCTSIZE:		_error = TEXT("CDERR_STRUCTSIZE");      break;
			case FNERR_BUFFERTOOSMALL:	_error = TEXT("FNERR_BUFFERTOOSMALL");  break;
			case FNERR_INVALIDFILENAME: _error = TEXT("FNERR_INVALIDFILENAME"); break;
			case FNERR_SUBCLASSFAILURE: _error = TEXT("FNERR_SUBCLASSFAILURE"); break;
			default:					_error = TEXT("You cancelled.");
		}
	}
}