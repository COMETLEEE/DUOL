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
	 * \brief 유니티 엔진의 Scene, AnimatorController, Prefab 정보를 읽어들입니다.
	 * TODO : 프리팹도 임포트할 수 있도록 합니다.
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
		 * \brief 유니티 씬 파일을 자체 포맷 씬 파일로 변환합니다.
		 * \param filePath file path of unity scene file to migration.
		 * \return 제작된 씬
		 */
		std::shared_ptr<DUOLGameEngine::Scene> MigrateUnitySceneFile(const DUOLCommon::tstring& filePath);

		/**
		 * \brief 유니티 애니메이터 컨트롤러 파일을 자체 포맷 애니메이터 컨트롤러 파일로 변환합니다.
		 * \param filePath file path of unity animator controller to migration.
		 * \return 제작된 애니메이터 컨트롤러
		 */
		std::shared_ptr<DUOLGameEngine::AnimatorController> MigrateUnityAnimatorController(const DUOLCommon::tstring& filePath);
	};
}
