#pragma once
#include <unordered_map>

#include "DUOLGameEngine/Util/Defines.h"
#include "DUOLGameEngine/Util/SingletonBase.h"
#include "DUOLGameEngine/Util/CameraEventInfo.h"

namespace DUOLGameEngine
{
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

		void SetMainCamera(DUOLGameEngine::Camera* maincamera);

		void SetSequenceList(std::vector<int>& sequencelist);

		void SetSequenceMode(bool value);

		void SequencePlay();

		bool IsSequencePlay();

	private:
		//  �̸��� key������ �޾� ��� ī�޶��� �̺�Ʈ�� ����Ѵ�. 
		std::unordered_map<UINT64, CameraEventInfo*> _cameraEvents;

		bool _playMode;

		UINT64 _nowPlayKey;

		DUOLGameEngine::Camera* _mainCamera;

		float _currentTime;

		bool _isSequenceMode;

		bool _isNextSequence;

		bool _isSequenceSuccess;

		std::vector<int> _cameraSequenceList;

		int _sequenceIndex;
	};

	
}
