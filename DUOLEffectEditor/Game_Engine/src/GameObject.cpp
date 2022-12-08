#include "GameObject.h"
#include "IComponents.h"
#include <assert.h>
#include "MuscleEngine.h"
#include "ObjectManager.h"
#include "Transform.h"

namespace Muscle
{

	GameObject::GameObject() : m_Parent(), m_isRender(true), m_isEnable(true), m_Tag(), m_ObjectID(0)
	{

	}

	GameObject::~GameObject()
	{
		m_Components.clear();
	}

	void GameObject::Start()
	{
		static UINT objectCount = 0;

		m_ObjectID = objectCount++;

		m_Name = "GameObject " + std::to_string(m_ObjectID);

		for (auto iter : m_Components)
		{
			iter->Start();
		}
	}

	void GameObject::Update()
	{
		if (!GetIsEnable()) return;
		for (auto iter : m_Components)
		{
			if (iter->GetIsEnable())
				iter->Update();
		}
	}

	void GameObject::LateUpdate()
	{
		if (!GetIsEnable()) return;
		for (auto iter : m_Components)
		{
			if (iter->GetIsEnable())
				iter->LateUpdate();
		}
	}

	void GameObject::Render()
	{
		if (!GetIsEnable()) return;
		// 컴포넌트의 렌더 돌리기
		for (auto iter : m_Components)
		{
			if (iter->GetIsEnable())
				iter->Render();
		}
	}

	void GameObject::SetTransform(std::shared_ptr<Transform> _transform)
	{
		m_Transform = _transform;
	}

	void GameObject::SetParent(std::shared_ptr<GameObject> _GameObject)
	{
		//부모가 없을때만 사용 가능.
		std::shared_ptr<GameObject> parent = m_Parent.lock();

		assert(!parent);

		m_Parent = _GameObject;

		m_Parent.lock()->m_Childrens.push_back(shared_from_this());
	}

	void GameObject::SetChild(std::shared_ptr<GameObject> _GameObject)
	{
		m_Childrens.push_back(_GameObject);

		_GameObject->m_Parent = this->weak_from_this();
	}

	void GameObject::SetName(std::wstring _Name)
	{
		std::string _Temp;

		_Temp.assign(_Name.begin(), _Name.end());

		SetName(_Temp);
	}

	void GameObject::SetTag(std::string _Tag)
	{
		for (auto& iter : m_Childrens)
		{
			iter->SetTag(_Tag);
		}

		m_Tag = _Tag;
	}

	bool GameObject::GetIsEnable()
	{

		return m_isEnable;
	}

	void GameObject::SetIsEnable(bool value)
	{
		m_isEnable = value;

		for (auto& iter : m_Childrens)
			iter->SetIsEnable(value);
	}

	std::shared_ptr<GameObject> GameObject::GetTopParent()
	{
		if (m_Parent.lock())
			return m_Parent.lock()->GetTopParent();
		else
			return shared_from_this();
	}

	std::shared_ptr<GameObject> GameObject::FindChildren(std::string _Name)
	{
		if (m_Name == _Name)
			return shared_from_this();

		for (auto iter : m_Childrens)
		{
			if (iter->m_Name == _Name)
				return iter;
			else
			{
				std::shared_ptr<GameObject> Temp = iter->FindChildren(_Name);

				if (Temp)
					return Temp;
			}
		}

		return nullptr;
	}

	std::shared_ptr<GameObject> GameObject::FindChildren(std::shared_ptr<GameObject> child)
	{
		if (shared_from_this() == child)
			return shared_from_this();

		for (auto iter : m_Childrens)
		{
			if (iter == child)
				return iter;
			else
			{
				std::shared_ptr<GameObject> Temp = iter->FindChildren(child);
				if (Temp)
					return Temp;
			}
		}

		return nullptr;
	}

	void GameObject::DeleteChildren(std::shared_ptr<GameObject> child)
	{
		if (m_Childrens.empty()) return;
		for (auto iter = m_Childrens.begin(); iter != m_Childrens.end(); iter++)
		{
			if (*iter == child)
			{
				iter = m_Childrens.erase(iter);
				break;
			}
			else
			{
				(*iter)->DeleteChildren(child);
			}
		}
	}

	void GameObject::Finalize()
	{
		m_Transform.reset();

		m_Parent.reset();

		for (auto& iter : m_Childrens)
		{
			iter.reset();
		}

		for (auto& iter : m_Components)
		{
			iter->Finalize();

			// 확인용으로 잠깐 풀어둠
			// iter.reset();
		}
	}

	void GameObject::SetIsRender(bool _bool)
	{
		for (auto& iter : m_Childrens)
		{
			iter->m_isRender = _bool;

			iter->SetIsRender(_bool);
		}

		m_isRender = _bool;
	}

	void GameObject::OnEnterCollision(std::shared_ptr<Collider> _Other)
	{
		for (auto& iter : m_Components)
		{
			iter->OnEnterCollision(_Other);
		}
	}

	void GameObject::OnStayCollision(std::shared_ptr<Collider> _Other)
	{
		for (auto& iter : m_Components)
		{
			iter->OnStayCollision(_Other);
		}
	}

	void GameObject::OnExitCollision(std::shared_ptr<Collider> _Other)
	{
		for (auto& iter : m_Components)
		{
			iter->OnExitCollision(_Other);
		}
	}
	std::shared_ptr<GameObject> CreateGameObject()
	{
		std::shared_ptr<GameObject> gameObject = std::make_shared<GameObject>();
		gameObject->AddComponent<Transform>();
		gameObject->SetTransform(gameObject->GetComponent<Transform>());
		MuscleEngine::GetInstance()->GetObjManager()->InsertObject(gameObject);
		return gameObject;
	}

	void DeleteGameObject(std::shared_ptr<GameObject> gameObject)
	{
		if (gameObject->GetParent())
			gameObject->GetParent()->DeleteChildren(gameObject);
		MuscleEngine::GetInstance()->GetObjManager()->DeleteObject(gameObject);
	}
}