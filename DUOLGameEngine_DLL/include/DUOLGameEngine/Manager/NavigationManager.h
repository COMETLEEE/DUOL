#pragma once
#include <unordered_map>

#include "DUOLCommon/Event/Event.h"
#include "DUOLGameEngine/Navigation/DebugImplement/NavDebugDrawDUOL.h"
#include "DUOLGameEngine/Navigation/Detour/DetourNavMesh.h"
#include "DUOLGameEngine/Navigation/Detour/DetourNavMeshQuery.h"
#include "DUOLGameEngine/Navigation/DetourCrowd/DetourCrowd.h"
#include "DUOLGameEngine/Navigation/Detour/DetourCommon.h"

#include "DUOLGameEngine/Util/SingletonBase.h"
#include "DUOLMath/DUOLMath.h"

namespace DUOLGameEngine
{
	class NavMeshAgent;
	class GameObject;
}

namespace DUOLGameEngine
{
	static const int MAX_AGENTS = 128;

	static const float MAX_AGENT_RADIUS = 5.f;

	static const int NAVMESHSET_MAGIC = 'M' << 24 | 'S' << 16 | 'E' << 8 | 'T'; //'MSET';

	static const int NAVMESHSET_VERSION = 1;

	struct NavMeshSetHeader
	{
		int magic;
		int version;
		int numTiles;
		dtNavMeshParams params;
	};

	struct NavMeshTileHeader
	{
		dtTileRef tileRef;
		int dataSize;
	};

	/**
	 * \brief 네비게이션 메쉬의 베이킹, 로드, 셋팅 등을 담당하는 매니저
	 */
	class DUOL_GAMEENGINE_API NavigationManager : public SingletonBase<NavigationManager>
	{
		DECLARE_SINGLETON(NavigationManager)

		DELETE_COPY_MOVE(NavigationManager)

	public:
		virtual ~NavigationManager() override;

	private:
		void InitializeNavigationGameObject(DUOLGameEngine::GameObject* gameObject, bool recursively = true);

		void InitializeCurrentGameScene(const std::list<std::shared_ptr<DUOLGameEngine::GameObject>>& rootObjectsInScene);

	private:
		/**
		 * \brief Navigation Mesh List.
		 */
		std::unordered_map<DUOLCommon::tstring, dtNavMesh*> _navMeshes;

		dtNavMesh* _currentNavMesh;

		DUOLCommon::tstring _currentNavMeshFileName;

		dtNavMeshQuery* _navMeshQuery;

		dtCrowd* _crowd;

	private:
		dtNavMesh* LoadNavMeshFile(const DUOLCommon::tstring& fileName);

		dtNavMesh* GetCurrentNavMesh() const;

		void SetCurrentNavMesh(const DUOLCommon::tstring& fileName);

		dtNavMeshQuery* GetNavMeshQuery() const;

		dtCrowd* GetCrowd() const;

		void AddAgent(DUOLGameEngine::NavMeshAgent* navMeshAgent);

		void RemoveAgent(int index);

		bool RequestMoveTarget(const DUOLMath::Vector3& targetPosition, DUOLGameEngine::NavMeshAgent* navMeshAgent);

	public:
		void Initialize();

		void UnInitialize();

		/**
		 * \brief 네비게이션 설정에 따라서 네비게이션 메쉬를 만듭니다.
		 * \param gameObj 해당 오브젝트 아래의 모든 메쉬들을 참조해서 굽습니다.
		 */
		void BakeNavMeshData(DUOLGameEngine::GameObject* gameObj);

		/**
		 * \brief 이미 구워져 있는 네비게이션 메쉬를 로드합니다.
		 * \param filePath 해당 네비게이션 메쉬 파일 경로입니다.
		 */
		void LoadNavMeshData(const DUOLCommon::tstring& fileName);

		/**
		 * \brief 네비게이션 시스템을 갱신합니다.
		 * \param deltaTime 
		 */
		void Update(float deltaTime);

#pragma region FRIEND_CLASS
		friend class SceneManager;

		friend class NavMeshAgent;

		friend class DebugManager;
#pragma endregion
	};
}
