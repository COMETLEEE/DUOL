#include "DUOLGameEngine/Manager/SceneManagement/Scene.h"

#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/ECS/Component/MeshRenderer.h"
#include "DUOLGameEngine/ECS/Component/MeshFilter.h"
#include "DUOLGameEngine/ECS/Component/SkinnedMeshRenderer.h"
#include "DUOLGameEngine/ECS/Object/Mesh.h"

#include "DUOLGameEngine/Manager/PhysicsManager.h"
#include "DUOLGameEngine/Manager/ResourceManager.h"
#include "DUOLGameEngine/Manager/TimeManager.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/Mesh.h"

#include <fstream>
#include <boost/archive/binary_iarchive.hpp>

#include "DUOLGameEngine/ECS/Component/Animator.h"
#include "DUOLGameEngine/ECS/Component/ParticleRenderer.h"
#include "DUOLGameEngine/ECS/Component/RectTransform.h"
#include "DUOLGameEngine/ECS/Component/NavMeshAgent.h"
#include "DUOLGameEngine/ECS/Component/Camera.h"
#include "DUOLGameEngine/ECS/Component/Light.h"
#include "DUOLGameEngine/ECS/Object/Material.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/PerlinNoise.h"

#include <rttr/registration>
#include "DUOLCommon/MetaDataType.h"
#include "DUOLGameEngine/Manager/NavigationManager.h"
#include "DUOLGraphicsEngine/ResourceManager/Resource/Material.h"
#include "DUOLGameEngine/ECS/Component/Canvas.h"
#include "DUOLGameEngine/Manager/GraphicsManager.h"
#include "DUOLGameEngine/Manager/UIManager.h"
#include "DUOLGraphicsEngine/GraphicsEngine/GraphicsEngine.h"

#include "DUOLGameEngine/Util/Quadtree.h"
#include "DUOLGameEngine/Util/Octree.h"

using namespace rttr;

RTTR_PLUGIN_REGISTRATION
{
	rttr::registration::class_<DUOLGameEngine::Scene>("Scene")
	.constructor<>()
	(
		// rttr::policy::ctor::as_raw_ptr
	)
	.property("_name", &DUOLGameEngine::Scene::_name)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("_navMeshFileName", &DUOLGameEngine::Scene::_navMeshFileName)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	).property("_graphicsSetting", &DUOLGameEngine::Scene::_graphicsSetting)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	)
	.property("_gameObjectsInScene", &DUOLGameEngine::Scene::_gameObjectsInScene)
	(
		metadata(DUOLCommon::MetaDataType::Serializable, true)
	);
}

namespace DUOLGameEngine
{
	class Canvas;

	int Scene::_canvasCount = 0;

	Scene::Scene(const DUOLCommon::tstring& name) :
		_gameObjectsInScene(std::list<std::shared_ptr<GameObject>>())
		, _rootObjectsInScene(std::list<std::shared_ptr<GameObject>>())
		, _gameObjectsForCreate(std::vector<std::shared_ptr<GameObject>>())
		, _gameObjectsForDestroy(std::list<std::pair<std::shared_ptr<GameObject>, float>>())
		, _gameObjectsForActive(std::list<std::shared_ptr<GameObject>>())
		, _gameObjectsForInActive(std::list<std::shared_ptr<GameObject>>())
		, _name(name)
		, _path(DUOLCommon::StringHelper::ToTString("Empty"))
		, _navMeshFileName(DUOLCommon::tstring())
		, _octree(nullptr)
		, _quadtree(nullptr)
	{

		_gameObjectsForCreate.reserve(1000);

		CreateGraphicsSetting();
	}

	Scene::~Scene()
	{
		/// <summary>
		/// 이걸 밖에 명시적 호출로 뺄까 .. 아니면 그냥 소멸자에서 정리할까
		/// </summary>
		UnInitialize();
	}

