#include "FileDialogs.h"
#include "StringHelper.h"
#include <commdlg.h>
#include "GameProcess.h"

#include <filesystem>
#include <fstream>
#include <shlobj_core.h>

#include "Export/RenderingData.h"

#include "EffectEditorManager.h"

#include "LogSystem.h"

DUOLCommon::tstring FileDialogs::OpenTextureFile()
{
	//dfx
	return OpenFile("이미지 파일 (*.png *.dds*)\0*.png;*.dds*\0");
}

std::vector<MuscleGrapics::RenderingData_Particle> FileDialogs::LoadParticleFile()
{
	std::vector<MuscleGrapics::RenderingData_Particle> data;

	auto path = OpenFile("파티클 파일 (*.dfx)\0*.dfx;*\0");

	std::ifstream fr(path);

	if (fr.is_open())
	{
		boost::archive::text_iarchive inArchive(fr);

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

PrevVersion::RenderingData_Particle FileDialogs::LoadPreviousVersionParticleFile()
{
	PrevVersion::RenderingData_Particle prevData;

	auto path = OpenFile("파티클 파일 (*.dfx)\0*.dfx;*\0");

	std::ifstream fr(path);

	if (fr.is_open())
	{
		boost::archive::binary_iarchive inArchive(fr);

		inArchive >> prevData;

		fr.close();

		EffectEditorManager::Get().SetSavedPath(path);

		WriteLog("[Log] LoadParticleFile Success!! %s \n", DUOLCommon::StringHelper::ToString(path).c_str());
	}
	else
	{
		WriteLog("[Error] File Open fail!! \n");
	}

	return prevData;
}

void FileDialogs::SaveParticleFile(std::vector<MuscleGrapics::RenderingData_Particle>& data)
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
		boost::archive::text_oarchive outArchive(fw);

		outArchive << data;

		fw.close();


		WriteLog("[Log] SaveParticleFile Success %s \n", DUOLCommon::StringHelper::ToString(EffectEditorManager::Get().GetSavedPath()).c_str());
	}
	else
	{
		WriteLog("[Error] File Open fail!! \n");
	}
}

template<class T>
void SaveLoadTest(DUOLCommon::tstring path, T& data)
{
	std::ofstream test(path);
	if (test.is_open())
	{
		boost::archive::text_oarchive outArchive(test);
		outArchive << data;
		test.close();
	}

	std::ifstream fr(path);
	T loadTest;
	if (fr.is_open())
	{
		boost::archive::text_iarchive inArchive(fr);
		inArchive >> loadTest;
		fr.close();
	}
}

void FileDialogs::SaveAsParticleFile(std::vector<MuscleGrapics::RenderingData_Particle>& data)
{
	WriteLog("[Log] SaveAsParticleFile... \n");

	auto path = SaveFile("파티클 파일 (*.dfx)\0*.dfx;*\0");

	EffectEditorManager::Get().SetSavedPath(path);

	std::ofstream fw(path);

	if (fw.is_open())
	{
		boost::archive::text_oarchive outArchive(fw);

		outArchive << data;

		fw.close();
		WriteLog("[Log] SaveAsParticleFile Success %s \n", DUOLCommon::StringHelper::ToString(EffectEditorManager::Get().GetSavedPath()).c_str());
	}
	else
	{
		WriteLog("[Error] File Open fail!! \n");
	}

	// SaveLoadTest
	//for (auto& iter : data)
	//{
	//	SaveLoadTest<MuscleGrapics::Particle_CommonInfo>(path, iter._commonInfo);
	//	SaveLoadTest<MuscleGrapics::Particle_Emission>(path, iter._emission);
	//	SaveLoadTest<MuscleGrapics::Particle_Shape>(path, iter._shape);
	//	SaveLoadTest<MuscleGrapics::Particle_Velocity_over_Lifetime>(path, iter._velocity_Over_Lifetime);
	//	SaveLoadTest<MuscleGrapics::Particle_Limit_Velocity_Over_Lifetime>(path, iter._limit_Velocity_Over_Lifetime);
	//	SaveLoadTest<MuscleGrapics::Particle_Force_over_LifeTime>(path, iter._force_Over_Lifetime);
	//	SaveLoadTest<MuscleGrapics::Particle_Color_over_Lifetime>(path, iter._color_Over_Lifetime);
	//	SaveLoadTest<MuscleGrapics::Particle_Size_Over_Lifetime>(path, iter._size_Over_Lifetime);
	//	SaveLoadTest<MuscleGrapics::Particle_Rotation_Over_Lifetime>(path, iter._rotation_Over_Lifetime);
	//	SaveLoadTest<MuscleGrapics::Particle_Texture_Sheet_Animation>(path, iter._texture_Sheet_Animaition);
	//	SaveLoadTest<MuscleGrapics::Particle_Noise>(path, iter._noise);
	//	SaveLoadTest<MuscleGrapics::Particle_Collision>(path, iter._collision);
	//	SaveLoadTest<MuscleGrapics::Particle_Trails>(path, iter._trails);
	//	SaveLoadTest<MuscleGrapics::Particle_Renderer>(path, iter._renderer);
	//}


	//int a = 0;

}

