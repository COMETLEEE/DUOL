#include "DUOLGameEngine/Manager/NavigationManager.h"

#include "DUOLGameEngine/ECS/GameObject.h"

#include "DUOLGameEngine/ECS/Component/MeshFilter.h"
#include "DUOLGameEngine/ECS/Component/MeshRenderer.h"
#include "DUOLGameEngine/ECS/Component/NavMeshAgent.h"


#include "DUOLGraphicsEngine/ResourceManager/Resource/Mesh.h"

namespace DUOLGameEngine
{
	NavigationManager::NavigationManager()
	{
		
	}

	NavigationManager::~NavigationManager()
	{
		
	}

	void NavigationManager::InitializeNavigationGameObject(DUOLGameEngine::GameObject* gameObject, bool recursively)
	{
		DUOLGameEngine::NavMeshAgent* navMeshAgent = gameObject->GetComponent<DUOLGameEngine::NavMeshAgent>();

		// NavMeshAgent Component가 있다. + enabled 이다. => 에이전트의 생성이 필요하다. 
		if ((navMeshAgent != nullptr) && (navMeshAgent->GetIsEnabled()))
		{
			AddAgent(navMeshAgent);
		}

		// 바로 아래의 자식 오브젝트들에 대해서 또한 실시하여줍니다.
		if (recursively)
		{
			DUOLGameEngine::Transform* transform = gameObject->GetTransform();

			auto& children = transform->GetChildGameObjects();

			for (auto& child : children)
				InitializeNavigationGameObject(child, true);
		}
	}

	void NavigationManager::InitializeCurrentGameScene(const std::list<std::shared_ptr<DUOLGameEngine::GameObject>>& rootObjectsInScene)
	{
		// 기존에 있던 Crowd 내 모든 에이전트들을 제거합니다.
		int agentsCount = _crowd->getAgentCount();

		for (int i = 0 ; i < agentsCount ; i++)
			_crowd->removeAgent(i);

		// NavMeshAgent Component가 부착된 게임 오브젝트에 대해 Crowd를 생성하고 셋팅합니다.
		for (auto rootObject : rootObjectsInScene)
			InitializeNavigationGameObject(rootObject.get(), true);
	}

	void NavigationManager::Initialize()
	{
		_navMeshQuery = dtAllocNavMeshQuery();

		_crowd = dtAllocCrowd();
	}

	void NavigationManager::UnInitialize()
	{
		for (auto& navMesh : _navMeshes)
			dtFreeNavMesh(navMesh.second);

		_navMeshes.clear();

		dtFreeCrowd(_crowd);

		dtFreeNavMeshQuery(_navMeshQuery);
	}

	void NavigationManager::BakeNavMeshData(DUOLGameEngine::GameObject* gameObj)
	{
		auto gameObjects = gameObj->GetTransform()->GetAllChildGameObjects();

		gameObjects.push_back(gameObj);

		// 버텍스들
		// std::vector<>

		// 인덱스들
		// std::vector<>

		// 모든 게임 오브젝트에 대해서 가지고 있는 메쉬가 있으면 (스태틱 메쉬만 해당한다.) 버텍스를 기록합니다.
		for (auto& gameObject : gameObjects)
		{
			auto meshFilter = gameObject->GetComponent<DUOLGameEngine::MeshFilter>();

			auto priMesh =	meshFilter->GetMesh()->GetPrimitiveMesh();

			if ((priMesh != nullptr) && (priMesh->GetMeshType() == DUOLGraphicsEngine::MeshBase::MeshType::Mesh))
			{
				auto staticMesh = reinterpret_cast<DUOLGraphicsEngine::Mesh*>(priMesh);

				auto& vertices = staticMesh->_vertices;

				// 서브 메쉬와 인덱스에 대하여 ..!
				for (int i = 0 ; i < priMesh->GetSubMeshCount() ; i++)
				{
					
				}
			}
		}

	}

	void NavigationManager::LoadNavMeshData(const DUOLCommon::tstring& fileName)
	{
		if (_navMeshes.contains(fileName))
			return;

		const dtNavMesh* navMesh = LoadNavMeshFile(fileName);

		// 사용 중인 DUOL FBX Loading 의 셋팅에 맞춰서 폴리들의 회전 + 인덱스 순서 변경
		const auto pitch = -90.0f * DUOLMath::PI / 180.0f;

		const auto yaw = 180.0f * DUOLMath::PI / 180.0f;

		DUOLMath::Quaternion q = Quaternion::CreateFromYawPitchRoll(yaw, pitch, 0.f);

		DUOLMath::Matrix rotMatrix = DUOLMath::Matrix::Identity;

		rotMatrix *= DUOLMath::Matrix::CreateFromQuaternion(q);

		for (int i = 0; i < navMesh->getMaxTiles(); ++i)
		{
			const dtMeshTile* tile = navMesh->getTile(i);
			if (!tile->header) continue;

			for (int j = 0; j < tile->header->polyCount; ++j)
			{
				const dtPoly* poly = &tile->polys[j];

				if (poly->getType() == DT_POLYTYPE_OFFMESH_CONNECTION)
					continue;

				for (int k = 0; k < poly->vertCount; ++k)
				{
					// TODO :  돌리지 말아보자. 돌리지 말고 모든 결과를 낸 후에 돌리는거다 ..!
					float* v = &tile->verts[poly->verts[k] * 3];

					/*DirectX::XMFLOAT3 vert = { v[0], v[2], v[1] };
					DirectX::XMVECTOR vec = DirectX::XMLoadFloat3(&vert);
					vec = DirectX::XMVector3Transform(vec, rotMatrix);
					DirectX::XMStoreFloat3(&vert, vec);*/
				}
			}
		}
	}

