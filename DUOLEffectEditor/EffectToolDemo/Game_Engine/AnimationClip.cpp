#include "pch.h"
#include "AnimationClip.h"
//#include "..\GraphicsEngine\IMesh.h"

#define Speed_Offset 0.5f // 속도 보정 값..! 블렌더의 애니메이션 속도와 맞춰주기 위함..!
namespace Muscle
{

	AnimationClip::AnimationClip(AnimationKey& anikey, std::shared_ptr<GameObject> _GameObj) : FrameCount(0), m_isPlay(false), m_isPuase(false)
	{
		m_AnimKey = anikey;
		m_GameObject = _GameObj;
		_startFrame = 0;
		_endFrame = 0;
		for (auto& iter : m_AnimKey._animPosTrackList)
		{
			std::string name;
			name.assign(iter._targetHierarchy.back().begin(), iter._targetHierarchy.back().end());
			iter._gameObject = m_GameObject.lock()->FindChildren(name);
			_startFrame = min(iter._posTrack.begin()->first, _startFrame);
			_endFrame = max(iter._posTrack.back().first, _endFrame);
		}
		for (auto& iter : m_AnimKey._animRotTrackList)
		{
			std::string name;
			name.assign(iter._targetHierarchy.back().begin(), iter._targetHierarchy.back().end());
			iter._gameObject = m_GameObject.lock()->FindChildren(name);
			_startFrame = min(iter._rotTrack.begin()->first, _startFrame);
			_endFrame = max(iter._rotTrack.back().first, _endFrame);
		}
	}

	AnimationClip::~AnimationClip()
	{
		m_GameObject.reset();
		_observe.reset();
	}

	void AnimationClip::Stop()
	{
		FrameCount = (float)_startFrame;
		m_isPlay = false;

		for (auto& iter : m_AnimKey._animEvnetKeyList)
		{
			iter._eventKey = false;
			Notify(iter);
		}
	}

	void AnimationClip::Pause()
	{
		m_isPuase = !m_isPuase;
	}

	void AnimationClip::Play()
	{
		FrameCount = (float)_startFrame;
		m_isPlay = true;
		for (auto& iter : m_AnimKey._animEvnetKeyList)
		{
			iter._eventKey = false;
		}
	}

	void AnimationClip::Update()
	{
		if (!m_isPlay || m_isPuase) return;


		for (auto& iter : m_AnimKey._animPosTrackList)
		{
			for (unsigned int i = 1; i < iter._posTrack.size(); i++)
				if (iter._posTrack[i].first > FrameCount)
				{

					float Time1 = (float)iter._posTrack[i].first;
					float Time2 = (float)iter._posTrack[i - 1].first;
					float _t = (float)(FrameCount - Time2) / ((Time1 - Time2));

					XMVECTOR _Pred = XMLoadFloat3(&iter._posTrack[i - 1].second);
					XMVECTOR _Now = XMLoadFloat3(&iter._posTrack[i].second);

					XMVECTOR _interpolRot = XMVectorLerp(_Pred, _Now, _t);

					iter._gameObject->GetTransform()->SetPosition(_interpolRot);
					break;
				}
				else
				{
					// 다음
				}
		}

		for (auto& iter : m_AnimKey._animRotTrackList)
		{
			for (unsigned int i = 1; i < iter._rotTrack.size(); i++)
				if (iter._rotTrack[i].first > FrameCount)
				{

					float Time1 = (float)iter._rotTrack[i].first;
					float Time2 = (float)iter._rotTrack[i - 1].first;
					float _t = (float)(FrameCount - Time2) / ((Time1 - Time2));

					XMVECTOR _interpolRot =
						XMQuaternionSlerp(iter._rotTrack[i - 1].second, iter._rotTrack[i].second, _t);

					iter._gameObject->GetTransform()->SetQuaternion(_interpolRot);
					break;
				}
				else
				{
					// 다음
				}
		}
		for (auto& iter : m_AnimKey._animEvnetKeyList)
		{
			if (iter._targetFream < FrameCount)
			{
				if (!iter._eventKey) // 아직 실행 시킨적이 없다면..!
				{
					iter._eventKey = true;
					if (iter._eventfunc) // 등록한 함수가 있다면..!
						iter._eventfunc(iter._targetGameObject);
					Notify(iter); // 애니메이터에게 알려주자.
				}
			}
			else
			{
				// 다음
			}
		}


		if (FrameCount > _endFrame)
			Stop();

		FrameCount += CTime::GetGameDeltaTime() * 16000.0f * _observe.lock()->GetSpeed() * Speed_Offset;
	}

	void AnimationClip::Attach(std::shared_ptr<Animater> observe)
	{
		_observe = observe;
	}

	void AnimationClip::AddAnimEvent(AnimEventKey& animevnetkey)
	{
		m_AnimKey._animEvnetKeyList.push_back(animevnetkey);
	}

	void AnimationClip::Notify(AnimEventKey& animEvent)
	{
		_observe.lock()->EvnetUpdate(animEvent);
	}

	void AnimationClip::Finalize()
	{
		for (auto iter : m_AnimKey._animEvnetKeyList)
			iter._targetGameObject.reset();

		for (auto iter : m_AnimKey._animPosTrackList)
			iter._gameObject.reset();

		for (auto iter : m_AnimKey._animRotTrackList)
			iter._gameObject.reset();

		m_GameObject.reset();

		_observe.reset();
	}
}