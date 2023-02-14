#include "DUOLGameEngine/Manager/UnityMigrator/UnityMigrator.h"
#include "DUOLGameEngine/Manager/SceneManagement/Scene.h"

#include <filesystem>

#include "DUOLGameEngine/ECS/GameObject.h"

namespace DUOLGameEngine
{
	UnityMigrator::UnityMigrator() :
		_yamlNodeList({})
		, _fileIDList({})
	{
		
	}

	UnityMigrator::~UnityMigrator()
	{
	}

	std::shared_ptr<DUOLGameEngine::Scene> UnityMigrator::MigrateUnitySceneFile(const DUOLCommon::tstring& filePath)
	{
		std::filesystem::path p(DUOLCommon::StringHelper::ToString(filePath));

		// 파일 이름을 씬 이름으로 지정합니다.
		DUOLGameEngine::Scene* scene = new Scene(DUOLCommon::StringHelper::ToTString(p.filename().c_str()));

		std::string stringPath = DUOLCommon::StringHelper::ToString(filePath);

		_yamlNodeList = YAML::LoadAllFromFile(stringPath);

		// 파일 아이디를 읽어서 저장한다.
		FILE* file;

		fopen_s(&file, stringPath.c_str(), "r");

		while (!feof(file))
		{
			fgets(_nLine, 256, file);

			int ch_Index = 0;

			// --- 로 시작하는 경우에 대해서 진행합니다.
			if (_nLine[ch_Index] == '-')
			{
				// &가 나올 때까지 문자 포인터를 이동한다.
				while (_nLine[ch_Index] != '&')
				{
					ch_Index++;
				}

				// 숫자를 읽어오기 위해 간다.
				ch_Index++;

				DUOLCommon::tstring fileID = TEXT("");

				// 숫자들을 모두 읽어 오브젝트 아이디를 만든다.
				while (_nLine[ch_Index] >= '0' && _nLine[ch_Index] <= '9')
				{
					fileID += _nLine[ch_Index];
					ch_Index++;
				}

				_fileIDList.emplace_back(fileID);
			}
		}

		for (int i = 0 ; i < _yamlNodeList.size() ; i++)
		{
			DUOLGameEngine::GameObject* newGO = scene->CreateEmpty();

			YAML::Node goNode = _yamlNodeList[i]["GameObject"];

			newGO->SetName(DUOLCommon::StringHelper::ToTString(goNode["m_Name"].as<std::string>()));

			newGO->SetTag(DUOLCommon::StringHelper::ToTString(goNode["m_TagString"].as<std::string>()));

			newGO->SetLayer(goNode["m_Layer"].as<int>());
		}













		_yamlNodeList.clear();

		_fileIDList.clear();

		return std::shared_ptr<DUOLGameEngine::Scene>(scene);
	}
}
