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
DUOLCommon::tstring FileDialogs::OpenTextureFile()
{

//dfx
	return OpenFile("이미지 파일 (*.png *.dds*)\0*.png;*.dds*\0");
}

DUOLCommon::tstring FileDialogs::OpenParticleFile(std::shared_ptr<MuscleGrapics::RenderingData_Particle>& data)
{

	std::ifstream fr("test.dat");
	if (fr.is_open())
	{
		boost::archive::binary_iarchive inArchive(fr);

		inArchive >> data;

		//data.show();

		fr.close();
	}
	return TEXT("");
}

DUOLCommon::tstring FileDialogs::SaveParticleFile(std::shared_ptr<MuscleGrapics::RenderingData_Particle>& data)
{
	std::ifstream fr("test.dat");

	if (fr.is_open()) // 파일이 있다면.
	{
		fr.close();
	}
	else // 파일이 없다면.
	{
		auto test = SaveFile("이미지 파일 (*.png *.dds*)\0*.png;*.dds*\0");
	}

	std::ofstream fw("test.dat");

	if (fw.is_open())
	{
		boost::archive::binary_oarchive outArchive(fw);

		outArchive << data;

		fw.close();
	}

	return TEXT("");
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
		return DUOLCommon::StringHelper::ToTString(ofn.lpstrFile);

	return DUOLCommon::tstring();
}
