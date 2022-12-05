#pragma once

#include <string>
#include <memory>


namespace Muscle
{
	/// <summary>
	/// �ż���
	/// �� ����� ��.
	/// ������Ʈ �Ŵ����� ������..
	/// 22.04.12
	/// </summary>
	class ObjectManager;

	class IScene
	{
	public:
		IScene(std::string sceneName);

		virtual ~IScene();

	private:
		// ���� �̸����� �ĺ��Ѵ�.
		std::string m_SceneName;

		//������Ʈ �����ּ���~
		std::shared_ptr<ObjectManager> m_ObjManager; //�����ּ���~!
		//�����ž��մϴ�.
	public:
		inline std::string GetSceneName() { return m_SceneName; }

		virtual void Start() abstract; // ī�޶� ���� �־��ּ���~ ������ ���� ī�޶� ���õ� �ϼ���~

		virtual void RapidUpdate() abstract; // ���� ������Ʈ.

		virtual void Update() abstract;

		std::shared_ptr<ObjectManager> GetObjManager();
	};
}