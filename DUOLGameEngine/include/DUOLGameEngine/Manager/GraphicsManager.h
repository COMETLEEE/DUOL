/**

    @file      GraphicsManager.h
    @brief     Work With Rendering Engine
    @details   ~
    @author    COMETLEE
    @date      17.10.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/

#pragma once
#include "DUOLGameEngine/Util/Defines.h"
#include "DUOLGameEngine/Util/SingletonBase.h"

namespace DUOLGameEngine
{
	/**
	 * \brief 게임 진행 중 렌더링 엔진과 1대1로 대응하는 매니저
	 */
	class GraphicsManager : public SingletonBase<GraphicsManager>
	{
		DECLARE_SINGLETON(GraphicsManager)

		DELETE_COPY_MOVE(GraphicsManager)

	private:
		virtual ~GraphicsManager() override;

	public:

	};
}