#pragma once
#include "DUOLGameEngine/ECS/Component/MonoBehaviourBase.h"

#define MIDDLESCENEX 26
#define MIDDLESCENEY 27

#define MIDDLEXGAP 2
#define MIDDLEYGAP 7

namespace DUOLGameEngine
{
	class Image;
}

namespace DUOLClient
{
	class MiniMap : public DUOLGameEngine::MonoBehaviourBase
	{
	public:
		MiniMap(DUOLGameEngine::GameObject* owner = nullptr, const DUOLCommon::tstring& name = TEXT("MiniMap"));

		virtual ~MiniMap() override;

	public:
		void OnStart() override;

	private:


		DUOLGameEngine::Image* _miniMapImage;

		DUOLGameEngine::Image* _playerPosImage;


		RTTR_REGISTRATION_FRIEND

		RTTR_ENABLE(DUOLGameEngine::MonoBehaviourBase)
	};

}