#pragma once
#include <vector>

#include "DUOLGameEngine/Util/SingletonBase.h"
#include "yaml-cpp/yaml.h"

namespace DUOLGameEngine
{
	class Scene;
	/**
	 * \brief 유니티 엔진의 씬 파일을 읽어와 DUOLGameEngine 의 씬을 구성합니다.
	 * TODO : 프리팹도 임포트할 수 있도록 합니다.
	 */
	class UnityMigrator : public SingletonBase<UnityMigrator>
	{
		DECLARE_SINGLETON(UnityMigrator)

		DELETE_COPY_MOVE(UnityMigrator)

	public:
		virtual ~UnityMigrator();

		char _nLine[256];

		std::vector<YAML::Node> _yamlNodeList;

		std::vector<DUOLCommon::tstring> _fileIDList;

	public:
		/**
		 * \brief 유니티 씬 파일을 자체 포맷 씬 파일로 변환합니다.
		 * \param filePath file path of unity scene file to migration.
		 * \return 제작된 씬 파일
		 */
		std::shared_ptr<DUOLGameEngine::Scene> MigrateUnitySceneFile(const DUOLCommon::tstring& filePath);
	};
}
