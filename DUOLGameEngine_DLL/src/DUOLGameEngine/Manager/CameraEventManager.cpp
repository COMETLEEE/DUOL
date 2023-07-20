#include "DUOLGameEngine/Manager/CameraEventManager.h"

#include "DUOLCommon//Log/LogHelper.h"

#include "DUOLCommon/Util/Hash.h"
#include "DUOLGameEngine/ECS/Component/Camera.h"
#include "DUOLGameEngine/ECS/Component/Transform.h"
#include "DUOLGameEngine/ECS/GameObject.h"
#include "DUOLGameEngine/Manager/InputManager.h"
#include "DUOLGameEngine/Manager/SoundManager.h"
#include "DUOLGameEngine/Manager/TimeManager.h"
#include "DUOLGameEngine/Manager/SceneManagement/SceneManager.h"
#include "DUOLJson/JsonReader.h"
#include "DUOLGameEngine/ECS/Component/FadeInOut.h"

namespace DUOLGameEngine
{
	CameraEventManager::CameraEventManager() :
		_playMode(false)
		, _nowPlayKey(-1)
		, _cameraEvents()
		, _currentTime(0)
		, _isSequenceMode(false)
		, _cameraSequenceList()
		, _isNextSequence(false)
		, _sequenceIndex(0)
		, _isSequenceSuccess(false)
		, _playerMat()
		, _mainCameraTransform(nullptr)
		, _realCameraTransform(nullptr)
		, _timaManager(nullptr)
	{
	}

	CameraEventManager::~CameraEventManager()
	{
	}

	void CameraEventManager::Update(float deltaTime)
	{

		if (_playMode)
		{
			if (_isSequenceMode)
				SequencePlay();
			Play(deltaTime);
		}

		// Use TotalScene
		if (_playMode == false && _isSequenceSuccess)
		{
			_isSequenceSuccess = false;
		}
	}

	void CameraEventManager::Initialize()
	{
		DUOL_INFO(DUOL_FILE, "CameraEventManager Initialize _ Camera Event Load");

		LoadCameraEvent(TEXT("Asset/DataTable/CameraSequences.json"));

		_timaManager = TimeManager::GetInstance();

		DUOL_INFO(DUOL_FILE, "CameraEventManager Initialize Success");
	}

	void CameraEventManager::UnInitialize()
	{
		_mainCameraTransform = nullptr;
		_realCameraTransform = nullptr;
		_timaManager = nullptr;
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

	void CameraEventManager::LoadScene()
	{
		_mainCameraTransform = nullptr;
		_realCameraTransform = nullptr;
		_timaManager = nullptr;

		_timaManager = TimeManager::GetInstance();
	}

	// 60 frame 
	void CameraEventManager::Play(float deltaTime)
	{
		if (_mainCameraTransform == nullptr)
			SetMainCamera();

		if(_timaManager==nullptr)
			_timaManager = TimeManager::GetInstance();

		// not same => camera action play
		_currentTime += (deltaTime / _timaManager->GetTimeScale());

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
					if (!_isSequenceMode)
					{
						_playMode = false;
						_sequenceIndex = 0;
					}
					_isNextSequence = true;

					_currentTime = 0.f;

					return;
				}

				if (currentFrame < frameinfo->_frame)
				{
					currKey = i - 1;

					nextKey = i;

					/*		_realCameraTransform->SetLocalPosition(DUOLMath::Vector3(0, 0, -10));

							_realCameraTransform->SetLocalEulerAngle(DUOLMath::Vector3(0, 0, 0));*/

					break;
				}
			}

			FrameInfo* currData = cameraevent->_frameInfo[currKey];

			FrameInfo* nextData = cameraevent->_frameInfo[nextKey];

			DUOLMath::Vector3 desiredPos = DUOLMath::Vector3::Lerp(currData->_position, nextData->_position, (currentFrame - currData->_frame) / (nextData->_frame - currData->_frame));

			DUOLMath::Quaternion desiredRot = DUOLMath::Quaternion::Slerp(currData->_rotation, nextData->_rotation, (currentFrame - currData->_frame) / (nextData->_frame - currData->_frame));

