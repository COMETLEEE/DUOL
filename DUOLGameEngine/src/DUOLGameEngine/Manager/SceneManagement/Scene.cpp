#include "DUOLGameEngine/Manager/SceneManagement/Scene.h"

#include "DUOLGameEngine/ECS/GameObject.h"

#include "DUOLGameEngine/Manager/PhysicsManager.h"
#include "DUOLGameEngine/Manager/TimeManager.h"

namespace DUOLGameEngine
{
	Scene::Scene(const DUOLCommon::tstring& name) :
		_gameObjectsInScene(std::list<std::shared_ptr<GameObject>>())
		, _gameObjectsForCreate(std::vector<std::shared_ptr<GameObject>>())
		, _gameObjectsForDestroy(std::list<std::pair<std::shared_ptr<GameObject>, float>>())
		, _gameObjectsForActive(std::list<std::shared_ptr<GameObject>>())
		, _gameObjectsForInActive(std::list<std::shared_ptr<GameObject>>())
		, _name(name)
		, _path(DUOLCommon::StringHelper::ToTString("Empty"))
	{
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

		for (auto& gameObject : _gameObjectsInScene)
			gameObject.reset();

		_gameObjectsForCreate.clear();

		_gameObjectsForDestroy.clear();

		_gameObjectsForActive.clear();

		_gameObjectsForInActive.clear();

		_gameObjectsInScene.clear();
	}

	void Scene::Awake()
	{
		// ���� ��� ������Ʈ ����Ʈ�� �Ű��ݴϴ�.
		for (auto iter = _gameObjectsForCreate.begin() ; iter != _gameObjectsForCreate.end() ;)
		{
			_gameObjectsInScene.push_back(*iter);

			++iter;
		}

		// �� �Ű����ϱ� ����Ʈ ����ְ�
		_gameObjectsForCreate.clear();

		for (const auto& gameObject : _gameObjectsInScene)
		{
			// Awake�� ��쿡�� ��Ȱ��ȭ ������ ���� ������Ʈ�� �����մϴ� ..!
			gameObject->OnAwake();
		}
	}

	void Scene::Start() const
	{
		for (const auto& gameObject : _gameObjectsInScene)
		{
			if (gameObject->GetIsActive())
			{
				// Active���� ���� !
				gameObject->OnActive();

				gameObject->OnStart();
			}
		}
	}

	void Scene::Update(float deltaTime) const
	{
		for (const auto& gameObject : _gameObjectsInScene)
		{
			// Active Game Object�� ���Ͽ� ���� �ڷ� ������ ���ΰ� �� �������� if �б� �ϴ� ����
			// ���� �� ���� �� ���� ..! (���� ���� �ʼ� !)
			if (gameObject->GetIsActive())
				gameObject->OnUpdate(deltaTime);
		}
	}

	void Scene::InvokeUpdate(float deltaTime) const
	{
		for (const auto& gameObject : _gameObjectsInScene)
		{
			if (gameObject->GetIsActive())
				gameObject->OnInvokeUpdate(deltaTime);
		}
	}

	void Scene::CoroutineUpdate(float deltaTime) const
	{
		for (const auto& gameObject : _gameObjectsInScene)
		{
			if (gameObject->GetIsActive())
				gameObject->OnCoroutineUpdate(deltaTime);
		}
	}

	void Scene::FixedUpdate(float deltaTime) const
	{
		for (const auto& gameObject : _gameObjectsInScene)
		{
			if (gameObject->GetIsActive())
				gameObject->OnFixedUpdate(deltaTime);
		}
	}

	void Scene::LateUpdate(float deltaTime) const
	{
		for (const auto& gameObject : _gameObjectsInScene)
		{
			if (gameObject->GetIsActive())
				gameObject->OnLateUpdate(deltaTime);
		}
	}

