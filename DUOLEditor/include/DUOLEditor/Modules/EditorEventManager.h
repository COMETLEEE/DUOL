/**

    @file      EditorEventManager.h
    @brief     이벤트의 모든 feature에 대한 event managing을 담당하는 매니저 클래스
    @details   ~
    @author    COMETLEE
    @date      28.12.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include "DUOLGameEngine/Util/SingletonBase.h"

#include "DUOLCommon/Event/Event.h"

namespace DUOLGameEngine
{
	class GameObject;
}

namespace DUOLEditor
{
	/**
	 * \brief 에디터의 수행 기능에 따라서 발생해야 하는 각 이벤트에 대해서
	 * 여러 UI의 이벤트 리스너들을 이 매니저에 등록하고 호출합니다.
	 */
	class EditorEventManager : public DUOLGameEngine::SingletonBase<EditorEventManager>
	{
		DECLARE_SINGLETON(EditorEventManager)

		DELETE_COPY_MOVE(EditorEventManager)

	private:
		virtual ~EditorEventManager() override;

	public:
		void SelectGameObject(DUOLGameEngine::GameObject* gameObject);

		void UnselectGameObject();

	public:
		DUOLCommon::Event<void, DUOLGameEngine::GameObject*>& GetGameObjectSelectedEvent();

		DUOLCommon::Event<void>& GetGameObjectUnselectedEvent();

	private:
		DUOLCommon::Event<void, DUOLGameEngine::GameObject*> _gameObjectSelectedEvent;

		DUOLCommon::Event<void> _gameObjectUnselectedEvent;
	};
}
