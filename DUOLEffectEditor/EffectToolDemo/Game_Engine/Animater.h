#pragma once

#include "AnimationClip.h"

namespace Muscle
{
	/// <summary>
	/// �ִϸ�����~
	/// Play�� ���ϴ� �ִϸ��̼� �̸� �־�����~
	/// ������ ������ ����ؾ��ϳ�??
	/// �ִϸ��̼� Ŭ������ �̺��z Ű�� ����� �� �˸��� �ް� �ʹ�.
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

		void AddAnimEvent(std::string name, AnimEventKey& clip); // ������ �̺�Ʈ ���� ����� ������... �ϵ��ڵ��̳� Json�� ��������..

		void SetAnimSpeed(float speed, float delayTime);

		float GetSpeed() { return _speed; }
	public:

		void AddAnimClip(std::string name, std::shared_ptr <AnimationClip> clip); // ASE_Prefab�� ������Ʈ�� ������ �� ����Ѵ�.

		void EvnetUpdate(AnimEventKey& animEvent);// �� �Լ��� �ִϸ��̼� Ŭ������ ����� �Լ���.

		virtual void Start() override;

		virtual void Update() override;

		virtual void Render() override;

		virtual void Finalize() override;
	};
}