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
		/// �̰� �ۿ� ����� ȣ��� ���� .. �ƴϸ� �׳� �Ҹ��ڿ��� �����ұ�
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
		// TODO : ���� ��� ������Ʈ ����Ʈ�� �Ű��ݴϴ�. �׽�Ʈ�� ���� ���� ���� �ڵ带 ���ξ����ϴ�.
		for (auto iter = _gameObjectsInScene.begin(); iter != _gameObjectsInScene.end();)
		{
			if ((*iter)->GetTransform()->IsRootObject())
			{
				_rootObjectsInScene.push_back(*iter);
			}

			iter->get()->_scene = this;

			++iter;
		}

		// TODO : ��ü ����ȭ�� �Ϸ�Ǹ� �� ������ �������� �մϴ�. ���� ��� ������Ʈ ����Ʈ�� �Ű��ݴϴ�. ������� �׽�Ʈ�� ���� ���� ���� �ڵ带 ���Ӵϴ�.
		for (auto iter = _gameObjectsForCreate.begin(); iter != _gameObjectsForCreate.end();)
		{
			if ((*iter)->GetTransform()->IsRootObject())
				_rootObjectsInScene.push_back(*iter);

			// ���� ������Ʈ�� ���� ī��Ʈ ������ ���Ͽ� ������ �ִ´� .. ���� ���� �� �����丵 ���ô� ..
			_gameObjectsInScene.push_back(*iter);

			++iter;
		}

		// �� �Ű����ϱ� ����Ʈ ����ְ�
		_gameObjectsForCreate.clear();

		for (const auto& rootObject : _rootObjectsInScene)
		{
			// Awake�� ��쿡�� ��Ȱ��ȭ ������ ���� ������Ʈ�� �����մϴ� ..!
			rootObject->OnAwake();
		}

		// Ʈ���� �����Ѵ�.
		_octree = DUOLGameEngine::Octree::BuildOctree(this);

		_quadtree = DUOLGameEngine::Quadtree::BuildQuadtree(this);

	}

	void Scene::SetGameObjectList()
	{
		// TODO : ���� ��� ������Ʈ ����Ʈ�� �Ű��ݴϴ�. �׽�Ʈ�� ���� ���� ���� �ڵ带 ���ξ����ϴ�.
		for (auto iter = _gameObjectsInScene.begin(); iter != _gameObjectsInScene.end();)
		{
			if ((*iter)->GetTransform()->IsRootObject())
			{
				_rootObjectsInScene.push_back(*iter);
			}

			iter->get()->_scene = this;

			++iter;
		}

		// TODO : ��ü ����ȭ�� �Ϸ�Ǹ� �� ������ �������� �մϴ�. ���� ��� ������Ʈ ����Ʈ�� �Ű��ݴϴ�. ������� �׽�Ʈ�� ���� ���� ���� �ڵ带 ���Ӵϴ�.
		for (auto iter = _gameObjectsForCreate.begin(); iter != _gameObjectsForCreate.end();)
		{
			if ((*iter)->GetTransform()->IsRootObject())
				_rootObjectsInScene.push_back(*iter);

			// ���� ������Ʈ�� ���� ī��Ʈ ������ ���Ͽ� ������ �ִ´� .. ���� ���� �� �����丵 ���ô� ..
			_gameObjectsInScene.push_back(*iter);

			++iter;
		}

		// �� �Ű����ϱ� ����Ʈ ����ְ�
		_gameObjectsForCreate.clear();
	}

	void Scene::BuildStaticGameObjectsTree()
	{
		// Ʈ���� �����Ѵ�.
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
		// �� .. ����ұ� ..
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

			// ���� ������Ʈ��� Physics Manager�� ��ϱ��� ! => ��, ��� ������Ʈ���� ���� �� �����Ƿ� recursively �ʰ� ..
			// ���� �迭 ������Ʈ�� �߰��ϰų� ���� �� .. �ڵ����� Initialize �˴ϴ� ..!
			// DUOLGameEngine::PhysicsManager::GetInstance()->InitializePhysicsGameObject((*iter).get(), false);

			// �׺���̼� ������Ʈ��� Navigation Manager�� ��ϱ��� ! => �׺���̼� �޽���
			// ������Ƽ�� ������Ʈ�� ������ ������ �ʿ��ؼ� .. ������Ʈ ������ �̺�Ʈ �Լ����� �ʱ�ȭ�մϴ�.
			// DUOLGameEngine::NavigationManager::GetInstance()->InitializeNavigationGameObject((*iter).get(), false);

			// ��Ʈ ������Ʈ��� ��Ʈ ������Ʈ ����Ʈ�� �־��ݴϴ�.
			if (gameObject->GetTransform()->IsRootObject())
				_rootObjectsInScene.push_back(*iter);

			// ���� ī��Ʈ ������ ���Ͽ� ������ �ִ´�.
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
		// ������ ���� �ð� with scaled.
		float deltaTime = DUOLGameEngine::TimeManager::GetInstance()->GetDeltaTime();

		// 1. ���� ������Ʈ �ı� ���� �ð� ������Ʈ
		for (auto iter = _gameObjectsForDestroy.begin(); iter != _gameObjectsForDestroy.end();)
		{
			iter->second -= deltaTime;

			++iter;
		}

		// 2. �ð��� ���� ������ ���� ������Ʈ�� �� ����Ʈ���� �����մϴ�.
		for (auto iter = _gameObjectsForDestroy.begin(); iter != _gameObjectsForDestroy.end();)
		{
			// ���� ���� �ð��� ���Ҵٸ� �Ѿ�ϴ�.
			if (iter->second > 0.f)
			{
				++iter;

				continue;
			}

			// �������ϴ� ������Ʈ���µ� ����, ��Ʈ ������Ʈ��� ����Ʈ���� �����ݴϴ�.
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

			// ���� ī��Ʈ�� ���ֱ� ���ؼ� '_gameObjectsInScene' ������ �������ݴϴ�.
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

			// ��������� �ڽ� ������Ʈ���� ����� ������ �����մϴ�.
			{
				std::shared_ptr<DUOLGameEngine::GameObject> gameObject = iter->first;

				gameObject->OnDestroy();

				// ���� �ùķ��̼ǿ��� ����
				DUOLGameEngine::PhysicsManager::GetInstance()->UnInitializePhysicsGameObject(gameObject.get(), true);

				// DUOLGameEngine::NavigationManager::GetInstance()->UnIni

				// OnDestroy�� �Ϸ�� �ڽ� ������Ʈ���� ����Ʈ���� �����մϴ�. _gameObjectsInScene ���� �����մϴ�.
				// �޸𸮰� �����˴ϴ�.
				auto&& children = gameObject->GetTransform()->GetAllChildGameObjects();

				for (auto child : children)
				{
					for (auto iter2 = _gameObjectsInScene.begin(); iter2 != _gameObjectsInScene.end();)
					{
						if (child == (*iter2).get())
						{
							// �θ� �ڽ� ���� reset
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

			// �θ� �ڽ� ���� reset
			auto parentTransform = iter->first.get()->GetTransform()->GetParent();

			if (parentTransform != nullptr)
				parentTransform->ResetChild(iter->first.get()->GetTransform());

			// iter->first.get()->GetTransform()->SetParent(nullptr, true, false);

			// ���� ī��Ʈ 0 !
			iter = _gameObjectsForDestroy.erase(iter);
		}
	}

	void Scene::ActiveGameObjects()
	{
		// ����� �༮���� Ȱ��ȭ�Ѵ�.
		for (auto iter = _gameObjectsForActive.begin(); iter != _gameObjectsForActive.end(); )
		{
			(*iter)->_isActive = true;

			(*iter)->OnActive();

			iter = _gameObjectsForActive.erase(iter);
		}
	}

	void Scene::InActiveGameObjects()
	{
		// ����� �༮���� ��Ȱ��ȭ�Ѵ�.
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
		// ���� ���� ���� ������Ʈ�̸� �� ��Ʈ ������Ʈ �����׿� ���� �ʽ��ϴ� ..!
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
			// �̹� ���� ���� ������Ʈ�� ���� ��û�� �־��ٸ� �׳� �Ѿ�ϴ�.
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
			// �̹� ���� ���� ������Ʈ�� ���� ��û�� �ִٸ�
			// ������� �ʰ� ��ȯ�Ѵ�. (������ ��û�� �� �켱 ������ �ִ� ������ �Ǵ��Ѵ�.)
			if (iter->first.get() == gameObject)
				return;
			else
				++iter;
		}

		_gameObjectsForDestroy.push_back({ gameObject->shared_from_this(), deltaTime });
	}

	void Scene::RegisterActiveGameObject(GameObject* gameObject)
	{
		for (auto iter = _gameObjectsForInActive.begin(); iter != _gameObjectsForInActive.end();) // ��Ȱ��ȭ�� ������ �ɷ� �ִٸ� �������ش�.
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
			// �̹� ����Ǿ� ������ �׳� �Ѿ�ϴ�.
			if (iter->get() == gameObject)
				return;
			else
				++iter;
		}

		_gameObjectsForActive.push_back(gameObject->shared_from_this());
	}

	void Scene::RegisterInActiveGameObject(GameObject* gameObject)
	{
		for (auto iter = _gameObjectsForActive.begin(); iter != _gameObjectsForActive.end();) // Ȱ��ȭ�� ������ �ɷ� �ִٸ� �������ش�.
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
		// ���� ������Ʈ�� shared_ptr�� ���� Control block �������� �����ȴ�.
		std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>(TEXT("EmptyObject"));

		gameObject->AddComponent<Transform>();

		gameObject->_scene = this;

		// ó���� ������� �� ��� ������Ʈ���� ���ٵ� ?
		RegisterCreateGameObject(gameObject.get());

		return gameObject.get();
	}

	DUOLGameEngine::GameObject* Scene::CreateEmtpyUI()
	{
		// ���� ������Ʈ�� shared_ptr�� ���� Control block �������� �����ȴ�.
		std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>(TEXT("UI"));

		GameObject* object;

		// Canvas�� ������ UI�� ���鶧 �����Ѵ�. 
		if (!DUOLGameEngine::UIManager::GetInstance()->GetIsCanvas())
		{
			object = CreateEmtpyCanvas();
			DUOLGameEngine::UIManager::GetInstance()->CreateCanvas(object);
			// Canvas�� �ִٰ� True�� �ٲ��ش�. 
			DUOLGameEngine::UIManager::GetInstance()->SetIsCanvas();
		}
		else
		{
			object = DUOLGameEngine::UIManager::GetInstance()->GetCanvas();
		}

		gameObject->AddComponent<RectTransform>();

		gameObject->_scene = this;

		gameObject->GetTransform()->SetParent(object->GetTransform());

		// ó���� ������� �� ��� ������Ʈ���� ���ٵ� ?
		RegisterCreateGameObject(gameObject.get());

		return gameObject.get();
	}

	DUOLGameEngine::GameObject* Scene::CreateEmtpyCanvas()
	{
		// ���� ������Ʈ�� shared_ptr�� ���� Control block �������� �����ȴ�.                                                  
		std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>(TEXT("Canvas"));

		gameObject->AddComponent<RectTransform>();
		gameObject->AddComponent<Canvas>();

		auto screensize = DUOLGameEngine::GraphicsManager::GetInstance()->GetScreenSize();

		std::string canvasName = "Canvas" + std::to_string(_canvasCount);

		gameObject->GetComponent<Canvas>()->CreateCanvas(DUOLGraphicsLibrary::CanvasRenderMode::Texture, DUOLCommon::StringHelper::ToTString(canvasName), screensize.x, screensize.y);

		gameObject->_scene = this;

		// ó���� ������� �� ��� ������Ʈ���� ���ٵ� ?
		RegisterCreateGameObject(gameObject.get());

		_canvasCount++;

		return gameObject.get();
	}

	DUOLGameEngine::GameObject* Scene::CreateFromFBXModel(const DUOLCommon::tstring& fbxFileName)
	{
		// ���� ����� ������Ʈ�� �����.
		DUOLGameEngine::GameObject* gameObject = CreateEmpty();

		gameObject->SetName(fbxFileName);

		// TODO : FBX ������ ���� �����սô�. (Static, Skinned �� ..)

		// 1. FBX Parsing ���� �����´�.
		DUOLGraphicsEngine::Model* model = DUOLGameEngine::ResourceManager::GetInstance()->GetModel(fbxFileName);

		if (model == nullptr)
			return gameObject;

		// ����ƽ ���� ���
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
		// ��Ų�� ���� ���
		else if (model->IsSkinningModel())
		{
			// �ִϸ��̼��� �غ����� !
			DUOLGameEngine::Animator* animator = gameObject->AddComponent<DUOLGameEngine::Animator>();

			// �Ž��� ����
			unsigned meshCount = model->GetMeshCount();

			// �� ����
			unsigned boneCount = model->GetBones().size();

			// ����
			auto&& bones = model->GetBones();

			// �� ���� ������Ʈ���� ����� ���̾��Ű�� �����մϴ�.
			std::vector<DUOLGameEngine::GameObject*> boneObjects{};
			std::vector<DUOLMath::Matrix> boneOffsetMatrices{};

			for (int i = 0; i < boneCount; i++)
			{
				DUOLGameEngine::GameObject* boneGO = CreateEmpty();

				// �Լ� ��� ���Ѵ� ..!
				boneGO->_isBone = true;

				DUOLGameEngine::Transform* boneTransform = boneGO->GetTransform();

				auto&& bone = bones[i];

				boneObjects.push_back(boneGO);

				boneOffsetMatrices.push_back(bone._offsetMatrix);

				if ((bone._parentIndex == -1) || (boneObjects.size() == 1))
					boneTransform->SetParent(gameObject->GetTransform());
				else if (bone._parentIndex == 1)
					// boneTransform->SetParent(boneObjects[0]->GetTransform());
					boneTransform->SetParent(boneObjects[1]->GetTransform());		// TODO : �ϴ� ������ ������Ʈ�� ����� �����ϱ� ���ؼ� �����´�.
				else
					boneTransform->SetParent(boneObjects[bone._parentIndex]->GetTransform());
				// boneTransform->SetParent(boneObjects[bone._parentIndex - 1]->GetTransform());

				boneGO->SetName(DUOLCommon::StringHelper::ToTString(bone._boneName));
			}

			// ������ �� ���� ������Ʈ���� �ִϸ����Ϳ� ����
			animator->SetBoneGameObjects(boneObjects);

			animator->SetBoneOffsetMatrices(boneOffsetMatrices);

			// �޽��� �����մϴ�.
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