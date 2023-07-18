#pragma once
#include "DUOLClient/Export_Client.h"
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"
/**
 * \brief Á¡ÇÁ´ë
 */

namespace DUOLClient
{
	class DUOL_CLIENT_API JumpingBoard : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		JumpingBoard(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = DUOLCommon::StringHelper::ToTString("JumpingBoard"));

		virtual ~JumpingBoard() override;

		void CalculateDistance();

	private:
		float _maxHeight;

		float _speed;

		DUOLMath::Vector3 _distanceVector;

		float _currentFocusPoint;

		float _deltaT;

		float _moveTime;

		JumpingBoard* _targetJumpingBoard;

		DUOLGameEngine::GameObject* _usingObject;

	};
}