	void Scene::UnInitialize()
	{
		for (auto& gameObject : _gameObjectsForCreate)
			gameObject.reset();

		for (auto& gameObject : _gameObjectsForDestroy)
			gameObject.first.reset();

		for (auto& gameObject : _gameObjectsForActive)
			gameObject.reset();

		for (auto& gameObject : _gameObjectsForInActive)
			gameObject.reset();

		for (auto& gameObject : _rootObjectsInScene)
			gameObject.reset();

		for (auto& gameObject : _gameObjectsInScene)
			gameObject.reset();

		_gameObjectsForCreate.clear();

		_gameObjectsForDestroy.clear();

		_gameObjectsForActive.clear();

		_gameObjectsForInActive.clear();

		_gameObjectsInScene.clear();

		_rootObjectsInScene.clear();

		if (_octree != nullptr)
			delete _octree;
	}

	void Scene::Awake()
	{
		// TODO : 씬의 등록 오브젝트 리스트로 옮겨줍니다. 테스트를 위해 쓸모 없는 코드를 놔두었습니다.
		for (auto iter = _gameObjectsInScene.begin(); iter != _gameObjectsInScene.end();)
		{
			if ((*iter)->GetTransform()->IsRootObject())
			{
				_rootObjectsInScene.push_back(*iter);
			}

			iter->get()->_scene = this;

			++iter;
		}

		// TODO : 자체 포맷화가 완료되면 이 구절은 없어져야 합니다. 씬의 등록 오브젝트 리스트로 옮겨줍니다. 계속적인 테스트를 위해 쓸모 없는 코드를 놔둡니다.
		for (auto iter = _gameObjectsForCreate.begin(); iter != _gameObjectsForCreate.end();)
		{
			if ((*iter)->GetTransform()->IsRootObject())
				_rootObjectsInScene.push_back(*iter);

			// 게임 오브젝트의 참조 카운트 유지를 위하여 가지고 있는다 .. 여유 있을 때 리팩토링 갑시다 ..
			_gameObjectsInScene.push_back(*iter);

			++iter;
		}

		// 다 옮겼으니까 리스트 비워주고
		_gameObjectsForCreate.clear();

		for (const auto& rootObject : _rootObjectsInScene)
		{
			// Awake의 경우에는 비활성화 상태의 게임 오브젝트도 실행합니다 ..!
			rootObject->OnAwake();
		}

		// 트리를 빌드한다.
		_octree = DUOLGameEngine::Octree::BuildOctree(this);

		_quadtree = DUOLGameEngine::Quadtree::BuildQuadtree(this);

	}

	void Scene::SetGameObjectList()
	{
		// TODO : 씬의 등록 오브젝트 리스트로 옮겨줍니다. 테스트를 위해 쓸모 없는 코드를 놔두었습니다.
		for (auto iter = _gameObjectsInScene.begin(); iter != _gameObjectsInScene.end();)
		{
			if ((*iter)->GetTransform()->IsRootObject())
			{
				_rootObjectsInScene.push_back(*iter);
			}

			iter->get()->_scene = this;

			++iter;
		}

		// TODO : 자체 포맷화가 완료되면 이 구절은 없어져야 합니다. 씬의 등록 오브젝트 리스트로 옮겨줍니다. 계속적인 테스트를 위해 쓸모 없는 코드를 놔둡니다.
		for (auto iter = _gameObjectsForCreate.begin(); iter != _gameObjectsForCreate.end();)
		{
			if ((*iter)->GetTransform()->IsRootObject())
				_rootObjectsInScene.push_back(*iter);

			// 게임 오브젝트의 참조 카운트 유지를 위하여 가지고 있는다 .. 여유 있을 때 리팩토링 갑시다 ..
			_gameObjectsInScene.push_back(*iter);

			++iter;
		}

		// 다 옮겼으니까 리스트 비워주고
		_gameObjectsForCreate.clear();
	}

	void Scene::BuildStaticGameObjectsTree()
	{
		// 트리를 빌드한다.
		_octree = DUOLGameEngine::Octree::BuildOctree(this);

		_quadtree = DUOLGameEngine::Quadtree::BuildQuadtree(this);
	}

