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
		_gameObjectsForCreate(std::vector<std::shared_ptr<GameObject>>())
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

		for (auto& gameObject : _rootObjectsInScene)
			gameObject.reset();

		_gameObjectsForCreate.clear();

		_gameObjectsForDestroy.clear();

		_gameObjectsForActive.clear();

		_gameObjectsForInActive.clear();

		_gameObjectsInScene.clear();

		_rootObjectsInScene.clear();
	}

	void Scene::Awake()
	{
		// ���� ��� ������Ʈ ����Ʈ�� �Ű��ݴϴ�.
		for (auto iter = _gameObjectsForCreate.begin(); iter != _gameObjectsForCreate.end();)
		{
			if ((*iter)->GetTransform()->IsRootObject())
				_rootObjectsInScene.push_back(*iter);

			// ���� ī��Ʈ ������ ���Ͽ� ������ �ִ´�.
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
	}

	void Scene::Start() const
	{
		for (const auto& rootObject : _rootObjectsInScene)
		{
			if (rootObject->GetIsActive())
			{
				rootObject->OnActive();

				rootObject->OnStart();
			}
		}
	}

	void Scene::Update(float deltaTime) const
	{
		for (const auto& rootObject : _rootObjectsInScene)
		{
			if (rootObject->GetIsActive())
				rootObject->OnUpdate(deltaTime);
		}
	}

	void Scene::InvokeUpdate(float deltaTime) const
	{
		for (const auto& rootObject : _rootObjectsInScene)
		{
			if (rootObject->GetIsActive())
				rootObject->OnInvokeUpdate(deltaTime);
		}
	}

	void Scene::CoroutineUpdate(float deltaTime) const
	{
		for (const auto& rootObject : _rootObjectsInScene)
		{
			if (rootObject->GetIsActive())
				rootObject->OnCoroutineUpdate(deltaTime);
		}
	}

	void Scene::FixedUpdate(float deltaTime) const
	{
		for (const auto& rootObject : _rootObjectsInScene)
		{
			if (rootObject->GetIsActive())
				rootObject->OnFixedUpdate(deltaTime);
		}
	}

	void Scene::LateUpdate(float deltaTime) const
	{
		for (const auto& rootObject : _rootObjectsInScene)
		{
			if (rootObject->GetIsActive())
				rootObject->OnLateUpdate(deltaTime);
		}
	}

	void Scene::CreateGameObjects()
	{
		for (auto iter = _gameObjectsForCreate.begin(); iter != _gameObjectsForCreate.end();)
		{
			DUOLGameEngine::GameObject* gameObject = iter->get();

			gameObject->OnCreate();

			// ���� ������Ʈ��� Physics Manager�� ��ϱ��� ! => ��, ��� ������Ʈ���� ���� �� �����Ƿ� recursively �ʰ� ..
			DUOLGameEngine::PhysicsManager::GetInstance()->InitializePhysicsGameObject((*iter).get(), false);

			// ��Ʈ ������Ʈ��� ��Ʈ ������Ʈ ����Ʈ�� �־��ݴϴ�.
			if (gameObject->GetTransform()->IsRootObject())
				_rootObjectsInScene.push_back(*iter);

			// ���� ī��Ʈ ������ ���Ͽ� ������ �ִ´�.
			_gameObjectsInScene.push_back(*iter);

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
					if (iter->first == *iter2)
					{
						iter2 = _rootObjectsInScene.erase(iter2);

						break;
					}
					else
						++iter;
				}
			}

			// ���� ī��Ʈ�� ���ֱ� ���ؼ� '_gameObjectsInScene' ������ �������ݴϴ�.
			for (auto iter2 = _gameObjectsInScene.begin(); iter2 != _gameObjectsInScene.end();)
			{
				if (iter->first == *iter2)
				{
					iter2 = _gameObjectsInScene.erase(iter2);

					break;
				}
				else
					++iter2;
			}

			// ��������� ����� ������ �����մϴ�.
			{
				std::shared_ptr<DUOLGameEngine::GameObject> gameObject = iter->first;

				gameObject->OnDestroy();

				// OnDestroy�� �Ϸ�� �ڽ� ������Ʈ���� ����Ʈ���� �����մϴ�. _gameObjectsInScene ���� �����մϴ�.
				// �޸𸮰� �����˴ϴ�.
				auto&& children = gameObject->GetTransform()->GetChildGameObjects();

				for (auto child : children)
				{
					for (auto iter2 = _gameObjectsInScene.begin(); iter2 != _gameObjectsInScene.end();)
					{
						if (child == (*iter2).get())
						{
							iter2 = _gameObjectsInScene.erase(iter2);

							break;
						}
						else
							++iter2;
					}
				}

				DUOLGameEngine::PhysicsManager::GetInstance()->UnInitializePhysicsGameObject(gameObject.get(), true);

				gameObject.reset();
			}

			// ���� ī��Ʈ 0 !
			iter = _gameObjectsForDestroy.erase(iter);
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

	void Scene::RemoveInRootObjectsList(DUOLGameEngine::GameObject* gameObject)
	{
		std::erase_if(_rootObjectsInScene, [&gameObject](const std::shared_ptr<DUOLGameEngine::GameObject>& item)
			{
				return (gameObject == item.get());
			});
	}

	void Scene::AddInRootObjectsList(DUOLGameEngine::GameObject* gameObject)
	{
		_rootObjectsInScene.push_back(gameObject->shared_from_this());
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

		// ó���� ������� �� ��� ������Ʈ���� ���ٵ� ?
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

				DUOLGameEngine::Transform* boneTransform = boneGO->GetTransform();

				auto&& bone = bones[i];

				boneObjects.push_back(boneGO);

				boneOffsetMatrices.push_back(bone._offsetMatrix);

				if ((bone._parentIndex == -1) || (boneObjects.size() == 1))
					boneTransform->SetParent(gameObject->GetTransform());
				else if (bone._parentIndex == 1)
					boneTransform->SetParent(boneObjects[0]->GetTransform());
				else
					boneTransform->SetParent(boneObjects[bone._parentIndex]->GetTransform());
					// boneTransform->SetParent(boneObjects[bone._parentIndex - 1]->GetTransform());

				boneGO->SetName(bone._boneName);

				// ���� ���� ��� ������ Ȯ���ϱ� ���ؼ� �޾Ƴ����ϴ�.
#ifdef _DEBUG
				DUOLGameEngine::Mesh* engineMesh = DUOLGameEngine::ResourceManager::GetInstance()->GetMesh(TEXT("Cube"));

				boneGO->AddComponent<DUOLGameEngine::MeshRenderer>()->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(_T("Debug")));

				boneGO->AddComponent<DUOLGameEngine::MeshFilter>()->SetMesh(engineMesh);;
#endif
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

				auto skinnedMeshRenderer = meshGO->GetComponent<DUOLGameEngine::SkinnedMeshRenderer>();

				for (int subMeshIndex = 0; subMeshIndex < engineMesh->GetPrimitiveMesh()->GetSubMeshCount(); subMeshIndex++)
				{
					skinnedMeshRenderer->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(engineMesh->GetPrimitiveMesh()->GetSubMesh(subMeshIndex)->_materialName));
				}

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

			auto& particleData = ParticleObject->AddComponent<DUOLGameEngine::ParticleRenderer>()->GetParticleData();

			if (parent)
				ParticleObject->GetTransform()->SetParent(parent->GetTransform());

			data._commonInfo._firstRun = true;

			data._objectID = ParticleObject->GetUUID();

			particleData = data;

			ParticleObject->GetTransform()->SetWorldTM(data._commonInfo._transformMatrix);

			auto objectID = DUOLCommon::StringHelper::ToTString(data._objectID);
			auto texturePath = DUOLCommon::StringHelper::ToTString(data._commonInfo._refTexturePath);
			auto textureID = texturePath.substr(texturePath.find_last_of(_T("/\\")) + 1);

			auto mat = DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(DUOLCommon::StringHelper::ToTString(data._objectID));

			if (mat == nullptr)
			{
				mat = DUOLGameEngine::ResourceManager::GetInstance()->CreateMaterial(objectID, textureID, _T("Particle"));
			}

			ParticleObject->GetComponent<DUOLGameEngine::ParticleRenderer>()->AddMaterial(mat);
			ParticleObject->GetComponent<DUOLGameEngine::ParticleRenderer>()->Play();

			for (auto iter : data._childrens)
			{
				func(iter, ParticleObject);
			}

			std::vector<DUOLGraphicsEngine::RenderingData_Particle>().swap(particleData._childrens);

			return ParticleObject;
		};

		return func(data, nullptr);
	}

	const DUOLCommon::tstring& Scene::GetName() const
	{
		return _name;
	}

	const DUOLCommon::tstring& Scene::GetPath() const
	{
		return _path;
	}

	const std::vector<DUOLGameEngine::GameObject*> Scene::GetRootObjects() const
	{
		std::vector<DUOLGameEngine::GameObject*> ret{};

		for (auto&& rootObject : _rootObjectsInScene)
			ret.push_back(rootObject.get());

		return ret;
	}
}