#pragma once
#include <vector>

/// <summary>
/// ������ �ܼ��ϰ� ������
/// ������ ������!
/// 2022-03-29 �ż���
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
		std::vector<std::shared_ptr<GameObject>> m_vectorObjects; // ����� Ȯ���� ���� ����α� �ϴµ�.. ���߿� �ٽ� ������..

		std::vector<std::shared_ptr<GameObject>>& GetDeleteObjects() { return m_DeleteObjects; }

	public:
		inline bool GetIsStart() { return _isStart; }

		inline void SetIsStart(bool value) { _isStart = value; }

		//��ü ����
		void InsertObject(std::shared_ptr<GameObject> obj);

		//Ư�� ��ü ����
		void DeleteObject(std::shared_ptr<GameObject> obj);

		//��ü ����
		void DeleteAll();

		void Start();

		void Update();

		void Render();

		const std::unordered_map<UINT, std::shared_ptr<Collider>>& GetColliders() { return m_Colliders; }// ����� �� �� ��������.

		const std::shared_ptr<QuadTree>& GetQuadTree() { return _QuadTree; } // ����� �� �� ��������.

	private:
		std::shared_ptr<QuadTree> _QuadTree;

		void ColliderUpdate();

		bool _isStart;


	private:
		std::vector<std::shared_ptr<GameObject>> m_DeleteObjects; //���� ���� ������Ʈ��.

		std::vector<std::shared_ptr<GameObject>> m_InsertObjects; //�߰� ���� ������Ʈ��.

		std::unordered_map<UINT, std::shared_ptr<Collider>> m_Colliders;
	};

}