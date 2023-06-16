#pragma once


#include "DUOLCommon/Event/Event.h"
#include "DUOLGameEngine/Util/Defines.h"
#include "DUOLGameEngine/Util/SingletonBase.h"

namespace DUOLGameEngine
{
	// 본인이 넣고자 하는 이벤트를 넣어주세요.
	enum class BUTTONEVENTEnum
	{
		LoadStartScene,
		LoadImGameScene,
		LoadEndGameScene
	};
}
/**
 * \brief button Event를 생성하고 관리하는 객체
 */
namespace DUOLGameEngine
{
	class DUOL_GAMEENGINE_API ButtonEventManager : public SingletonBase<ButtonEventManager>
	{
		DECLARE_SINGLETON(ButtonEventManager)

		DELETE_COPY_MOVE(ButtonEventManager)

	private:
		virtual ~ButtonEventManager();

	public:
		void Initialize();

		void UnInitialize();

		//void InitializeCurrentGameScene(const std::list<std::shared_ptr<DUOLGameEngine::GameObject>>& rootObjectsInScene);

		void Update(float deltaTime);

		void LoadScene(DUOLCommon::tstring& scenename);

		void LoadUnityScene(DUOLCommon::tstring& scenename);

		void EndGame();

	public:
		bool _exit;

	};
}
