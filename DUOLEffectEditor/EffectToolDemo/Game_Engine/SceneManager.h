#pragma once
/// <summary>
/// 씬 매니저
/// 신성현
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
		std::vector<std::shared_ptr<IScene> >m_DeleteScene; //씬 변경하며 이전 씬 삭제 시 사용.

		void DeleteScene(); // m_DeleteScene에 값이 있을 경우 삭제.

	private:
		std::map<std::string, std::shared_ptr<IScene>> m_SceneMap;

	public:
		bool _isLoadSceneReserved;

		std::shared_ptr<IScene> m_pLoadScene = nullptr;

		std::shared_ptr<IScene> m_pCurrentScene = nullptr;

		std::shared_ptr<IScene> GetScene();

		void RegistScene(std::shared_ptr<IScene> scene);
		
		// 이 함수를 호출하면 다음 프레임에 씬이 바뀐다.
		void LoadScene(std::string sceneName);

		// 만약 로드 씬이 예약되어 있다면, 본격 프레임 업데이트 전 씬을 전환한다.
		void FlipScene();

		std::string GetCurrentSceneName();
	};


}