	void Scene::RegisterAllRenderingEvent()
	{
		for (auto gameObject : _gameObjectsInScene)
		{
			auto renderer = gameObject->GetComponent<DUOLGameEngine::RendererBase>();

			auto camera = gameObject->GetComponent<DUOLGameEngine::Camera>();

			auto light = gameObject->GetComponent<DUOLGameEngine::Light>();

			if (renderer != nullptr && renderer->GetIsEnabled())
			{
				renderer->OnEnable();
			}

			if (camera != nullptr)
			{
				camera->OnEnable();
			}

			if (light != nullptr)
			{
				light->OnEnable();
			}
		}
	}

	void Scene::AwakeAllGameObject()
	{
		for (const auto& rootObject : _rootObjectsInScene)
		{
			rootObject->OnAwake();
		}
	}

	void Scene::StartAllGameObject()
	{
		for (const auto& rootObject : _rootObjectsInScene)
		{
			if (rootObject->GetIsActiveSelf())
			{
				rootObject->OnStart();

				rootObject->OnActive();
			}
		}
	}

	void Scene::Start() const
	{
		for (const auto& rootObject : _rootObjectsInScene)
		{
			if (rootObject->GetIsActiveSelf())
			{
				rootObject->OnStart();

				rootObject->OnActive();
			}
		}
	}

	void Scene::Update(float deltaTime)
	{
		for (auto& onUpdate : _onUpdateFunctions)
			onUpdate.second(deltaTime);
	}

	void Scene::InvokeUpdate(float deltaTime) const
	{
		for (auto& onInvokeUpdate : _onInvokeUpdateFunctions)
			onInvokeUpdate.second(deltaTime);
	}

	void Scene::CoroutineUpdate(float deltaTime) const
	{
		for (auto& onCoroutineUpdate : _onCoroutineUpdateFunctions)
			onCoroutineUpdate.second(deltaTime);
	}

	void Scene::LateUpdate(float deltaTime) const
	{
		for (auto& onLateUpdate : _onLateUpdateFunctions)
			onLateUpdate.second(deltaTime);
	}

	void Scene::DestroyComponents(float deltaTime)
	{
		// 하 .. 어떻게할까 ..
		for (const auto& rootObject : _rootObjectsInScene)
		{
			rootObject->UpdateDestroyComponent(deltaTime);
		}
	}

	void Scene::CreateGameObjects()
	{
		for (auto iter = _gameObjectsForCreate.begin(); iter != _gameObjectsForCreate.end();)
		{
			DUOLGameEngine::GameObject* gameObject = iter->get();

			// 물리 오브젝트라면 Physics Manager에 등록까지 ! => 단, 모든 오브젝트들이 여기 들어가 있으므로 recursively 않게 ..
			// 물리 계열 컴포넌트를 추가하거나 만들 때 .. 자동으로 Initialize 됩니다 ..!
			// DUOLGameEngine::PhysicsManager::GetInstance()->InitializePhysicsGameObject((*iter).get(), false);

			// 네비게이션 오브젝트라면 Navigation Manager에 등록까지 ! => 네비게이션 메쉬는
			// 프리미티브 에이전트의 생성과 삭제가 필요해서 .. 컴포넌트 내부의 이벤트 함수에서 초기화합니다.
			// DUOLGameEngine::NavigationManager::GetInstance()->InitializeNavigationGameObject((*iter).get(), false);

			// 루트 오브젝트라면 루트 오브젝트 리스트에 넣어줍니다.
			if (gameObject->GetTransform()->IsRootObject())
				_rootObjectsInScene.push_back(*iter);

			// 참조 카운트 유지를 위하여 가지고 있는다.
			_gameObjectsInScene.push_back(*iter);

			// OnCreate.
			gameObject->OnCreate();

			// Event On.
			_gameObjectCreatedEvent.Invoke(gameObject);

			++iter;
		}

		_gameObjectsForCreate.clear();
	}