			if (_isPlayerAction)
			{
				// Mat
				DUOLMath::Matrix CamMat = Matrix::Identity * Matrix::CreateFromQuaternion(desiredRot) * Matrix::CreateTranslation(desiredPos);

				DUOLMath::Matrix totalMat = CamMat * _playerMat;

				// Decompose
				DUOLMath::Vector3 pos;
				DUOLMath::Quaternion rot;
				DUOLMath::Vector3 scale;

				totalMat.Decompose(scale, rot, pos);

				_mainCameraTransform->SetLocalPosition(pos);

				_mainCameraTransform->SetLocalRotation(rot);
			}
			else
			{
				_mainCameraTransform->SetLocalPosition(desiredPos);

				_mainCameraTransform->SetLocalRotation(desiredRot);

			}
			break;
		}
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
					if (!_isSequenceMode)
					{
						_playMode = false;
						_sequenceIndex = 0;
					}

					_isNextSequence = true;

					_currentTime = 0.f;

					// return first frame
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

			if (_isPlayerAction)
			{// Mat
				DUOLMath::Matrix CamMat = Matrix::Identity * Matrix::CreateFromQuaternion(desiredRot) * Matrix::CreateTranslation(desiredPos);

				DUOLMath::Matrix totalMat = CamMat * _playerMat;

				// Decompose
				DUOLMath::Vector3 pos;
				DUOLMath::Quaternion rot;
				DUOLMath::Vector3 scale;

				totalMat.Decompose(scale, rot, pos);

				_mainCameraTransform->SetLocalPosition(pos);

				_mainCameraTransform->SetLocalRotation(rot);
			}
			else
			{
				_mainCameraTransform->SetLocalPosition(desiredPos);

				_mainCameraTransform->SetLocalRotation(desiredRot);

			}
			break;
		}
		}
	}

	// change play key frame
	void CameraEventManager::SetPlayKey(UINT64 key)
	{
		_isSequenceSuccess = false;

		_nowPlayKey = key;

		if (_playMode == true)
			return;

		_playMode = true;
	}

	void CameraEventManager::SetMainCamera()
	{
		auto& allGOs = DUOLGameEngine::SceneManager::GetInstance()->GetCurrentScene()->GetAllGameObjects();

		// Transform Information
		for (auto gameObject : allGOs)
		{
			if (gameObject->GetTag() == TEXT("Camera"))
			{
				GameObject* obj = gameObject;

				_mainCameraTransform = gameObject->GetTransform();

				//_Cameralist.push_back(obj);
			}
			if (gameObject->GetTag() == TEXT("MainCamera"))
			{
				GameObject* obj = gameObject;

				_realCameraTransform = gameObject->GetTransform();

				//_realCameralist.push_back(obj);

			}
		}
	}


	void CameraEventManager::SetSequenceList(std::vector<UINT64>& sequencelist)
	{
		_cameraSequenceList = sequencelist;
	}

	void CameraEventManager::SetSequenceMode(bool value)
	{
		_isPlayerAction = false;

		_isNextSequence = false;

		_isSequenceMode = value;

		if (_mainCameraTransform == nullptr)
			SetMainCamera();

		if (!_cameraSequenceList.empty())
		{
			_currentTime = 0.f;
			SetPlayKey(_cameraSequenceList[_sequenceIndex]);
			_sequenceIndex++;
		}
	}

	void CameraEventManager::SequencePlay()
	{
		if (_isNextSequence)
		{
			_isNextSequence = false;

			if (_sequenceIndex < _cameraSequenceList.size())
			{
				SetPlayKey(_cameraSequenceList[_sequenceIndex]);
				_sequenceIndex++;
			}
			else
			{
				_playMode = false;
				_isSequenceMode = false;
				_isSequenceSuccess = true;
				_sequenceIndex = 0;
			}
		}
	}

	bool CameraEventManager::IsSequencePlay()
	{
		if (_isSequenceSuccess && !_isNextSequence && !_isSequenceMode)
			return true;
		else
			return false;
	}

	bool CameraEventManager::IsPlayMode()
	{
		return _playMode;
	}

	void CameraEventManager::PlayerAction(std::string& name, Transform* transform)
	{
		UINT64 key = GetKey(name);

		SetPlayKey(key);
		_isPlayerAction = true;

		if (_mainCameraTransform == nullptr)
			SetMainCamera();

		_realCameraTransform->SetLocalPosition(Vector3(0, 0, 0));
		_realCameraTransform->SetLocalRotation(Vector4(0, 0, 0, 0));

		_mainCameraTransform->SetLocalPosition(Vector3(0, 0, 0));
		_mainCameraTransform->SetLocalRotation(Vector4(0, 0, 0, 0));

		auto mat = transform->GetWorldMatrix();

		// 회전 행렬 빼야한다.
		DUOLMath::Vector3 pos; Quaternion quaternion; DUOLMath::Vector3 scale;

		mat.Decompose(scale, quaternion, pos);

		_playerMat = DUOLMath::Matrix::CreateTranslation(pos);

	}

	UINT64 CameraEventManager::GetKey(std::string name)
	{
		UINT64 key = DUOLCommon::Hash::Hash64(DUOLCommon::StringHelper::ToTString(name));

		return key;
	}


}
