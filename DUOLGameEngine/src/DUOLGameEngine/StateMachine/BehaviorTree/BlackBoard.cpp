#include "DUOLGameEngine/StateMachine/BehaviorTree/BlackBoard.h"

namespace DUOLGameEngine
{
	BlackBoard::~BlackBoard()
	{
		for (auto& iter : _dataTable)
		{
			DataStructure& structure = iter.second;

			for (auto iter = structure.begin(); iter != structure.end(); iter++)
				iter->second._destructor(iter->second._data);
		}
	}
}