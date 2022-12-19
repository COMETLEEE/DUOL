#include "FileDialogs.h"
#include "StringHelper.h"
#include <commdlg.h>
#include "GameProcess.h"

#include <fstream>
#include <boost/serialization/serialization.hpp>
#include <boost/serialization/string.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

#include "Export/RenderingData.h"

#include "EffectEditorManager.h"

#include "LogSystem.h"

DUOLCommon::tstring FileDialogs::OpenTextureFile()
{
	//dfx
	return OpenFile("이미지 파일 (*.png *.dds*)\0*.png;*.dds*\0");
}

MuscleGrapics::RenderingData_Particle FileDialogs::LoadParticleFile()
{
	MuscleGrapics::RenderingData_Particle data;
	auto path = OpenFile("파티클 파일 (*.dfx)\0*.dfx;*\0");

	std::ifstream fr(path);

	if (fr.is_open())
	{
		boost::archive::binary_iarchive inArchive(fr);

		inArchive >> data;

		fr.close();

		EffectEditorManager::Get().SetSavedPath(path);

		WriteLog("[Log] LoadParticleFile Success!! %s \n", DUOLCommon::StringHelper::ToString(path).c_str());
	}
	else
	{
		WriteLog("[Error] File Open fail!! \n");
	}
	return data;
}

void FileDialogs::SaveParticleFile(MuscleGrapics::RenderingData_Particle& data)
{
	WriteLog("[Log] SaveParticleFile... \n");
	if (EffectEditorManager::Get().GetSavedPath() == TEXT(""))
	{
		auto path = SaveFile("파티클 파일 (*.dfx)\0*.dfx;*\0");

		EffectEditorManager::Get().SetSavedPath(path);
	}
	else
	{
		WriteLog("[Log] SavedPath is already exists \n");
	}

	std::ofstream fw(EffectEditorManager::Get().GetSavedPath());

	if (fw.is_open())
	{
		boost::archive::binary_oarchive outArchive(fw);

		outArchive << data;

		fw.close();


		WriteLog("[Log] SaveParticleFile Success %s \n", DUOLCommon::StringHelper::ToString(EffectEditorManager::Get().GetSavedPath()).c_str());
	}
	else
	{
		WriteLog("[Error] File Open fail!! \n");
	}
}

void FileDialogs::SaveAsParticleFile(MuscleGrapics::RenderingData_Particle& data)
{
	WriteLog("[Log] SaveAsParticleFile... \n");

	auto path = SaveFile("파티클 파일 (*.dfx)\0*.dfx;*\0");

	EffectEditorManager::Get().SetSavedPath(path);

	std::ofstream fw(path);

	if (fw.is_open())
	{
		boost::archive::binary_oarchive outArchive(fw);

		outArchive << data;

		fw.close();
		WriteLog("[Log] SaveAsParticleFile Success %s \n", DUOLCommon::StringHelper::ToString(EffectEditorManager::Get().GetSavedPath()).c_str());
	}
	else
	{
		WriteLog("[Error] File Open fail!! \n");
	}
}

DUOLCommon::tstring FileDialogs::OpenFile(const char* filter)
{
	OPENFILENAMEA ofn;
	CHAR szFile[260] = { 0 };
	CHAR currentDir[256] = { 0 };
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = GameProcess::Get()->GetHWND();
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	if (GetCurrentDirectoryA(256, currentDir))
		ofn.lpstrInitialDir = currentDir;
	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;


	if (GetOpenFileNameA(&ofn) == TRUE)
	{
		// EXE 파일 또는 프로젝트 경로까지의 경로를 빼는 코드.
		std::string path = ofn.lpstrInitialDir;

		std::string fullPath = ofn.lpstrFile;

		auto pathSubStr = fullPath.substr(path.size() + 1, fullPath.size());

		return DUOLCommon::StringHelper::ToTString(pathSubStr);
	}


	return DUOLCommon::tstring();
}

DUOLCommon::tstring FileDialogs::SaveFile(const char* filter)
{
	OPENFILENAMEA ofn;
	CHAR szFile[260] = { 0 };
	CHAR currentDir[256] = { 0 };
	ZeroMemory(&ofn, sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = GameProcess::Get()->GetHWND();
	ofn.lpstrFile = szFile;
	ofn.nMaxFile = sizeof(szFile);
	if (GetCurrentDirectoryA(256, currentDir))
		ofn.lpstrInitialDir = currentDir;
	ofn.lpstrFilter = filter;
	ofn.nFilterIndex = 1;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

	// Sets the default extension by extracting it from the filter
	ofn.lpstrDefExt = strchr(filter, '\0') + 1;

	if (GetSaveFileNameA(&ofn) == TRUE)
	{
		// EXE 파일 또는 프로젝트 경로까지의 경로를 빼는 코드.
		std::string path = ofn.lpstrInitialDir;

		std::string fullPath = ofn.lpstrFile;

		auto pathSubStr = fullPath.substr(path.size() + 1, fullPath.size());

		return DUOLCommon::StringHelper::ToTString(pathSubStr);

	}

	return DUOLCommon::tstring();
}
