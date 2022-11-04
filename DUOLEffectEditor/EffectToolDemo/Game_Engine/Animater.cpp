#include "pch.h"
#include "Animater.h"
//#include "..\GraphicsEngine\IMesh.h"

namespace Muscle
{
	Animater::Animater(std::shared_ptr<GameObject> _GameObj) :IComponents(_GameObj), _isPlay(false), _delayTime(0), _speed(1)
	{

	}

	Animater::~Animater()
	{
		m_AnimClips.clear();
	}



	void Animater::Play(std::string _AniName)
	{

		if (m_AnimClips.find(_AniName) != m_AnimClips.end())
		{
			if (_AniName == m_PlayAnimName)
			{
				if (!_isPlay)
				{
					_isPlay = true;
					m_AnimClips[m_PlayAnimName]->Play();
				}
			}
			else
			{
				m_AnimClips[m_PlayAnimName]->Stop();
				m_PlayAnimName = _AniName;
				_isPlay = true;
				m_AnimClips[m_PlayAnimName]->Play();
			}
		}
		else
		{
			// ::MessageBoxA(nullptr, "Animation NotFound", nullptr, MB_OK);
		}
	}

	void Animater::RePlay(std::string _AniName)
	{
		m_AnimClips[m_PlayAnimName]->Stop();
		Play(_AniName);
	}

	void Animater::Stop()
	{
		m_AnimClips[m_PlayAnimName]->Stop();
		_isPlay = false;
	}

	void Animater::AddAnimClip(std::string name, std::shared_ptr<AnimationClip> clip)
	{
		// 내가 등록한 애니메이션에게 옵저버 붙이기..

		m_AnimClips[name] = clip;
		m_AnimClips[name]->Attach(this->shared_from_this());
		m_PlayAnimName = name;
	}

	void Animater::AddAnimEvent(std::string name, AnimEventKey& clip)
	{
		const auto iter = m_AnimClips.find(name);

		if (iter == m_AnimClips.end())
			return; // 그런 애니메이션클립은 없다..!
		iter->second->AddAnimEvent(clip);

		_eventBools[clip._name] = clip._eventKey;
	}

	void Animater::SetAnimSpeed(float speed, float delayTime)
	{
		_speed = speed;
		_delayTime = delayTime;
	}
	bool Animater::GetEventKey(std::string name)
	{
		const auto iter = _eventBools.find(name);
		if (iter == _eventBools.end())
			return false; // 그런 이벤트는 없다..!
		return iter->second;
	}

	void Animater::EvnetUpdate(AnimEventKey& animEvent)
	{
		const auto& iter = _eventBools.find(animEvent._name); //못찾는다면 구조가 틀린 것.
		// 존재하지 않는 이름을 했을 확률이 큼.

		iter->second = animEvent._eventKey;
	}

	void Animater::Start()
	{
	}

	void Animater::Update()
	{
		if (!m_AnimClips[m_PlayAnimName] || !_isPlay)
			return;
		m_AnimClips[m_PlayAnimName]->Update();

		_isPlay = m_AnimClips[m_PlayAnimName]->GetisPlaying();



		if (_delayTime > 0)
			_delayTime -= CTime::GetGameDeltaTime();
		else
			_speed = 1;
	}


	void Animater::Render()
	{

	}

	void Animater::Finalize()
	{
		for (auto& iter : m_AnimClips)
		{
			iter.second->Finalize();

			iter.second.reset();
		}

		m_AnimClips.clear();
	}
}