void FileDialogs::UpdateAllFileToLatestVersionInFolder()
{
	using std::filesystem::directory_iterator;

	auto directory_path = OpenFolder();

	if (directory_path != TEXT(""))
		WriteLog("[Log] Open Try : %s \n", directory_path);
	else
	{
		WriteLog("[Log] Open Try : fail.. \n", directory_path);
		return;
	}

	std::function<void(PrevVersion::RenderingData_Particle&, std::vector<MuscleGrapics::RenderingData_Particle>&)>
		convertFunc = [&](PrevVersion::RenderingData_Particle& prevData, std::vector<MuscleGrapics::RenderingData_Particle>& datas)
	{
		MuscleGrapics::RenderingData_Particle newData = prevData;

		for (auto& iter : prevData._childrens)
		{
			convertFunc(iter, datas);

			newData._childrenIDs.push_back(datas.size() - 1);
		}
		datas.push_back(newData);
	};

	for (auto const& entry : directory_iterator(directory_path))
	{
		if (entry.path().extension() == ".dfx" || entry.path().extension() == ".DFX")
		{
			PrevVersion::RenderingData_Particle prevData;

			std::ifstream fr(entry.path());

			if (fr.is_open())
			{
				boost::archive::binary_iarchive inArchive(fr);

				inArchive >> prevData;

				fr.close();

				WriteLog("[Log] LoadPreviousParticleFile Success!! \n");
			}
			else
			{
				WriteLog("[Error] File Open fail!! \n");
			}

			std::vector<MuscleGrapics::RenderingData_Particle> datas;

			convertFunc(prevData, datas);

			std::ofstream fw(entry.path());

			if (fw.is_open())
			{
				boost::archive::text_oarchive outArchive(fw);

				outArchive << datas;

				fw.close();
				WriteLog("[Log] SaveParticleFile Success %s \n", DUOLCommon::StringHelper::ToString(EffectEditorManager::Get().GetSavedPath()).c_str());
			}
			else
			{
				WriteLog("[Error] File Open fail!! \n");
			}

		}

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

DUOLCommon::tstring FileDialogs::OpenFolder()
{

	BROWSEINFO bi = { 0 };
	bi.lpszTitle = _T("Select a folder");
	LPITEMIDLIST pidl = SHBrowseForFolder(&bi);
	TCHAR path[MAX_PATH];

	if (pidl != NULL)
	{
		// PIDL에서 폴더 경로 문자열 가져오기
		SHGetPathFromIDList(pidl, path);

		// 메모리 해제
		IMalloc* pMalloc;
		if (SUCCEEDED(SHGetMalloc(&pMalloc)))
		{
			pMalloc->Free(pidl);
			pMalloc->Release();
		}

		return DUOLCommon::tstring(path);
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
