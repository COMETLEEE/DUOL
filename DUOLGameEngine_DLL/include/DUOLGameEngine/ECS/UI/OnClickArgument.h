#pragma once
#include "DUOLGameEngine/ECS/Component/BehaviourBase.h"

namespace DUOLGameEngine
{
	class GameObject;
}

namespace DUOLGameEngine
{
	class DUOL_GAMEENGINE_API OnClickArgument final :public DUOLGameEngine::ObjectBase, public std::enable_shared_from_this<OnClickArgument>
	{
	public:
		OnClickArgument();

		OnClickArgument(DUOLGameEngine::GameObject* owner, const DUOLCommon::tstring& name = TEXT("OnClickArgument"));

		virtual ~OnClickArgument();

		void OnUpdate(float deltaTime);

		void Initialize();

	public:
		std::function<void(void)> _onClickVoidVoid;

		std::function<void(int)> _onClickVoidInt;

		std::function<void(bool)> _onClickVoidBool;

		std::function<void(std::string)> _onClickVoidString;

		std::function<void(float)> _onClickVoidFloat;


#pragma region FRIEND_CLASS
		friend class GameObject;

		RTTR_ENABLE(DUOLGameEngine::ObjectBase)

		RTTR_REGISTRATION_FRIEND
#pragma endregion
	};
}


