#include "DUOLGameEngine/StateMachine/BehaviorTree/BlackBoard.h"

namespace DUOLGameEngine
{
	BlackBoard::~BlackBoard()
	{
		for (auto& iter : _dataTable)
		{
			DataStructure& structure = iter.second;

			for (auto iter = structure._dataStructure.begin(); iter != structure._dataStructure.end(); iter++)
				structure._destructor(iter->second);
		}
	}
}