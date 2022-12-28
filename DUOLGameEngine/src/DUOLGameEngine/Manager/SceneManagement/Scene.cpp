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
		// 씬의 등록 오브젝트 리스트로 옮겨줍니다.
		for (auto iter = _gameObjectsForCreate.begin(); iter != _gameObjectsForCreate.end();)
		{
			if ((*iter)->GetTransform()->IsRootObject())
				_rootObjectsInScene.push_back(*iter);

			// 참조 카운트 유지를 위하여 가지고 있는다.
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

			// 물리 오브젝트라면 Physics Manager에 등록까지 ! => 단, 모든 오브젝트들이 여기 들어가 있으므로 recursively 않게 ..
			DUOLGameEngine::PhysicsManager::GetInstance()->InitializePhysicsGameObject((*iter).get(), false);

			// 루트 오브젝트라면 루트 오브젝트 리스트에 넣어줍니다.
			if (gameObject->GetTransform()->IsRootObject())
				_rootObjectsInScene.push_back(*iter);

			// 참조 카운트 유지를 위하여 가지고 있는다.
			_gameObjectsInScene.push_back(*iter);

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
					if (iter->first == *iter2)
					{
						iter2 = _rootObjectsInScene.erase(iter2);

						break;
					}
					else
						++iter;
				}
			}

			// 참조 카운트를 없애기 위해서 '_gameObjectsInScene' 에서도 제외해줍니다.
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

			// 재귀적으로 지우는 과정을 수행합니다.
			{
				std::shared_ptr<DUOLGameEngine::GameObject> gameObject = iter->first;

				gameObject->OnDestroy();

				// OnDestroy가 완료된 자식 오브젝트들을 리스트에서 제거합니다. _gameObjectsInScene 에서 제거합니다.
				// 메모리가 해제됩니다.
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

			// 참조 카운트 0 !
			iter = _gameObjectsForDestroy.erase(iter);
		}
	}

	void Scene::ActiveGameObjects()
	{
		// 예약된 녀석들을 활성화한다.
		for (auto iter = _gameObjectsForActive.begin(); iter != _gameObjectsForActive.end(); )
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
		// 게임 오브젝트는 shared_ptr을 통한 Control_Block 형성으로 관리된다.
		std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>(TEXT("EmptyObject"));

		gameObject->AddComponent<Transform>();

		gameObject->_scene = this->weak_from_this();

		// 처음에 만들어질 때 모든 오브젝트들이 들어갈텐데 ?
		RegisterCreateGameObject(gameObject.get());

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

				// 대충 본이 어떻게 생긴지 확인하기 위해서 달아놓습니다.
#ifdef _DEBUG
				DUOLGameEngine::Mesh* engineMesh = DUOLGameEngine::ResourceManager::GetInstance()->GetMesh(TEXT("Cube"));

				boneGO->AddComponent<DUOLGameEngine::MeshRenderer>()->AddMaterial(DUOLGameEngine::ResourceManager::GetInstance()->GetMaterial(_T("Debug")));

				boneGO->AddComponent<DUOLGameEngine::MeshFilter>()->SetMesh(engineMesh);;
#endif
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
			// 에러 출력.
		}
		/// 부스트 라이브러리 디시리얼라이즈 완료.


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