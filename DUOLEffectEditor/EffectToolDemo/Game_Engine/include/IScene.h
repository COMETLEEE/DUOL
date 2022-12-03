#pragma once

#include <string>
#include <memory>


namespace Muscle
{
	/// <summary>
	/// 신성현
	/// 씬 만드는 중.
	/// 오브젝트 매니저는 씬마다..
	/// 22.04.12
	/// </summary>
	class ObjectManager;

	class IScene
	{
	public:
		IScene(std::string sceneName);

		virtual ~IScene();

	private:
		// 씬은 이름으로 식별한다.
		std::string m_SceneName;

		//업데이트 돌려주세용~
		std::shared_ptr<ObjectManager> m_ObjManager; //돌려주세요~!
		//돌리셔야합니다.
	public:
		inline std::string GetSceneName() { return m_SceneName; }

		virtual void Start() abstract; // 카메라 만들어서 넣어주세용~ 엔진에 메인 카메라 셋팅도 하세용~

		virtual void RapidUpdate() abstract; // 빠른 업데이트.

		virtual void Update() abstract;

		std::shared_ptr<ObjectManager> GetObjManager();
	};
}