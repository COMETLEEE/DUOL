#pragma once

#include "Collider.h"

namespace Muscle
{
	class BoxCollider : public Collider
	{

	public:
		BoxCollider(std::shared_ptr<GameObject> _GameObject);

		virtual ~BoxCollider();

	private:
		DUOLMath::Vector3 _size;

	public:
		virtual void Finalize() override;

		void SetSize(DUOLMath::Vector3 size);

		DUOLMath::Vector3 GetSize();

		DUOLMath::Matrix GetWorldRotTM();
	public:
		virtual void ColliderCheck(std::shared_ptr<Collider> _Other) abstract;

		virtual void Start() abstract;

		virtual void Render() abstract; // ��� Object ���� ���� �� (ObjectManager) ������Ʈ�� Components�� ������ ������ ���� ����

		virtual void UpdateMinMax() override {};
	};
}
