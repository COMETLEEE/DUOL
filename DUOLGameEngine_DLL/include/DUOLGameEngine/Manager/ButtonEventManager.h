#pragma once


#include "DUOLCommon/Event/Event.h"
#include "DUOLGameEngine/Util/Defines.h"
#include "DUOLGameEngine/Util/SingletonBase.h"

namespace DUOLGameEngine
{
	// ������ �ְ��� �ϴ� �̺�Ʈ�� �־��ּ���.
	enum class BUTTONEVENTEnum
	{
		LoadStartScene,
		LoadImGameScene,
		LoadEndGameScene
	};
}
/**
 * \brief button Event�� �����ϰ� �����ϴ� ��ü
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
