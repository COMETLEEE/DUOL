#pragma once

namespace Muscle
{
	class Frustum
	{
	public:
		Frustum(std::shared_ptr<GameObject> _GameObj); //부모를 게임 오브젝트를 인자로 받고 초기화
		~Frustum();

	private:
		//Camera* m_Camera;
		//GameObject* m_GameObject;
		//XMFLOAT3 m_VertexPos[8];
		//Plane* m_Plane[6];

		//XMVECTOR m_WorldNomal[6];
		//XMVECTOR m_WorldPoint[6];

	private:
		bool CheckDot(Vector3& _WorldVertexPos);
	public:
		void OnResize();
		void Update();
	};

}