	void Scene::CreateGameObjects()
	{
		for (auto iter = _gameObjectsForCreate.begin() ; iter != _gameObjectsForCreate.end() ;)
		{
			// �Ͼ��
			(*iter)->OnAwake();

			// �����ϱ�
			(*iter)->OnStart();

			_gameObjectsInScene.push_back(*iter);

			// ���� ������Ʈ��� Physics Manager�� ��ϱ��� !
			DUOLGameEngine::PhysicsManager::GetInstance()->InitializePhysicsGameObject(*iter);

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

		// 2. �ð��� ���� ������ ������Ʈ�� �� ����Ʈ���� �����մϴ�.
		for (auto iter = _gameObjectsForDestroy.begin() ; iter != _gameObjectsForDestroy.end() ;)
		{
			// ���� ���� �ð��� ���Ҵٸ� �Ѿ�ϴ�.
			if (iter->second > 0.f)
			{
				++iter;

				continue;
			}

			for (auto iter2 = _gameObjectsInScene.begin() ; iter2 != _gameObjectsInScene.end(); )
			{
				if (iter->first == *iter2)
				{
					std::shared_ptr<DUOLGameEngine::GameObject> gameObject = *iter2;

					iter2 = _gameObjectsInScene.erase(iter2);

					// ���� �������� �͵� �Ҹ���Ѿ� �ϴ� ���̴� ...
					DUOLGameEngine::PhysicsManager::GetInstance()->UnInitializePhysicsGameObject(gameObject);

					// �̿� ���ÿ� �޸𸮿��� ���� ..? �Ǳ⸦ �ٷ��� �ϴ� �� ���� ? ���߿� �޸� Ǯ ���õǾ
					// ��⵵ �غ��ƾ��� �� ������.
					gameObject.reset();

					// �ı� ����Ʈ���� �����ְ� iterator �����ֱ�.
					iter = _gameObjectsForDestroy.erase(iter);

					// �� ������ ������.
					break;
				}
				else
					++iter2;
			}
		}
	}

	void Scene::ActiveGameObjects()
	{
		// ����� �༮���� Ȱ��ȭ�Ѵ�.
		for (auto iter = _gameObjectsForActive.begin() ; iter != _gameObjectsForActive.end(); )
		{
			(*iter)->OnActive();

			(*iter)->_isActive = true;

			iter = _gameObjectsForActive.erase(iter);
		}
	}

	void Scene::InActiveGameObjects()
	{
		// ����� �༮���� ��Ȱ��ȭ�Ѵ�.
		for (auto iter = _gameObjectsForInActive.begin(); iter != _gameObjectsForInActive.end(); )
		{
			(*iter)->OnInActive();

			(*iter)->_isActive = false;

			iter = _gameObjectsForInActive.erase(iter);
		}
	}

	void Scene::RegisterCreateGameObject(const std::shared_ptr<GameObject>& gameObject)
	{
		for (auto iter = _gameObjectsForCreate.begin() ; iter != _gameObjectsForCreate.end();)
		{
			if (*iter == gameObject)
				return;
			else
				++iter;
		}

		_gameObjectsForCreate.push_back(gameObject);
	}

	void Scene::RegisterDestroyGameObject(const std::shared_ptr<GameObject>& gameObject, float deltaTime)
	{
		for (auto iter = _gameObjectsForDestroy.begin() ; iter != _gameObjectsForDestroy.end() ;)
		{
			// �̹� ���� ���� ������Ʈ�� ���� ��û�� �ִٸ�
			// ������� �ʰ� ��ȯ�Ѵ�. (������ ��û�� �� �켱 ������ �ִ� ������ �Ǵ��Ѵ�.)
			if (iter->first == gameObject)
				return;
			else
				++iter;
		}

		_gameObjectsForDestroy.push_back({gameObject, deltaTime});
	}

	void Scene::RegisterActiveGameObject(const std::shared_ptr<GameObject>& gameObject)
	{
		for (auto iter = _gameObjectsForActive.begin() ; iter != _gameObjectsForActive.end();)
		{
			if (*iter == gameObject)
				return;
			else
				++iter;
		}

		_gameObjectsForActive.push_back(gameObject);
	}

	void Scene::RegisterInActiveGameObject(const std::shared_ptr<GameObject>& gameObject)
	{
		for (auto iter = _gameObjectsForInActive.begin(); iter != _gameObjectsForInActive.end();)
		{
			if (*iter == gameObject)
				return;
			else
				++iter;
		}

		_gameObjectsForInActive.push_back(gameObject);
	}

	std::shared_ptr<GameObject> Scene::CreateEmpty()
	{
		std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>(TEXT("EmptyObject"));

		gameObject->AddComponent<Transform>();

		gameObject->_scene = this->weak_from_this();

		RegisterCreateGameObject(gameObject);

		return gameObject;
	}
}