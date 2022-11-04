#pragma once

namespace Muscle
{

	class GameObject;
	class Animater;

	struct AnimEventKey
	{
		std::string _name; // �̺�Ʈ�� �̸�.
		float _targetFream; // ���° �����ӿ� �̺�Ʈ�� �߻�����.
		bool _eventKey = false; // �̺�Ʈ �߻� bool / ��ǥ �����ӿ� ���޽� true�� ��ȯ.
		std::shared_ptr<GameObject> _targetGameObject; // �Լ��� �����ų Ÿ�� ������Ʈ.
		std::function<void(std::shared_ptr<Muscle::GameObject>)> _eventfunc; // �̺�Ʈ �Լ�. // ��ǥ ������ ���޽� �Լ� ����.
	};

	struct AnimPositionTrack
	{
		// �� Ʈ���� � Hierarchy �༮�� Target�ΰ�.
		// Ÿ���� �������� ������ ���̾��Ű ������ Ÿ�� ���� ������ �˱� ����.
		std::vector<tstring> _targetHierarchy;

		std::vector<std::pair<unsigned, Vector3>> _posTrack;

		std::shared_ptr<GameObject> _gameObject; // ������ ������ ���ΰ�! �Ź� Ž���ϴ� ���� ���� ����. ĳ������
	};

	struct AnimRotationTrack
	{
		std::vector<tstring> _targetHierarchy;

		std::vector<std::pair<unsigned, Quaternion>> _rotTrack;

		std::shared_ptr<GameObject> _gameObject; // ������ ������ ���ΰ�!
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

		std::weak_ptr<GameObject> m_GameObject; // �ֻ��� �θ�!

		std::weak_ptr<Muscle::Animater> _observe; // �� �������� �ִϸ�����, ��ȯ������ ������~

		bool GetisPlaying() { return m_isPlay; }

		void Stop();

		void Pause();

		void Play();

		void Update();

	public:
		void Attach(std::shared_ptr<Muscle::Animater> observe); // ������ ���

		void AddAnimEvent(AnimEventKey& animevnetkey);

	private:
		void Notify(AnimEventKey& animEvent); // �� �������� ���������� �ൿ �˷��ֱ�.

	public:
		void Finalize();
	};
}