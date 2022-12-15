#include "ObjectManager.h"
#include "QuadTree.h"
namespace Muscle
{
	ObjectManager::ObjectManager() :_isStart(false), _gameObjects(), m_DeleteObjects(), m_InsertObjects(), m_Colliders()
	{
		_QuadTree = std::make_shared<QuadTree>();
	}

	ObjectManager::~ObjectManager()
	{
		DeleteAll();

		_QuadTree.reset();

	}
	std::shared_ptr<GameObject> ObjectManager::GetGameObject(unsigned int objectID)
	{
		if (_gameObjects.find(objectID) != _gameObjects.end())
			return _gameObjects[objectID];
		else
			return nullptr;
	}

	//객체 추가
	void ObjectManager::InsertObject(std::shared_ptr<GameObject> obj)
	{
		if (_isStart)
		{
			m_InsertObjects.emplace_back(obj);

			return;
		}

		if (!obj) return;
		_gameObjects.insert({ obj->GetObjectID(),obj });
	}

	//특정 객체 삭제
	void ObjectManager::DeleteObject(std::shared_ptr<GameObject> obj)
	{
		if (!obj) return;

		m_DeleteObjects.push_back(obj);

		obj->_isDelete = true;

		if (!obj->GetChildrens().empty())
		{
			for (auto& iter : obj->GetChildrens())
			{
				DeleteObject(iter);
			}
		}

	}

	//오브젝트와 콜라이더 전부 삭제
	void ObjectManager::DeleteAll()
	{
		for (auto& iter : _gameObjects)
		{
			iter.second->Finalize();
			iter.second.reset();
		}
		_gameObjects.clear();

		for (auto& iter : m_Colliders)
		{
			iter.second->Finalize();
			iter.second.reset();
		}
		m_Colliders.clear();

		for (auto& iter : m_DeleteObjects)
		{
			iter->Finalize();
			iter.reset();
		}
		m_DeleteObjects.clear(); //삭제 예정 오브젝트들.

		for (auto& iter : m_InsertObjects)
		{
			iter->Finalize();
			iter.reset();
		}
		m_InsertObjects.clear(); //추가 예정 오브젝트들.

		_QuadTree->ClearNode();
	}

	void ObjectManager::Start()
	{
		_isStart = true;

		for (auto& iter : _gameObjects)
		{
			iter.second->Start();

			std::shared_ptr<Collider> _Collider = iter.second->GetComponent<Collider>();

			if (_Collider)
				m_Colliders.insert({ _Collider->_ColliderID ,_Collider });
		}
	}

	void ObjectManager::Update()
	{
		//모든 오브젝트 업데이트 돌리기
		for (auto& iter : _gameObjects)
		{
			if (iter.second->GetIsEnable())
				iter.second->Update();
		}

		//모든 오브젝트 업데이트 돌리기
		for (auto& iter : _gameObjects)
		{
			if (iter.second->GetIsEnable())
				iter.second->LateUpdate();
		}

		// 충돌 관련 로직 업데이트
		ColliderUpdate();

	}

	//모든 오브젝트 렌더
	void ObjectManager::Render()
	{
		for (auto& iter : _gameObjects)
		{
			if (iter.second->GetIsRender())
				iter.second->Render();
		}
	}
	void ObjectManager::DeleteInsertUpdate()
	{
		// 오브젝트 삭제. Update 도중이나 Render 도중에 삭제되면 이상이 발생할지도.. 모든 작업이 끝난 랜더 뒤에서 한다.



		for (auto iter : m_DeleteObjects)
		{

			if (iter->GetIsEnable())
			{
				iter->SetIsEnable(false);
			}
			auto temp = _gameObjects.find(iter->GetObjectID());
			if (temp != _gameObjects.end())
			{
				std::shared_ptr<Collider> _Collider = temp->second->GetComponent<Collider>();
				if (_Collider)
				{
					m_Colliders.erase(_Collider->_ColliderID);
				}
				temp->second->SetParent(nullptr);

				temp->second->Finalize();

				_gameObjects.erase(temp);
			}
			iter.reset();
		}
		m_DeleteObjects.clear();
		// 오브젝트 추가.
		for (auto& iter : m_InsertObjects)
		{
			_gameObjects.insert({ iter->GetObjectID(),iter });

			iter->Start();

			std::shared_ptr<Collider> _Collider1 = iter->GetComponent<Collider>();
			if (_Collider1)
				m_Colliders.insert({ _Collider1->_ColliderID ,_Collider1 });

		}
		if (!m_InsertObjects.empty())
			m_InsertObjects.clear();
	}
	void ObjectManager::ColliderUpdate()
	{
		// y축은 거의 사용 안할듯 하니 쿼드트리로 하겠다!
		//
		// 좌표가 매 프레임마다 달라지겠지??
		_QuadTree->StartSplit(m_Colliders);


		for (auto& iter : m_Colliders)
		{
			// 벡터의 스마트 포인터 복사하는 방법을 찾아야한다..! Todo : 
			iter.second->m_isCollided = false;
			iter.second->m_PrevColliders = iter.second->m_CurColliders;
			iter.second->m_CurColliders.clear();
		}
		for (auto& iter : _QuadTree->GetLeafNodes())
		{

			for (auto Col1 = iter->_values.begin(); Col1 != iter->_values.end(); Col1++)
			{
				if (!Col1->second->GetIsEnable()) continue;
				int a = 0;
				for (auto Col2 = Col1; Col2 != iter->_values.end(); Col2++)
				{
					if (!Col1->second->GetIsEnable()) continue;
					if (!Col2->second->GetIsEnable()) continue;
					if (Col1 == Col2)
						continue;
					const std::string& col1tag = (*Col1).second->GetGameObject()->GetTag();
					const std::string& col2tag = (*Col2).second->GetGameObject()->GetTag();
					if (col1tag == "" || col2tag == "" || // 태그가 비어있거나,
						col1tag != col2tag // 태그가 다르면 충돌.
						)
					{
						(*Col1).second->ColliderCheck(Col2->second);
					}
				}
			}
		}
		for (auto& iter : m_Colliders)
		{
			for (auto& prev : iter.second->m_PrevColliders)
			{
				prev.second->m_CurColliders;

				const auto& check = iter.second->m_CurColliders.find(prev.first); // 이전 프레임 충돌한 콜라이더가 있는지 검사.
				if (check == iter.second->m_CurColliders.end()) // 충돌 안하는 중!
				{
					prev.second->ExitEvent(iter.second);
					iter.second->ExitEvent(prev.second);
				}
				else
				{
					prev.second->StayEvent(iter.second);
					iter.second->StayEvent(prev.second);
				}
				// Enter함수는 콜라이더에서 충돌 검사할 때 호출하고있다.
			}

		}

	}



}