	void Scene::DestroyGameObjects()
	{
		// 프레임 간격 시간 with scaled.
		float deltaTime = DUOLGameEngine::TimeManager::GetInstance()->GetDeltaTime();

		// 1. 게임 오브젝트 파괴 예약 시간 업데이트
		for (auto iter = _gameObjectsForDestroy.begin(); iter != _gameObjectsForDestroy.end();)
		{
			iter->second -= deltaTime;

			++iter;
		}

		// 2. 시간이 전부 지나간 게임 오브젝트는 씬 리스트에서 제거합니다.
		for (auto iter = _gameObjectsForDestroy.begin(); iter != _gameObjectsForDestroy.end();)
		{
			// 아직 예약 시간이 남았다면 넘어갑니다.
			if (iter->second > 0.f)
			{
				++iter;

				continue;
			}

			// 지워야하는 오브젝트였는데 만약, 루트 오브젝트라면 리스트에서 지워줍니다.
			if (iter->first->GetTransform()->IsRootObject())
			{
				for (auto iter2 = _rootObjectsInScene.begin(); iter2 != _rootObjectsInScene.end();)
				{
					if (iter->first.get() == iter2->get())
					{
						iter2 = _rootObjectsInScene.erase(iter2);

						break;
					}
					else
						++iter2;
				}
			}

			// 참조 카운트를 없애기 위해서 '_gameObjectsInScene' 에서도 제외해줍니다.
			for (auto iter2 = _gameObjectsInScene.begin(); iter2 != _gameObjectsInScene.end();)
			{
				if (iter->first.get() == iter2->get())
				{
					iter2 = _gameObjectsInScene.erase(iter2);

					break;
				}
				else
					++iter2;
			}

			// 재귀적으로 자식 오브젝트까지 지우는 과정을 수행합니다.
			{
				std::shared_ptr<DUOLGameEngine::GameObject> gameObject = iter->first;

				gameObject->OnDestroy();

				// 물리 시뮬레이션에서 제거
				DUOLGameEngine::PhysicsManager::GetInstance()->UnInitializePhysicsGameObject(gameObject.get(), true);

				// DUOLGameEngine::NavigationManager::GetInstance()->UnIni

				// OnDestroy가 완료된 자식 오브젝트들을 리스트에서 제거합니다. _gameObjectsInScene 에서 제거합니다.
				// 메모리가 해제됩니다.
				auto&& children = gameObject->GetTransform()->GetAllChildGameObjects();

				for (auto child : children)
				{
					for (auto iter2 = _gameObjectsInScene.begin(); iter2 != _gameObjectsInScene.end();)
					{
						if (child == (*iter2).get())
						{
							// 부모 자식 관계 reset
							auto parentTransform = child->GetTransform()->GetParent();

							if (parentTransform != nullptr)
								parentTransform->ResetChild(child->GetTransform());

							// child->GetTransform()->SetParent(nullptr, true, false);

							// Delete event child game object.
							_gameObjectDeletedEvent.Invoke(iter2->get());

							iter2 = _gameObjectsInScene.erase(iter2);

							break;
						}
						else
							++iter2;
					}
				}

				gameObject.reset();
			}

			// Delete event
			_gameObjectDeletedEvent.Invoke(iter->first.get());

			// 부모 자식 관계 reset
			auto parentTransform = iter->first.get()->GetTransform()->GetParent();

			if (parentTransform != nullptr)
				parentTransform->ResetChild(iter->first.get()->GetTransform());

			// iter->first.get()->GetTransform()->SetParent(nullptr, true, false);

			// 참조 카운트 0 !
			iter = _gameObjectsForDestroy.erase(iter);
		}
	}

	void Scene::ActiveGameObjects()
	{
		// 예약된 녀석들을 활성화한다.
		for (auto iter = _gameObjectsForActive.begin(); iter != _gameObjectsForActive.end(); )
		{
			(*iter)->_isActive = true;

			(*iter)->OnActive();

			iter = _gameObjectsForActive.erase(iter);
		}
	}

	void Scene::InActiveGameObjects()
	{
		// 예약된 녀석들을 비활성화한다.
		for (auto iter = _gameObjectsForInActive.begin(); iter != _gameObjectsForInActive.end(); )
		{
			(*iter)->_isActive = false;

			(*iter)->OnInActive();

			iter = _gameObjectsForInActive.erase(iter);
		}
	}

