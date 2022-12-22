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
//#include "DUOLGraphicsEngine/ResourceManager/Resource/Particle.h"


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
		for (auto iter = _gameObjectsForCreate.begin(); iter != _gameObjectsForCreate.end();)
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
		for (auto iter = _gameObjectsForCreate.begin(); iter != _gameObjectsForCreate.end();)
		{
			// �Ͼ��
			(*iter)->OnAwake();

			// �����ϱ�
			(*iter)->OnStart();

			_gameObjectsInScene.push_back(*iter);

			// ���� ������Ʈ��� Physics Manager�� ��ϱ��� !
			DUOLGameEngine::PhysicsManager::GetInstance()->InitializePhysicsGameObject((*iter).get());

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
		for (auto iter = _gameObjectsForDestroy.begin(); iter != _gameObjectsForDestroy.end();)
		{
			// ���� ���� �ð��� ���Ҵٸ� �Ѿ�ϴ�.
			if (iter->second > 0.f)
			{
				++iter;

				continue;
			}

			for (auto iter2 = _gameObjectsInScene.begin(); iter2 != _gameObjectsInScene.end(); )
			{
				if (iter->first == *iter2)
				{
					std::shared_ptr<DUOLGameEngine::GameObject> gameObject = *iter2;

					iter2 = _gameObjectsInScene.erase(iter2);

					// ���� �������� �͵� �Ҹ���Ѿ� �ϴ� ���̴� ...
					DUOLGameEngine::PhysicsManager::GetInstance()->UnInitializePhysicsGameObject(gameObject.get());

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
		for (auto iter = _gameObjectsForActive.begin(); iter != _gameObjectsForActive.end(); )
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
		for (auto iter = _gameObjectsForActive.begin(); iter != _gameObjectsForActive.end();)
		{
			if (iter->get() == gameObject)
				return;
			else
				++iter;
		}

		_gameObjectsForActive.push_back(gameObject->shared_from_this());
	}

	void Scene::RegisterInActiveGameObject(GameObject* gameObject)
	{
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
		// ���� ������Ʈ�� shared_ptr�� ���� Control_Block �������� �����ȴ�.
		std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>(TEXT("EmptyObject"));

		gameObject->AddComponent<Transform>();

		gameObject->_scene = this->weak_from_this();

		RegisterCreateGameObject(gameObject.get());

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
		if (!model->GetIsSkinningModel())
		{
			unsigned meshCount = model->GetMeshCount();

			for (unsigned i = 0 ; i < meshCount ; i++)
			{
				DUOLCommon::tstring meshName = model->GetMesh(i)->_meshName;

				DUOLGameEngine::Mesh* engineMesh = DUOLGameEngine::ResourceManager::GetInstance()->GetMesh(meshName);

				DUOLGameEngine::GameObject* newGO = CreateEmpty();

				newGO->SetName(meshName);

				newGO->AddComponent<DUOLGameEngine::MeshRenderer>();

				newGO->AddComponent<DUOLGameEngine::MeshFilter>()->SetMesh(engineMesh);

				newGO->GetTransform()->SetParent(gameObject->GetTransform());
			}
		}
		// ��Ų�� ���� ���
		else if (model->GetIsSkinningModel())
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
			std::vector<DUOLGameEngine::GameObject*> boneObjects {};

			std::vector<DUOLMath::Matrix> boneOffsetMatrices {};

			for (int i = 0 ; i < boneCount ; i++)
			{
				DUOLGameEngine::GameObject* boneGO = CreateEmpty();

				DUOLGameEngine::Transform* boneTransform = boneGO->GetTransform();

				auto&& bone = bones[i];

				boneObjects.push_back(boneGO);

				boneOffsetMatrices.push_back(bone._offsetMatrix);

				if (bone._parentIndex == -1)
					boneTransform->SetParent(gameObject->GetTransform());
				else
					boneTransform->SetParent(boneObjects[bone._parentIndex - 1]->GetTransform());

				boneGO->SetName(bone._boneName);
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

				meshGO->AddComponent<DUOLGameEngine::SkinnedMeshRenderer>()->SetSkinnedMesh(engineMesh);

				meshGO->GetTransform()->SetParent(gameObject->GetTransform());
			}
		}

		return gameObject;
	}

	DUOLGameEngine::GameObject* Scene::CreateFromParticleData(const DUOLCommon::tstring& ParticleFileName)
	{
		DUOLGraphicsEngine::RenderingData_Particle data;

		std::ifstream fr(ParticleFileName);

		if (fr.is_open())
		{
			boost::archive::binary_iarchive inArchive(fr);

			inArchive >> data;

			fr.close();
		}
		else
		{
			// ���� ���.
		}
		/// �ν�Ʈ ���̺귯�� ��ø�������� �Ϸ�.


		std::function<GameObject* (DUOLGraphicsEngine::RenderingData_Particle&, GameObject*)> func
			= [&](DUOLGraphicsEngine::RenderingData_Particle& data, GameObject* parent)->GameObject*
		{
			auto ParticleObject = this->CreateEmpty();

			auto particleData = ParticleObject->AddComponent<DUOLGameEngine::ParticleRenderer>()->GetParticleData();

			if (parent)
				ParticleObject->GetTransform()->SetParent(parent->GetTransform());
			data._commonInfo._firstRun = true;

			data._objectID = ParticleObject->GetUUID();

			particleData = data;

			ParticleObject->GetTransform()->SetWorldTM(data._commonInfo._transformMatrix);

			for (auto iter : data._childrens)
			{
				func(iter, ParticleObject);
			}

			std::vector<DUOLGraphicsEngine::RenderingData_Particle>().swap(particleData._childrens);

			return ParticleObject;
		};

		return func(data, nullptr);
	}
}
