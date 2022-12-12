/**

    @file      Animator.h
    @brief     Animation system component.
    @details   ~
    @author    COMETLEE
    @date      12.12.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include "DUOLGameEngine/ECS/Component/BehaviourBase.h"

namespace DUOLGameEngine
{
	/**
	 * \brief Interface to control the Mecanim animation system.
	 */
	class Animator : public DUOLGameEngine::BehaviourBase
	{
	public:
		Animator(const std::weak_ptr<DUOLGameEngine::GameObject> owner, const DUOLCommon::tstring& name = TEXT("Animator"));

		virtual ~Animator() override;

	public:
	};
}