	void Scene::RemoveInRootObjectsList(DUOLGameEngine::GameObject* gameObject)
	{
		std::erase_if(_rootObjectsInScene, [&gameObject](const std::shared_ptr<DUOLGameEngine::GameObject>& item)
			{
				return (gameObject == item.get());
			});
	}

	void Scene::AddInRootObjectsList(DUOLGameEngine::GameObject* gameObject)
	{
		// 아직 생성 중인 오브젝트이면 씬 루트 오브젝트 리스테에 넣지 않습니다 ..!
		for (auto iter = _gameObjectsForCreate.begin(); iter != _gameObjectsForCreate.end();)
		{
			if (iter->get() == gameObject)
				return;
			else
				++iter;
		}

		_rootObjectsInScene.push_back(gameObject->shared_from_this());
	}

	void Scene::AddDontDestroyOnLoadGameObjects(DUOLGameEngine::Scene* prevScene)
	{
		std::for_each(prevScene->_gameObjectsInScene.begin(), prevScene->_gameObjectsInScene.end(), [this](std::shared_ptr<DUOLGameEngine::GameObject> go)
			{
				if (go->_isDontDestroyOnLoad)
					this->_gameObjectsInScene.push_back(go);
			});
	}

	void Scene::RegisterCreateGameObject(GameObject* gameObject)
	{
		for (auto iter = _gameObjectsForCreate.begin(); iter != _gameObjectsForCreate.end();)
		{
			// 이미 같은 게임 오브젝트에 대한 요청이 있었다면 그냥 넘어갑니다.
			if (iter->get() == gameObject)
				return;
			else
				++iter;
		}

		_gameObjectsForCreate.push_back(gameObject->shared_from_this());
	}

	void Scene::RegisterDestroyGameObject(GameObject* gameObject, float deltaTime)
	{
		for (auto iter = _gameObjectsForDestroy.begin(); iter != _gameObjectsForDestroy.end();)
		{
			// 이미 같은 게임 오브젝트에 대한 요청이 있다면
			// 등록하지 않고 반환한다. (기존의 요청이 더 우선 순위가 있는 것으로 판단한다.)
			if (iter->first.get() == gameObject)
				return;
			else
				++iter;
		}

		_gameObjectsForDestroy.push_back({ gameObject->shared_from_this(), deltaTime });
	}

	void Scene::RegisterActiveGameObject(GameObject* gameObject)
	{
		for (auto iter = _gameObjectsForInActive.begin(); iter != _gameObjectsForInActive.end();) // 비활성화에 예약이 걸려 있다면 제거해준다.
		{
			if (iter->get() == gameObject)
			{
				_gameObjectsForInActive.erase(iter);
				break;
			}
			else
				++iter;
		}

		for (auto iter = _gameObjectsForActive.begin(); iter != _gameObjectsForActive.end();)
		{
			// 이미 예약되어 있으면 그냥 넘어갑니다.
			if (iter->get() == gameObject)
				return;
			else
				++iter;
		}

		_gameObjectsForActive.push_back(gameObject->shared_from_this());
	}

	void Scene::RegisterInActiveGameObject(GameObject* gameObject)
	{
		for (auto iter = _gameObjectsForActive.begin(); iter != _gameObjectsForActive.end();) // 활성화에 예약이 걸려 있다면 제거해준다.
		{
			if (iter->get() == gameObject)
			{
				_gameObjectsForActive.erase(iter);
				break;
			}
			else
				++iter;
		}

		for (auto iter = _gameObjectsForInActive.begin(); iter != _gameObjectsForInActive.end();)
		{
			if (iter->get() == gameObject)
				return;
			else
				++iter;
		}

		_gameObjectsForInActive.push_back(gameObject->shared_from_this());
	}

	DUOLGameEngine::GameObject* Scene::CreateEmpty()
	{
		// 게임 오브젝트는 shared_ptr을 통한 Control block 형성으로 관리된다.
		std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>(TEXT("EmptyObject"));

		gameObject->AddComponent<Transform>();

		gameObject->_scene = this;

		// 처음에 만들어질 때 모든 오브젝트들이 들어갈텐데 ?
		RegisterCreateGameObject(gameObject.get());

		return gameObject.get();
	}

