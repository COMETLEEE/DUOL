#pragma once

#include "DUOLGameEngine/Util/SingletonBase.h"


namespace BT
{
	class BehaviorTreeFactory;
}

namespace DUOLGameEngine
{
	class DUOL_GAMEENGINE_API BehaviorTreeFactory : public SingletonBase<BehaviorTreeFactory>
	{
		DECLARE_SINGLETON(BehaviorTreeFactory);

		DELETE_COPY_MOVE(BehaviorTreeFactory);

	public:
		virtual ~BehaviorTreeFactory() override;



	};
}