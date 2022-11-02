/**

    @file      ResourceManager.h
    @brief     Resource Management class.
    @details   ~
    @author    COMETLEE
    @date      1.11.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include "DUOLGameEngine/Util/SingletonBase.h"
#include "DUOLGameEngine/Util/Defines.h"

namespace DUOLGameEngine
{
	/**
	 * \brief 게임 진행 중 리소스와 관련된 사항의
	 * 로드와 언로드, 다른 모듈들과의 연동을 담당하는 매니저
	 */
	class ResourceManager : public SingletonBase<ResourceManager>
	{
        DECLARE_SINGLETON(ResourceManager)

        DELETE_COPY_MOVE(ResourceManager)

	private:
        virtual ~ResourceManager() override;

	public:
        void Initialize();

        void UnInitialize();

        void Update(float deltaTime);
	};
}