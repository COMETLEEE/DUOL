#include "DUOLGameEngine/Manager/CameraEventManager.h"

#include "DUOLCommon//Log/LogHelper.h"

#include "DUOLCommon/Util/Hash.h"
#include "DUOLGameEngine/ECS/Component/Camera.h"
#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "DUOLGameEngine/Manager/InputManager.h"
#include "DUOLJson/JsonReader.h"

namespace DUOLGameEngine
{
	CameraEventManager::CameraEventManager() :
		_playMode(false)
		, _nowPlayKey(-1)
		, _cameraEvents()
		, _currentTime(0)
		, _isSequenceMode(false)
		,_cameraSequenceList()
	{
	}

	CameraEventManager::~CameraEventManager()
	{
	}

	void CameraEventManager::Update(float deltaTime)
	{
		if (DUOLGameEngine::InputManager::GetInstance()->GetKeyPressed(DUOLGameEngine::KeyCode::B))
		{
			SetPlayKey(0);
		}

		if (DUOLGameEngine::InputManager::GetInstance()->GetKeyPressed(DUOLGameEngine::KeyCode::N))
		{
			SetPlayKey(1);
		}
		if (DUOLGameEngine::InputManager::GetInstance()->GetKeyPressed(DUOLGameEngine::KeyCode::M))
		{
			SetPlayKey(2);
		}
		/*if (DUOLGameEngine::InputManager::GetInstance()->GetKeyPressed(DUOLGameEngine::KeyCode::O))
			SetPlayKey(0);*/

		if (_playMode)
		{
			if (_isSequenceMode)
				Play(deltaTime);

			Play(deltaTime);
		}
	}

	void CameraEventManager::Initialize()
	{
		DUOL_INFO(DUOL_FILE, "CameraEventManager Initialize _ Camera Event Load");

		LoadCameraEvent(TEXT("Asset/DataTable/CameraSequences.json"));

		DUOL_INFO(DUOL_FILE, "CameraEventManager Initialize Success");
	}

	void CameraEventManager::UnInitialize()
	{
	}

	void CameraEventManager::LoadCameraEvent(const DUOLCommon::tstring& path)
	{
		auto jsonReader = DUOLJson::JsonReader::GetInstance();

		auto cameraEventTable = jsonReader->LoadJson(path);

		const TCHAR* eventName = TEXT("sequenceName");

		const TCHAR* sequenceType = TEXT("sequenceType");

		const TCHAR* cameraKeyFrameDatas = TEXT("cameraKeyFrameDatas");

		uint64 eventID;

		// CameraKeyFrameSequences
		for (auto iter = cameraEventTable->MemberBegin(); iter != cameraEventTable->MemberEnd(); iter++)
		{
			//const DUOLCommon::tstring& name = iter->name.GetString(); //cameraKeyFrameSequences

			// In Sequeneces Array
			for (auto& cameraEvent : iter->value.GetArray())
			{
				CameraEventInfo* cameraEventInfo = new CameraEventInfo();

				if (cameraEvent.HasMember(eventName))
				{
					const DUOLCommon::tstring& name = cameraEvent[eventName].GetString();
					cameraEventInfo->_eventName = name;
					cameraEventInfo->_id = DUOLCommon::Hash::Hash64(name);
				}
				if (cameraEvent.HasMember(sequenceType))
					cameraEventInfo->_type = static_cast<SequenceType>(cameraEvent[sequenceType].GetInt64());
				if (cameraEvent[cameraKeyFrameDatas].IsArray())
				{
					for (auto iter = cameraEvent[cameraKeyFrameDatas].Begin(); iter != cameraEvent[cameraKeyFrameDatas].End(); iter++)
					{
						FrameInfo* frameInfo = new FrameInfo();

						frameInfo->_frame = (*iter)[L"frame"].GetInt64();

						frameInfo->_position.x = (*iter)[L"position"][L"x"].GetFloat();
						frameInfo->_position.y = (*iter)[L"position"][L"y"].GetFloat();
						frameInfo->_position.z = (*iter)[L"position"][L"z"].GetFloat();

						frameInfo->_rotation.x = (*iter)[L"rotation"][L"x"].GetFloat();
						frameInfo->_rotation.y = (*iter)[L"rotation"][L"y"].GetFloat();
						frameInfo->_rotation.z = (*iter)[L"rotation"][L"z"].GetFloat();
						frameInfo->_rotation.w = (*iter)[L"rotation"][L"w"].GetFloat();

						cameraEventInfo->_frameInfo.emplace_back(frameInfo);
					}
				}

				_cameraEvents.emplace(cameraEventInfo->_id, cameraEventInfo);

			}
		}
	}

