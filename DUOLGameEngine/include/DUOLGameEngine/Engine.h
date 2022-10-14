#pragma once
#include "DUOLGameEngine/Util/Defines.h"

namespace DUOLGameEngine
{
	/**
	 * \brief 게임 엔진. .exe로 노출되어 초기화를 하고 (동시에 모든 모듈 함께 초기화)
	 * Game Loop를 실시한다.
	 */
	class Engine
	{
	public:
		Engine();

		~Engine();

		DELETE_COPY_MOVE(Engine)
	};
}