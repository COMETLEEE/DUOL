#pragma once

#include <memory>
#include "SimpleMath.h"

namespace Muscle
{
	class GameObject;

	class Frustum
	{
	public:
		Frustum(std::shared_ptr<GameObject> _GameObj); //�θ� ���� ������Ʈ�� ���ڷ� �ް� �ʱ�ȭ
		~Frustum();

	private:
		//Camera* m_Camera;
		//GameObject* m_GameObject;
		//XMFLOAT3 m_VertexPos[8];
		//Plane* m_Plane[6];

		//XMVECTOR m_WorldNomal[6];
		//XMVECTOR m_WorldPoint[6];

	private:
		bool CheckDot(DirectX::SimpleMath::Vector3& _WorldVertexPos);
	public:
		void OnResize();
		void Update();
	};

}