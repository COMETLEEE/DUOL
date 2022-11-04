#pragma once

#include "AnimationClip.h"

namespace Muscle
{
	/// <summary>
	/// 애니메이터~
	/// Play로 원하는 애니메이션 이름 넣어주자~
	/// 옵저버 패턴을 사용해야하나??
	/// 애니메이션 클립에서 이벤틑 키를 사용할 때 알림을 받고 싶다.
	/// </summary>
	class Animater : public IComponents, public std::enable_shared_from_this<Animater>
	{
	public:
		Animater(std::shared_ptr<GameObject> _GameObj);

		virtual ~Animater();

	private:
		std::map<std::string, std::shared_ptr<AnimationClip>> m_AnimClips;

		std::map<std::string, bool> _eventBools;

		std::string m_PlayAnimName;

		bool _isPlay;

		float _speed;

		float _delayTime;

	public:
		void Play(std::string _AniName);

		void RePlay(std::string _AniName);

		void Stop();

		bool GetisPlay() { return _isPlay; }

		bool GetEventKey(std::string name);

		void AddAnimEvent(std::string name, AnimEventKey& clip); // 블랜더에 이벤트 삽입 기능은 없었다... 하드코딩이나 Json을 생각하자..

		void SetAnimSpeed(float speed, float delayTime);

		float GetSpeed() { return _speed; }
	public:

		void AddAnimClip(std::string name, std::shared_ptr <AnimationClip> clip); // ASE_Prefab이 오브젝트를 생성할 때 사용한다.

		void EvnetUpdate(AnimEventKey& animEvent);// 이 함수는 애니메이션 클립에서 사용할 함수다.

		virtual void Start() override;

		virtual void Update() override;

		virtual void Render() override;

		virtual void Finalize() override;
	};
}