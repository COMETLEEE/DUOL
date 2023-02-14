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

		// ���� �̸��� �� �̸����� �����մϴ�.
		DUOLGameEngine::Scene* scene = new Scene(DUOLCommon::StringHelper::ToTString(p.filename().c_str()));

		std::string stringPath = DUOLCommon::StringHelper::ToString(filePath);

		_yamlNodeList = YAML::LoadAllFromFile(stringPath);

		// ���� ���̵� �о �����Ѵ�.
		FILE* file;

		fopen_s(&file, stringPath.c_str(), "r");

		while (!feof(file))
		{
			fgets(_nLine, 256, file);

			int ch_Index = 0;

			// --- �� �����ϴ� ��쿡 ���ؼ� �����մϴ�.
			if (_nLine[ch_Index] == '-')
			{
				// &�� ���� ������ ���� �����͸� �̵��Ѵ�.
				while (_nLine[ch_Index] != '&')
				{
					ch_Index++;
				}

				// ���ڸ� �о���� ���� ����.
				ch_Index++;

				DUOLCommon::tstring fileID = TEXT("");

				// ���ڵ��� ��� �о� ������Ʈ ���̵� �����.
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
