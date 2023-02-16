#pragma once
#include <vector>

#include "DUOLGameEngine/Util/SingletonBase.h"

#include "yaml-cpp/yaml.h"

namespace DUOLGameEngine
{
	class ObjectBase;
	
	class AnimatorController;

	class Transform;
}
 
namespace DUOLGameEngine
{
	class Scene;
	/**
	 * \brief ����Ƽ ������ Scene, AnimatorController, Prefab ������ �о���Դϴ�.
	 * TODO : �����յ� ����Ʈ�� �� �ֵ��� �մϴ�.
	 */
	class DUOL_GAMEENGINE_API UnityMigrator : public SingletonBase<UnityMigrator>
	{
		DECLARE_SINGLETON(UnityMigrator)

		DELETE_COPY_MOVE(UnityMigrator)

	public:
		virtual ~UnityMigrator();

		char _nLine[256];

		std::vector<YAML::Node> _yamlNodeList;

		std::vector<DUOLCommon::tstring> _fileIDList;

		// FILEID => ObjectBase Map
		std::unordered_map<DUOLCommon::tstring, DUOLGameEngine::ObjectBase*> _fileIDObjectMap;

		// Transform => Children Transform's ID List
		std::list<std::pair<DUOLGameEngine::Transform*, std::vector<DUOLCommon::tstring>>> _childrenTransformIDList;

	public:
		/**
		 * \brief ����Ƽ �� ������ ��ü ���� �� ���Ϸ� ��ȯ�մϴ�.
		 * \param filePath file path of unity scene file to migration.
		 * \return ���۵� ��
		 */
		std::shared_ptr<DUOLGameEngine::Scene> MigrateUnitySceneFile(const DUOLCommon::tstring& filePath);

		/**
		 * \brief ����Ƽ �ִϸ����� ��Ʈ�ѷ� ������ ��ü ���� �ִϸ����� ��Ʈ�ѷ� ���Ϸ� ��ȯ�մϴ�.
		 * \param filePath file path of unity animator controller to migration.
		 * \return ���۵� �ִϸ����� ��Ʈ�ѷ�
		 */
		std::shared_ptr<DUOLGameEngine::AnimatorController> MigrateUnityAnimatorController(const DUOLCommon::tstring& filePath);
	};
}
