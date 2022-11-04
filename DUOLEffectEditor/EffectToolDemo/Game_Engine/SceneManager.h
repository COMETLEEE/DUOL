#pragma once
/// <summary>
/// �� �Ŵ���
/// �ż���
/// 22.04.12
/// </summary>
#include "IScene.h"
namespace Muscle
{

	class IScene;
	class SceneManager
	{
	public:
		SceneManager() = default;
		~SceneManager();

	private:
		std::vector<std::shared_ptr<IScene> >m_DeleteScene; //�� �����ϸ� ���� �� ���� �� ���.

		void DeleteScene(); // m_DeleteScene�� ���� ���� ��� ����.

	private:
		std::map<std::string, std::shared_ptr<IScene>> m_SceneMap;

	public:
		bool _isLoadSceneReserved;

		std::shared_ptr<IScene> m_pLoadScene = nullptr;

		std::shared_ptr<IScene> m_pCurrentScene = nullptr;

		std::shared_ptr<IScene> GetScene();

		void RegistScene(std::shared_ptr<IScene> scene);
		
		// �� �Լ��� ȣ���ϸ� ���� �����ӿ� ���� �ٲ��.
		void LoadScene(std::string sceneName);

		// ���� �ε� ���� ����Ǿ� �ִٸ�, ���� ������ ������Ʈ �� ���� ��ȯ�Ѵ�.
		void FlipScene();

		std::string GetCurrentSceneName();
	};


}
