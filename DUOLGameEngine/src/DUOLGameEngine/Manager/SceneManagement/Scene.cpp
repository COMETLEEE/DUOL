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
		// 씬의 등록 오브젝트 리스트로 옮겨줍니다.
		for (auto iter = _gameObjectsForCreate.begin() ; iter != _gameObjectsForCreate.end() ;)
		{
			_gameObjectsInScene.push_back(*iter);

			++iter;
		}

		// 다 옮겼으니까 리스트 비워주고
		_gameObjectsForCreate.clear();

		for (const auto& gameObject : _gameObjectsInScene)
		{
			// Awake의 경우에는 비활성화 상태의 게임 오브젝트도 실행합니다 ..!
			gameObject->OnAwake();
		}
	}

	void Scene::Start() const
	{
		for (const auto& gameObject : _gameObjectsInScene)
		{
			if (gameObject->GetIsActive())
			{
				// Active까지 시작 !
				gameObject->OnActive();

				gameObject->OnStart();
			}
		}
	}

	void Scene::Update(float deltaTime) const
	{
		for (const auto& gameObject : _gameObjectsInScene)
		{
			// Active Game Object에 대하여 따로 자료 구조를 놔두고 매 구문마다 if 분기 하는 것을
			// 줄일 수 있을 것 같다 ..! (추후 개선 필수 !)
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
			// 일어나기
			(*iter)->OnAwake();

			// 시작하기
			(*iter)->OnStart();

			_gameObjectsInScene.push_back(*iter);

			// 물리 오브젝트라면 Physics Manager에 등록까지 !
			DUOLGameEngine::PhysicsManager::GetInstance()->InitializePhysicsGameObject(*iter);

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

		// 2. 시간이 전부 지나간 오브젝트는 씬 리스트에서 제거합니다.
		for (auto iter = _gameObjectsForDestroy.begin() ; iter != _gameObjectsForDestroy.end() ;)
		{
			// 아직 예약 시간이 남았다면 넘어갑니다.
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

					// 먼저 물리적인 것도 소멸시켜야 하는 것이다 ...
					DUOLGameEngine::PhysicsManager::GetInstance()->UnInitializePhysicsGameObject(gameObject);

					// 이와 동시에 메모리에서 해제 ..? 되기를 바래야 하는 것 맞음 ? 나중에 메모리 풀 관련되어서
					// 얘기도 해보아야할 것 같은데.
					gameObject.reset();

					// 파괴 리스트에서 지워주고 iterator 돌려주기.
					iter = _gameObjectsForDestroy.erase(iter);

					// 이 문장을 끝낸다.
					break;
				}
				else
					++iter2;
			}
		}
	}

	void Scene::ActiveGameObjects()
	{
		// 예약된 녀석들을 활성화한다.
		for (auto iter = _gameObjectsForActive.begin() ; iter != _gameObjectsForActive.end(); )
		{
			(*iter)->OnActive();

			(*iter)->_isActive = true;

			iter = _gameObjectsForActive.erase(iter);
		}
	}

	void Scene::InActiveGameObjects()
	{
		// 예약된 녀석들을 비활성화한다.
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
			// 이미 같은 게임 오브젝트에 대한 요청이 있다면
			// 등록하지 않고 반환한다. (기존의 요청이 더 우선 순위가 있는 것으로 판단한다.)
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