#pragma once
#include "DUOLGameEngine/Util/Defines.h"

namespace DUOLGameEngine
{
	struct ApplicationCommandLineArgs
	{
		int _count = 0;
		char** _args = nullptr;

		const char* operator[](int index) const
		{

		}
	};

	/**
	 * \brief Windows Application의 구동을 담당하는 클래스
	 */
	class ApplicationManager
	{
	public:
		ApplicationManager() {};

		~ApplicationManager() {};

		DELETE_COPY_MOVE(ApplicationManager)

	private:

	public:
		
	};
}