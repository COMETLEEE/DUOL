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
	 * \brief �׺���̼� �޽��� ����ŷ, �ε�, ���� ���� ����ϴ� �Ŵ���
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
		 * \brief �׺���̼� ������ ���� �׺���̼� �޽��� ����ϴ�.
		 * \param gameObj �ش� ������Ʈ �Ʒ��� ��� �޽����� �����ؼ� �����ϴ�.
		 */
		void BakeNavMeshData(DUOLGameEngine::GameObject* gameObj);

		/**
		 * \brief �̹� ������ �ִ� �׺���̼� �޽��� �ε��մϴ�.
		 * \param filePath �ش� �׺���̼� �޽� ���� ����Դϴ�.
		 */
		void LoadNavMeshData(const DUOLCommon::tstring& fileName);

		/**
		 * \brief �׺���̼� �ý����� �����մϴ�.
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
