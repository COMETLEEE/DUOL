#pragma once

namespace Muscle
{

	class GameObject;
	class Animater;

	struct AnimEventKey
	{
		std::string _name; // 이벤트의 이름.
		float _targetFream; // 몇번째 프레임에 이벤트가 발생할지.
		bool _eventKey = false; // 이벤트 발생 bool / 목표 프레임에 도달시 true로 변환.
		std::shared_ptr<GameObject> _targetGameObject; // 함수를 실행시킬 타겟 오브젝트.
		std::function<void(std::shared_ptr<Muscle::GameObject>)> _eventfunc; // 이벤트 함수. // 목표 프레임 도달시 함수 실행.
	};

	struct AnimPositionTrack
	{
		// 이 트랙은 어떤 Hierarchy 녀석이 Target인가.
		// 타겟이 여러개인 이유는 하이어라키 구조를 타고 들어가는 순서를 알기 위함.
		std::vector<tstring> _targetHierarchy;

		std::vector<std::pair<unsigned, Vector3>> _posTrack;

		std::shared_ptr<GameObject> _gameObject; // 누구를 움직일 것인가! 매번 탐색하는 것은 연산 낭비. 캐싱하자
	};

	struct AnimRotationTrack
	{
		std::vector<tstring> _targetHierarchy;

		std::vector<std::pair<unsigned, Quaternion>> _rotTrack;

		std::shared_ptr<GameObject> _gameObject; // 누구를 움직일 것인가!
	};

	struct AnimationKey
	{
		std::string _animName;
		std::vector<AnimPositionTrack> _animPosTrackList;
		std::vector<AnimRotationTrack> _animRotTrackList;
		std::vector<AnimEventKey> _animEvnetKeyList;
	};

	struct Bone
	{
		~Bone() { _boneObject.reset(); }

		uint32 _index;

		tstring _name;

		Matrix _boneNodeTM;

		std::shared_ptr<GameObject> _boneObject;
	};

	class AnimationClip
	{
	public:
		AnimationClip(AnimationKey& anikey, std::shared_ptr<GameObject> _GameObj);
		~AnimationClip();
	private:

		float FrameCount;

		int _startFrame;

		int _endFrame;

		bool m_isPlay;

		bool m_isPuase;

	public:
		AnimationKey m_AnimKey;

		std::weak_ptr<GameObject> m_GameObject; // 최상위 부모!

		std::weak_ptr<Muscle::Animater> _observe; // 날 관찰중인 애니메이터, 순환참조를 피하자~

		bool GetisPlaying() { return m_isPlay; }

		void Stop();

		void Pause();

		void Play();

		void Update();

	public:
		void Attach(std::shared_ptr<Muscle::Animater> observe); // 옵저버 등록

		void AddAnimEvent(AnimEventKey& animevnetkey);

	private:
		void Notify(AnimEventKey& animEvent); // 날 관찰중인 옵저버에게 행동 알려주기.

	public:
		void Finalize();
	};
}