	void NavigationManager::Update(float deltaTime)
	{
		if (_currentNavMesh == nullptr || _crowd == nullptr)
			return;

		_crowd->update(deltaTime, nullptr);
	}

	dtNavMesh* NavigationManager::LoadNavMeshFile(const DUOLCommon::tstring& fileName)
	{
		// 네비게이션 메쉬 파일이 있는 경로는 정해져있다.
		std::string path = "Asset/NavMesh/" + DUOLCommon::StringHelper::ToString(fileName);

		FILE* fp;

		fopen_s(&fp, path.c_str(), "rb");

		if (!fp) return 0;

		// Read header.
		NavMeshSetHeader header;
		size_t readLen = fread(&header, sizeof(NavMeshSetHeader), 1, fp);
		if (readLen != 1)
		{
			fclose(fp);
			return 0;
		}
		if (header.magic != NAVMESHSET_MAGIC)
		{
			fclose(fp);
			return 0;
		}
		if (header.version != NAVMESHSET_VERSION)
		{
			fclose(fp);
			return 0;
		}

		dtNavMesh* mesh = dtAllocNavMesh();
		if (!mesh)
		{
			fclose(fp);
			return 0;
		}
		dtStatus status = mesh->init(&header.params);
		if (dtStatusFailed(status))
		{
			fclose(fp);
			return 0;
		}

		// Read tiles.
		for (int i = 0; i < header.numTiles; ++i)
		{
			NavMeshTileHeader tileHeader;
			readLen = fread(&tileHeader, sizeof(tileHeader), 1, fp);
			if (readLen != 1)
			{
				fclose(fp);
				return 0;
			}

			if (!tileHeader.tileRef || !tileHeader.dataSize)
				break;

			unsigned char* data = (unsigned char*)dtAlloc(tileHeader.dataSize, DT_ALLOC_PERM);
			if (!data) break;
			memset(data, 0, tileHeader.dataSize);
			readLen = fread(data, tileHeader.dataSize, 1, fp);
			if (readLen != 1)
			{
				dtFree(data);
				fclose(fp);
				return 0;
			}

			mesh->addTile(data, tileHeader.dataSize, DT_TILE_FREE_DATA, tileHeader.tileRef, 0);
		}

		fclose(fp);

		_navMeshes.insert({ fileName, mesh });

		return mesh;
	}

	dtNavMesh* NavigationManager::GetCurrentNavMesh() const
	{
		return _currentNavMesh;
	}

	void NavigationManager::SetCurrentNavMesh(const DUOLCommon::tstring& fileName)
	{
		if (_navMeshes.contains(fileName))
		{
			_currentNavMeshFileName = fileName;

			_currentNavMesh = _navMeshes.at(fileName);

			_navMeshQuery->init(_currentNavMesh, 2048);

			_crowd->init(MAX_AGENTS, MAX_AGENT_RADIUS, _currentNavMesh);
		}
		else if (fileName.empty())
		{
			_currentNavMesh = nullptr;
		}
		// Empty는 아니다 ..! 그럼 로드하자.
		else
		{
			LoadNavMeshData(TEXT("Model_Test_C_Pivot_INDE.bin"));

			_currentNavMesh = _navMeshes.at(fileName);

			_navMeshQuery->init(_currentNavMesh, 2048);

			_crowd->init(MAX_AGENTS, MAX_AGENT_RADIUS, _currentNavMesh);
		}

		// Nav Mesh Changing Event Invoke.
		DUOLGameEngine::EventManager::GetInstance()->InvokeEvent(TEXT("NavMeshChanging"));
	}

	dtNavMeshQuery* NavigationManager::GetNavMeshQuery() const
	{
		return _navMeshQuery;
	}

	dtCrowd* NavigationManager::GetCrowd() const
	{
		return _crowd;
	}

