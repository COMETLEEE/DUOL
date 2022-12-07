#include "BoxCollider.h"
#include "Transform.h"

Muscle::BoxCollider::BoxCollider(std::shared_ptr<GameObject> _GameObject) :Collider(_GameObject), _size(1.0f, 1.0f, 1.0f)
{

}

Muscle::BoxCollider::~BoxCollider()
{

}

void Muscle::BoxCollider::Finalize()
{
	__super::Finalize();
}

void Muscle::BoxCollider::SetSize(DUOLMath::Vector3 size)
{
	_size = size;
}

DUOLMath::Vector3 Muscle::BoxCollider::GetSize()
{
	DUOLMath::Vector3 tempSize;
	tempSize.x = m_Transform->GetWorldScale().x * _size.x;
	tempSize.y = m_Transform->GetWorldScale().y * _size.y;
	tempSize.z = m_Transform->GetWorldScale().z * _size.z;


	return tempSize;
}

DUOLMath::Matrix Muscle::BoxCollider::GetWorldRotTM()
{
	return m_Transform->GetWorldRotateTM();
}
