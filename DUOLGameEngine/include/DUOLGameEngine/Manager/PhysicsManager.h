/**

    @file      PhysicsManager.h
    @brief     물리와 관련된 흐름을 담당하는 매니저 클래스
    @details   ~
    @author    COMETLEE
    @date      4.11.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include "DUOLGameEngine/Util/SingletonBase.h"

namespace DUOLGameEngine
{
	class PhysicsManager : public SingletonBase<PhysicsManager>
	{
		DECLARE_SINGLETON(PhysicsManager)

		DELETE_COPY_MOVE(PhysicsManager)

	public:
        void Initialize();

        void UnInitialize();

        void Update(float deltaTime);
    };
}