	DUOLGameEngine::GameObject* Scene::CreateEmtpyUI()
	{
		// 게임 오브젝트는 shared_ptr을 통한 Control block 형성으로 관리된다.
		std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>(TEXT("UI"));

		GameObject* object;

		// Canvas가 없으면 UI를 만들때 생성한다. 
		if (!DUOLGameEngine::UIManager::GetInstance()->GetIsCanvas())
		{
			object = CreateEmtpyCanvas();
			DUOLGameEngine::UIManager::GetInstance()->CreateCanvas(object);
			// Canvas가 있다고 True로 바꿔준다. 
			DUOLGameEngine::UIManager::GetInstance()->SetIsCanvas();
		}
		else
		{
			object = DUOLGameEngine::UIManager::GetInstance()->GetCanvas();
		}

		gameObject->AddComponent<RectTransform>();

		gameObject->_scene = this;

		gameObject->GetTransform()->SetParent(object->GetTransform());

		// 처음에 만들어질 때 모든 오브젝트들이 들어갈텐데 ?
		RegisterCreateGameObject(gameObject.get());

		return gameObject.get();
	}

	DUOLGameEngine::GameObject* Scene::CreateEmtpyCanvas()
	{
		// 게임 오브젝트는 shared_ptr을 통한 Control block 형성으로 관리된다.                                                  
		std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>(TEXT("Canvas"));

		gameObject->AddComponent<RectTransform>();
		gameObject->AddComponent<Canvas>();

		auto screensize = DUOLGameEngine::GraphicsManager::GetInstance()->GetScreenSize();

		std::string canvasName = "Canvas" + std::to_string(_canvasCount);

		gameObject->GetComponent<Canvas>()->CreateCanvas(DUOLGraphicsLibrary::CanvasRenderMode::Texture, DUOLCommon::StringHelper::ToTString(canvasName), screensize.x, screensize.y);

		gameObject->_scene = this;

		// 처음에 만들어질 때 모든 오브젝트들이 들어갈텐데 ?
		RegisterCreateGameObject(gameObject.get());

		_canvasCount++;

		return gameObject.get();
	}

