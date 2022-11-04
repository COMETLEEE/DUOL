#pragma once




namespace Muscle
{
	/// <summary>
	/// �ż���
	/// �� ����� ��.
	/// ������Ʈ �Ŵ����� ������..
	/// 22.04.12
	/// </summary>
	class IScene
	{
	public:
		IScene(std::string sceneName);

		virtual ~IScene() = default;

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

		void* _inputData = nullptr; // ��Ե� ���� ������ �Ѱܾ��� ��쿡 �������.. �˾Ƽ� ����ȯ�ؼ� ����ϴ°ɷ�~
	};
}