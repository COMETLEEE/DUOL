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


	//��ü �߰�
	void ObjectManager::InsertObject(std::shared_ptr<GameObject> obj)
	{
		if (_isStart)
		{
			m_InsertObjects.emplace_back(obj);

			return;
		}

		if (!obj) return;
		m_vectorObjects.emplace_back(obj);

		// CreateObject�� ��ü�� �����ϸ� �ȴ�..!
		//for (auto& GameObjiter : obj->m_Childrens)
		//{
		//	InsertObject(GameObjiter);
		//}
	}

	//Ư�� ��ü ����
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

	//������Ʈ�� �ݶ��̴� ���� ����
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
		m_DeleteObjects.clear(); //���� ���� ������Ʈ��.

		for (auto& iter : m_InsertObjects)
		{
			iter->Finalize();
			iter.reset();
		}
		m_InsertObjects.clear(); //�߰� ���� ������Ʈ��.

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
		//��� ������Ʈ ������Ʈ ������
		for (auto& iter : m_vectorObjects)
		{
			if (iter->GetIsEnable())
				iter->Update();
		}

		//��� ������Ʈ ������Ʈ ������
		for (auto& iter : m_vectorObjects)
		{
			if (iter->GetIsEnable())
				iter->LateUpdate();
		}

		// �浹 ���� ���� ������Ʈ
		ColliderUpdate();

		// ������Ʈ ����. for�� ���߿� ������ �߰��� ���� �߻�.
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

		// ������Ʈ �߰�.
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
	 
	//��� ������Ʈ ����
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
		// y���� ���� ��� ���ҵ� �ϴ� ����Ʈ���� �ϰڴ�!
		//
		// ��ǥ�� �� �����Ӹ��� �޶�������??
		_QuadTree->StartSplit(m_Colliders);


		for (auto& iter : m_Colliders)
		{
			// ������ ����Ʈ ������ �����ϴ� ����� ã�ƾ��Ѵ�..! Todo : 
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
					if (col1tag == "" || col2tag == "" || // �±װ� ����ְų�,
						col1tag != col2tag // �±װ� �ٸ��� �浹.
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

				const auto& check = iter.second->m_CurColliders.find(prev.first); // ���� ������ �浹�� �ݶ��̴��� �ִ��� �˻�.
				if (check == iter.second->m_CurColliders.end()) // �浹 ���ϴ� ��!
				{
					prev.second->ExitEvent(iter.second);
					iter.second->ExitEvent(prev.second);
				}
				else
				{
					prev.second->StayEvent(iter.second);
					iter.second->StayEvent(prev.second);
				}
				// Enter�Լ��� �ݶ��̴����� �浹 �˻��� �� ȣ���ϰ��ִ�.
			}

		}

	}



}
