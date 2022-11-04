#include "pch.h"
#include "ObjectManager.h"
#include "QuadTree.h"
namespace Muscle
{
	ObjectManager::ObjectManager() :_isStart(false)
	{
		_QuadTree = std::make_shared<QuadTree>();
	}

	ObjectManager::~ObjectManager()
	{
		DeleteAll();
		_QuadTree.reset();
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
		m_vectorObjects.emplace_back(obj);

		// CreateObject로 객체를 생성하면 된다..!
		//for (auto& GameObjiter : obj->m_Childrens)
		//{
		//	InsertObject(GameObjiter);
		//}
	}

	//특정 객체 삭제
	void ObjectManager::DeleteObject(std::shared_ptr<GameObject> obj)
	{
		if (!obj) return;
		m_DeleteObjects.push_back(obj);

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
		for (auto& iter : m_vectorObjects)
		{
			iter->Finalize();
			iter.reset();
		}
		m_vectorObjects.clear();

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

		for (auto& iter : m_vectorObjects)
		{
			iter->Start();

			std::shared_ptr<Collider> _Collider = iter->GetComponent<Collider>();

			if (_Collider)
				m_Colliders.insert({ _Collider->_ColliderID ,_Collider });
		}
	}

	void ObjectManager::Update()
	{
		//모든 오브젝트 업데이트 돌리기
		for (auto& iter : m_vectorObjects)
		{
			if (iter->GetIsEnable())
				iter->Update();
		}

		//모든 오브젝트 업데이트 돌리기
		for (auto& iter : m_vectorObjects)
		{
			if (iter->GetIsEnable())
				iter->LateUpdate();
		}

		// 충돌 관련 로직 업데이트
		ColliderUpdate();

		// 오브젝트 삭제. for문 도중에 삭제나 추가시 에러 발생.
		for (auto iter = m_DeleteObjects.begin(); iter != m_DeleteObjects.end(); iter++)
		{

   			if ((*iter)->GetIsEnable())
			{
				(*iter)->SetIsEnable(false);
			}
			for (auto iter2 = m_vectorObjects.begin(); iter2 != m_vectorObjects.end(); iter2++)
			{
				if (*iter == *iter2)
				{
					std::shared_ptr<Collider> _Collider = (*iter2)->GetComponent<Collider>();
					if (_Collider)
					{
						m_Colliders.erase(_Collider->_ColliderID);
					}

					(*iter2)->Finalize();
					m_vectorObjects.erase(iter2);
					break;
				}
			}

			(*iter).reset();
			iter = m_DeleteObjects.erase(iter);
			if (iter == m_DeleteObjects.end())
				break;
		}

		// 오브젝트 추가.
		for (auto& iter : m_InsertObjects)
		{
			m_vectorObjects.emplace_back(iter);

			iter->Start();

			std::shared_ptr<Collider> _Collider1 = iter->GetComponent<Collider>();
			if (_Collider1)
				m_Colliders.insert({ _Collider1->_ColliderID ,_Collider1 });

		}

		if (!m_InsertObjects.empty())
			m_InsertObjects.clear();
	}
	 
	//모든 오브젝트 렌더
	void ObjectManager::Render()
	{
		for (auto& iter : m_vectorObjects)
		{
			if (iter->GetIsRender())
				iter->Render();
		}
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