	// 60 frame 
	void CameraEventManager::Play(float deltaTime)
	{
		if (_mainCamera == nullptr)
			_mainCamera = DUOLGameEngine::Camera::GetMainCamera();

		// not same => camera action play
		_currentTime += deltaTime;

		// 현재 시간에 맞는 프레임
		float currentFrame = _currentTime * 60;

		auto cameraevent = _cameraEvents[_nowPlayKey];

		switch (cameraevent->_type)
		{
		case SequenceType::Linear:
		{
			int currKey = 0;

			int nextKey = 0;

			for (int i = 0; i < cameraevent->_frameInfo.size(); i++)
			{

				FrameInfo* frameinfo = cameraevent->_frameInfo[i];

				// size is end and last Frame < now Frame 
				if (i == cameraevent->_frameInfo.size() - 1 && frameinfo->_frame < currentFrame)
				{
					_playMode = false;

					_currentTime = 0.f;

					// return first frame
					DUOLMath::Vector3 desiredPos = cameraevent->_frameInfo[0]->_position;
					DUOLMath::Vector4 desiredRot = cameraevent->_frameInfo[0]->_rotation;

					_mainCamera->GetTransform()->SetLocalPosition(desiredPos);

					_mainCamera->GetTransform()->SetLocalRotation(desiredRot);

					return;
				}

				if (currentFrame < frameinfo->_frame)
				{
					currKey = i - 1;

					nextKey = i;

					break;
				}
			}

			FrameInfo* currData = cameraevent->_frameInfo[currKey];

			FrameInfo* nextData = cameraevent->_frameInfo[nextKey];

			DUOLMath::Vector3 desiredPos = DUOLMath::Vector3::Lerp(currData->_position, nextData->_position, (currentFrame - currData->_frame) / (nextData->_frame - currData->_frame));

			DUOLMath::Quaternion desiredRot = DUOLMath::Quaternion::Slerp(currData->_rotation, nextData->_rotation, (currentFrame - currData->_frame) / (nextData->_frame - currData->_frame));

			_mainCamera->GetTransform()->SetLocalPosition(desiredPos);

			_mainCamera->GetTransform()->SetLocalRotation(desiredRot);

			break;
		}
		break;
		case SequenceType::Catmullrom:
		{
			int prevKey = 0;

			int currKey = 0;

			int nextKey = 0;

			int nextNextKey = 0;

			for (int i = 0; i < cameraevent->_frameInfo.size(); i++)
			{

				FrameInfo* frameinfo = cameraevent->_frameInfo[i];

				// size is end and last Frame < now Frame 
				if (i == cameraevent->_frameInfo.size() - 1 && frameinfo->_frame < currentFrame)
				{
					_playMode = false;

					_currentTime = 0.f;

					// return first frame
					DUOLMath::Vector3 desiredPos = cameraevent->_frameInfo[0]->_position;
					DUOLMath::Vector4 desiredRot = cameraevent->_frameInfo[0]->_rotation;

					_mainCamera->GetTransform()->SetLocalPosition(desiredPos);

					_mainCamera->GetTransform()->SetLocalRotation(desiredRot);

					return;
				}

				if (currentFrame < frameinfo->_frame)
				{
					if (i - 2 < 0)
						prevKey = 0;
					else
						prevKey = i - 2;

					if (i == 0)
					{
						currKey = 0;
						nextKey = 1;
					}
					else
					{
						currKey = i - 1;
						nextKey = i;
					}

					if (i + 1 >= cameraevent->_frameInfo.size())
						nextNextKey = i;
					else
						nextNextKey = i + 1;

					break;
				}
			}

			FrameInfo* prevData = cameraevent->_frameInfo[prevKey];

			FrameInfo* currData = cameraevent->_frameInfo[currKey];

			FrameInfo* nextData = cameraevent->_frameInfo[nextKey];

			FrameInfo* nextNextData = cameraevent->_frameInfo[nextNextKey];

			float t = (currentFrame - currData->_frame) / (nextData->_frame - currData->_frame);

			float t2 = t * t;

			float t3 = t2 * t;

			// P(t) = 0.5 * ((-b1 * P0 + b2 * P1 - b3 * P2 + b4 * P3) * t^3
			//	+(2 * b1 * P0 - (5 * b2 - 3) * P1 + (4 - 3 * b3) * P2 - b4 * P3) * t ^ 2
			//	+ (-b1 * P0 + b3 * P2) * t
			//	+ (2 * P1))
			float b1 = 0.5f * (-t3 + 2 * t2 - t);

			float b2 = 0.5f * (3 * t3 - 5 * t2 + 2);

			float b3 = 0.5f * (-3 * t3 + 4 * t2 + t);

			float b4 = 0.5f * (t3 - t2);

			DUOLMath::Vector3 desiredPos = b1 * prevData->_position + b2 * currData->_position + b3 * nextData->_position + b4 * nextNextData->_position;

			DUOLMath::Quaternion desiredRot = DUOLMath::Quaternion::Slerp(currData->_rotation, nextData->_rotation, (currentFrame - currData->_frame) / (nextData->_frame - currData->_frame));

			_mainCamera->GetTransform()->SetLocalPosition(desiredPos);

			_mainCamera->GetTransform()->SetLocalRotation(desiredRot);

			break;
		}
		}
	}

	// change play key frame
	void CameraEventManager::SetPlayKey(UINT64 key)
	{
		if (_playMode == true)
			return;

		_playMode = true;

		_nowPlayKey = key;
	}

	void CameraEventManager::SetMainCamera(DUOLGameEngine::Camera* maincamera)
	{
		_mainCamera = maincamera;
	}

	void CameraEventManager::SetSequenceList(std::vector<int>& sequencelist)
	{
		_cameraSequenceList = sequencelist;
	}
}