	DUOLGameEngine::GameObject* Scene::CreateFromFBXModel(const DUOLCommon::tstring& fbxFileName)
	{
		// 가장 상단의 오브젝트를 만든다.
		DUOLGameEngine::GameObject* gameObject = CreateEmpty();

		gameObject->SetName(fbxFileName);

		// TODO : FBX 정보에 따라 조립합시다. (Static, Skinned 등 ..)

		// 1. FBX Parsing 정보 가져온다.
		DUOLGraphicsEngine::Model* model = DUOLGameEngine::ResourceManager::GetInstance()->GetModel(fbxFileName);

		if (model == nullptr)
			return gameObject;

		// 스태틱 모델인 경우
		if (!model->IsSkinningModel())
		{
			unsigned meshCount = model->GetMeshCount();

			for (unsigned i = 0; i < meshCount; i++)
			{
				DUOLCommon::tstring meshName = model->GetMesh(i)->_meshName;

				DUOLGameEngine::Mesh* engineMesh = DUOLGameEngine::ResourceManager::GetInstance()->GetMesh(meshName);

				DUOLGameEngine::GameObject* newGO = CreateEmpty();

				newGO->SetName(meshName);

				auto meshRenderer = newGO->AddComponent<DUOLGameEngine::MeshRenderer>();

				newGO->AddComponent<DUOLGameEngine::MeshFilter>()->SetMesh(engineMesh);

				for (int subMeshIndex = 0; subMeshIndex < engineMesh->GetPrimitiveMesh()->GetSubMeshCount(); subMeshIndex++)
				{
					meshRenderer->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(engineMesh->GetPrimitiveMesh()->GetSubMesh(subMeshIndex)->_materialName));
				}

				newGO->GetTransform()->SetParent(gameObject->GetTransform());
			}
		}
		// 스킨드 모델인 경우
		else if (model->IsSkinningModel())
		{
			// 애니메이션을 준비하자 !
			DUOLGameEngine::Animator* animator = gameObject->AddComponent<DUOLGameEngine::Animator>();

			// 매쉬의 갯수
			unsigned meshCount = model->GetMeshCount();

			// 뼈 갯수
			unsigned boneCount = model->GetBones().size();

			// 뼈들
			auto&& bones = model->GetBones();

			// 본 게임 오브젝트들을 만들고 하이어라키를 연결합니다.
			std::vector<DUOLGameEngine::GameObject*> boneObjects{};
			std::vector<DUOLMath::Matrix> boneOffsetMatrices{};

			for (int i = 0; i < boneCount; i++)
			{
				DUOLGameEngine::GameObject* boneGO = CreateEmpty();

				// 함수 등록 안한다 ..!
				boneGO->_isBone = true;

				DUOLGameEngine::Transform* boneTransform = boneGO->GetTransform();

				auto&& bone = bones[i];

				boneObjects.push_back(boneGO);

				boneOffsetMatrices.push_back(bone._offsetMatrix);

				if ((bone._parentIndex == -1) || (boneObjects.size() == 1))
					boneTransform->SetParent(gameObject->GetTransform());
				else if (bone._parentIndex == 1)
					// boneTransform->SetParent(boneObjects[0]->GetTransform());
					boneTransform->SetParent(boneObjects[1]->GetTransform());		// TODO : 일단 지아의 오브젝트를 제대로 구성하기 위해서 가져온다.
				else
					boneTransform->SetParent(boneObjects[bone._parentIndex]->GetTransform());
				// boneTransform->SetParent(boneObjects[bone._parentIndex - 1]->GetTransform());

				boneGO->SetName(DUOLCommon::StringHelper::ToTString(bone._boneName));
			}

			// 생성된 본 게임 오브젝트들을 애니메이터에 부착
			animator->SetBoneGameObjects(boneObjects);

			animator->SetBoneOffsetMatrices(boneOffsetMatrices);

			// 메쉬를 지정합니다.
			for (unsigned i = 0; i < meshCount; i++)
			{
				DUOLCommon::tstring meshName = model->GetMesh(i)->_meshName;

				DUOLGameEngine::Mesh* engineMesh = DUOLGameEngine::ResourceManager::GetInstance()->GetMesh(meshName);

				DUOLGameEngine::GameObject* meshGO = CreateEmpty();

				meshGO->SetName(meshName);

				meshGO->GetTransform()->SetParent(gameObject->GetTransform());

				meshGO->AddComponent<DUOLGameEngine::SkinnedMeshRenderer>()->SetSkinnedMesh(engineMesh);

				auto skinnedMeshRenderer = meshGO->GetComponent<DUOLGameEngine::SkinnedMeshRenderer>();

				for (int subMeshIndex = 0; subMeshIndex < engineMesh->GetPrimitiveMesh()->GetSubMeshCount(); subMeshIndex++)
				{
					skinnedMeshRenderer->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(engineMesh->GetPrimitiveMesh()->GetSubMesh(subMeshIndex)->_materialName));
				}
			}
		}

		return gameObject;
	}

	DUOLGameEngine::GameObject* Scene::CreateFromFBXMesh(const DUOLCommon::tstring& meshName)
	{
		DUOLGameEngine::Mesh* engineMesh = DUOLGameEngine::ResourceManager::GetInstance()->GetMesh(meshName);

		DUOLGameEngine::GameObject* newGO = CreateEmpty();

		newGO->SetName(meshName);

		auto meshRenderer = newGO->AddComponent<DUOLGameEngine::MeshRenderer>();

		newGO->AddComponent<DUOLGameEngine::MeshFilter>()->SetMesh(engineMesh);

		for (int subMeshIndex = 0; subMeshIndex < engineMesh->GetPrimitiveMesh()->GetSubMeshCount(); subMeshIndex++)
		{
			meshRenderer->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(engineMesh->GetPrimitiveMesh()->GetSubMesh(subMeshIndex)->_materialName));
		}

		return newGO;
	}

	DUOLGameEngine::GameObject* Scene::CreateFromParticleData(const DUOLCommon::tstring& ParticleFileName)
	{
		auto datas = ResourceManager::GetInstance()->LoadRenderingData_Particle(ParticleFileName);

		if(datas == nullptr)
		{
			ResourceManager::GetInstance()->CreateParticleMaterial(ParticleFileName);

			datas = ResourceManager::GetInstance()->LoadRenderingData_Particle(ParticleFileName);
		}

		std::vector<GameObject*> gameObjects;

		gameObjects.resize(datas->size());

		for (int i = datas->size() - 1; i >= 0; i--)
		{
			gameObjects[i] = this->CreateEmpty();

			gameObjects[i]->SetName(ParticleFileName + DUOLCommon::StringHelper::ToTString(i));
		}

		for (int i = 0; i < datas->size(); i++)
		{
			auto& particleData = gameObjects[i]->AddComponent<DUOLGameEngine::ParticleRenderer>()->GetParticleData();

			particleData = *(*datas)[i];

			for (auto childID : (*datas)[i]->_childrenIDs)
			{
				gameObjects[childID]->GetTransform()->SetParent(gameObjects[i]->GetTransform());
			}
		}

		for (int i = gameObjects.size() - 1; i >= 0; i--)
		{
			gameObjects[i]->GetTransform()->SetWorldTM((*datas)[i]->_commonInfo._transformMatrix);

			auto mat = DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(ParticleFileName + DUOLCommon::StringHelper::ToTString(i));

			auto particleRenderer = gameObjects[i]->GetComponent<DUOLGameEngine::ParticleRenderer>();

			particleRenderer->AddMaterial(mat);

			particleRenderer->CreateParticleBuffer();

			particleRenderer->Play();
		}

		return gameObjects.back();
	}

	void Scene::CreateGraphicsSetting()
	{
		if (_graphicsSetting._exponentialHeightFog == nullptr)
		{
			_graphicsSetting._exponentialHeightFog = std::make_shared<ExponentialHeightFog>();
		}
		if (_graphicsSetting._toneMapping == nullptr)
		{
			_graphicsSetting._toneMapping = std::make_shared<ToneMapping>();
		}
		if (_graphicsSetting._screenSpaceReflection == nullptr)
		{
			_graphicsSetting._screenSpaceReflection = std::make_shared<ScreenSpaceReflection>();
		}
		if (_graphicsSetting._rimLight == nullptr)
		{
			_graphicsSetting._rimLight = std::make_shared<RimLight>();
		}
		if (_graphicsSetting._bloom == nullptr)
		{
			_graphicsSetting._bloom = std::make_shared<Bloom>();
		}
	}

	void Scene::UpdateGraphicsSettings()
	{
		GraphicsManager::GetInstance()->SetGraphicSetting(_graphicsSetting);
	}

	const DUOLCommon::tstring& Scene::GetName() const
	{
		return _name;
	}

	const DUOLCommon::tstring& Scene::GetPath() const
	{
		return _path;
	}

	const DUOLCommon::tstring& Scene::GetNavMeshFileName() const
	{
		return _navMeshFileName;
	}

	const GraphicsSetting& Scene::GetGraphicsSetting() const
	{
		return _graphicsSetting;
	}

	void Scene::SetNavMeshFileName(const DUOLCommon::tstring& fileName)
	{
		_navMeshFileName = fileName;
	}

	const std::vector<DUOLGameEngine::GameObject*> Scene::GetRootObjects() const
	{
		std::vector<DUOLGameEngine::GameObject*> ret{};

		for (auto&& rootObject : _rootObjectsInScene)
			ret.push_back(rootObject.get());

		return ret;
	}

	const std::vector<DUOLGameEngine::GameObject*> Scene::GetAllGameObjects() const
	{
		std::vector<DUOLGameEngine::GameObject*> ret{};

		for (auto&& gameObject : _gameObjectsInScene)
			ret.push_back(gameObject.get());

		return ret;
	}
}