	void NavigationManager::AddAgent(DUOLGameEngine::NavMeshAgent* navMeshAgent)
	{
		dtCrowdAgentParams ap;
		memset(&ap, 0, sizeof(ap));

		ap.radius = navMeshAgent->_radius;
		ap.height = navMeshAgent->_height;
		ap.maxAcceleration = navMeshAgent->_maxAcceleration;
		ap.maxSpeed = navMeshAgent->_maxSpeed;
		ap.collisionQueryRange = ap.radius * 12.f;
		ap.pathOptimizationRange = ap.radius * 30.0f;

		ap.updateFlags = 0;

		if (navMeshAgent->_anticipateTurns)
			ap.updateFlags |= DT_CROWD_ANTICIPATE_TURNS;

		if (navMeshAgent->_optimizeVisibility)
			ap.updateFlags |= DT_CROWD_OPTIMIZE_VIS;

		if (navMeshAgent->_optimizeTopology)
			ap.updateFlags |= DT_CROWD_OPTIMIZE_TOPO;

		if (navMeshAgent->_obstacleAvoidance)
			ap.updateFlags |= DT_CROWD_OBSTACLE_AVOIDANCE;

		if (navMeshAgent->_separation)
			ap.updateFlags |= DT_CROWD_SEPARATION;

		ap.obstacleAvoidanceType = static_cast<unsigned char>(navMeshAgent->_obstacleAvoidanceType);
		ap.separationWeight = navMeshAgent->_separationWeight;
		const DUOLMath::Vector3& genPos = navMeshAgent->GetTransform()->GetWorldPosition();

		DUOLMath::Vector3 convertedGenPos = navMeshAgent->ConvertForFBXBinaryExporter(genPos.x, genPos.y, genPos.z);

		int idx = _crowd->addAgent(reinterpret_cast<const float*>(&convertedGenPos), &ap);

		// 생성이 완료되었습니다.
		if (idx != -1)
		{
			navMeshAgent->_primitiveAgent = _crowd->getAgent(idx);

			navMeshAgent->_primitiveAgentIndex = idx;

			// TODO : CrowdToolState 참조하시오.
			// Target Ref ..?

			// Target Pos ..?
		}
	}

	void NavigationManager::RemoveAgent(int index)
	{
		_crowd->removeAgent(index);
	}

	void NavigationManager::UpdateAgentParameters(DUOLGameEngine::NavMeshAgent* navMeshAgent, int agentIndex)
	{
		dtCrowdAgentParams ap;
		memset(&ap, 0, sizeof(ap));

		ap.radius = navMeshAgent->_radius;
		ap.height = navMeshAgent->_height;
		ap.maxAcceleration = navMeshAgent->_maxAcceleration;
		ap.maxSpeed = navMeshAgent->_maxSpeed;
		ap.collisionQueryRange = ap.radius * 12.f;
		ap.pathOptimizationRange = ap.radius * 30.0f;

		ap.updateFlags = 0;

		if (navMeshAgent->_anticipateTurns)
			ap.updateFlags |= DT_CROWD_ANTICIPATE_TURNS;

		if (navMeshAgent->_optimizeVisibility)
			ap.updateFlags |= DT_CROWD_OPTIMIZE_VIS;

		if (navMeshAgent->_optimizeTopology)
			ap.updateFlags |= DT_CROWD_OPTIMIZE_TOPO;

		if (navMeshAgent->_obstacleAvoidance)
			ap.updateFlags |= DT_CROWD_OBSTACLE_AVOIDANCE;

		if (navMeshAgent->_separation)
			ap.updateFlags |= DT_CROWD_SEPARATION;

		ap.obstacleAvoidanceType = static_cast<unsigned char>(navMeshAgent->_obstacleAvoidanceType);
		ap.separationWeight = navMeshAgent->_separationWeight;

		if (agentIndex < _crowd->getAgentCount())
			_crowd->updateAgentParameters(agentIndex, &ap);
	}

	bool NavigationManager::RequestMoveTarget(const DUOLMath::Vector3& targetPosition, DUOLGameEngine::NavMeshAgent* navMeshAgent)
	{
		const dtQueryFilter* filter = _crowd->getFilter(0);

		const float* halfExtents = _crowd->getQueryExtents();
		
		_navMeshQuery->findNearestPoly(reinterpret_cast<const float*>(&targetPosition), halfExtents, filter, &navMeshAgent->_destinationRef, navMeshAgent->_destinationPos);

		if (!navMeshAgent->_primitiveAgent->active)
			return false;

		return _crowd->requestMoveTarget(navMeshAgent->_primitiveAgentIndex, navMeshAgent->_destinationRef, navMeshAgent->_destinationPos);
	}

	bool NavigationManager::RequestMoveVelocity(const DUOLMath::Vector3& targetVelocity,
		DUOLGameEngine::NavMeshAgent* navMeshAgent)
	{
		if (!navMeshAgent->_primitiveAgent->active)
			return false;

		return _crowd->requestMoveVelocity(navMeshAgent->_primitiveAgentIndex, reinterpret_cast<const float*>(&targetVelocity));
	}
}