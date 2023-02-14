#pragma once
#include <vector>

#include "DUOLGameEngine/Util/SingletonBase.h"
#include "yaml-cpp/yaml.h"

namespace DUOLGameEngine
{
	class Scene;
	/**
	 * \brief ����Ƽ ������ �� ������ �о�� DUOLGameEngine �� ���� �����մϴ�.
	 * TODO : �����յ� ����Ʈ�� �� �ֵ��� �մϴ�.
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
		 * \brief ����Ƽ �� ������ ��ü ���� �� ���Ϸ� ��ȯ�մϴ�.
		 * \param filePath file path of unity scene file to migration.
		 * \return ���۵� �� ����
		 */
		std::shared_ptr<DUOLGameEngine::Scene> MigrateUnitySceneFile(const DUOLCommon::tstring& filePath);
	};
}
