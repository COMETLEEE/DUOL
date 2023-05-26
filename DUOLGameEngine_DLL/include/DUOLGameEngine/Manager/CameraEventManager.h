#pragma once
#include <unordered_map>

#include "DUOLGameEngine/Util/Defines.h"
#include "DUOLGameEngine/Util/SingletonBase.h"
#include "DUOLGameEngine/Util/CameraEventInfo.h"

namespace DUOLGameEngine
{
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
		void Play(UINT64 key);

	private:
		//  �̸��� key������ �޾� ��� ī�޶��� �̺�Ʈ�� ����Ѵ�. 
		std::unordered_map<UINT64, CameraEventInfo*> _cameraEvents;


	};

	
}
