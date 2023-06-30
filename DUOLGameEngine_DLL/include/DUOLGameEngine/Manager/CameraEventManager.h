#pragma once
#include <unordered_map>

#include "DUOLGameEngine/Util/Defines.h"
#include "DUOLGameEngine/Util/SingletonBase.h"
#include "DUOLGameEngine/Util/CameraEventInfo.h"

namespace DUOLGameEngine
{
	class Transform;
	class Camera;
	struct CameraEventInfo;

	class DUOL_GAMEENGINE_API CameraEventManager : public SingletonBase<CameraEventManager>
	{
		DECLARE_SINGLETON(CameraEventManager)

			DELETE_COPY_MOVE(CameraEventManager)

	private:
		virtual ~CameraEventManager();


	public:
		void Initialize();
		void UnInitialize();

		void Update(float deltaTime);

		/**
		 * \brief Json CameraEvent is all load
		 */
		void LoadCameraEvent(const DUOLCommon::tstring& path);

		/**
		 * \brief input key value play CameraEvent
		 * \param key
		 */
		void Play(float deltaTime);

		void SetPlayKey(UINT64 key);

		void SetMainCamera();

		// DUOLGameEngine::Camera* GetMainCamera();

		void SetSequenceList(std::vector<int>& sequencelist);

		void SetSequenceMode(bool value);

		void SequencePlay();

		bool IsSequencePlay();

		bool IsPlayMode();

		void PlayerAction(std::string& name, Transform* transform);

	private:
		//  이름을 key값으로 받아 모든 카메라의 이벤트를 등록한다. 
		std::unordered_map<UINT64, CameraEventInfo*> _cameraEvents;

		bool _playMode;

		UINT64 _nowPlayKey;

		Transform* _mainCameraTransform;

		Transform* _realCameraTransform;

		float _currentTime;

		bool _isSequenceMode;

		bool _isNextSequence;

		bool _isSequenceSuccess;

		std::vector<int> _cameraSequenceList;

		int _sequenceIndex;

		// Player action mode
		bool _isPlayerAction;

		DUOLMath::Matrix _playerMat;
	};
}
