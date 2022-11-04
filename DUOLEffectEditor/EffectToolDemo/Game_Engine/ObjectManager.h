#pragma once
#include <vector>

/// <summary>
/// 구조를 단순하게 만들어보자
/// 엔진을 만들자!
/// 2022-03-29 신성현
/// </summary>

namespace Muscle
{

	class GameObject;
	class Collider;
	class QuadTree;

	class ObjectManager
	{

	public:
		ObjectManager();
		~ObjectManager();
	public:
		std::vector<std::shared_ptr<GameObject>> m_vectorObjects; // 디버깅 확인을 위해 열어두긴 하는데.. 나중에 다시 숨기자..

		std::vector<std::shared_ptr<GameObject>>& GetDeleteObjects() { return m_DeleteObjects; }

	public:
		inline bool GetIsStart() { return _isStart; }

		inline void SetIsStart(bool value) { _isStart = value; }

		//객체 생성
		void InsertObject(std::shared_ptr<GameObject> obj);

		//특정 객체 삭제
		void DeleteObject(std::shared_ptr<GameObject> obj);

		//전체 삭제
		void DeleteAll();

		void Start();

		void Update();

		void Render();

		const std::unordered_map<UINT, std::shared_ptr<Collider>>& GetColliders() { return m_Colliders; }// 디버깅 할 때 쓰기위함.

		const std::shared_ptr<QuadTree>& GetQuadTree() { return _QuadTree; } // 디버깅 할 때 쓰기위함.

	private:
		std::shared_ptr<QuadTree> _QuadTree;

		void ColliderUpdate();

		bool _isStart;


	private:
		std::vector<std::shared_ptr<GameObject>> m_DeleteObjects; //삭제 예정 오브젝트들.

		std::vector<std::shared_ptr<GameObject>> m_InsertObjects; //추가 예정 오브젝트들.

		std::unordered_map<UINT, std::shared_ptr<Collider>> m_Colliders;
	};

}