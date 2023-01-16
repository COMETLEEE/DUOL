#pragma once
#include "DUOLGameEngine/Util/SingletonBase.h"
#include "DUOLGameEngine/Util/Defines.h"

namespace DUOLGameEngine
{
	class UtillKeyInput : public SingletonBase<UtillKeyInput>
	{
		DECLARE_SINGLETON(UtillKeyInput)

		DELETE_COPY_MOVE(UtillKeyInput)

	private:
		virtual ~UtillKeyInput() override;

	private:
		bool _isConsole;

	public:
		void Initialize();

		void UnInitialize();

		void Update();
	};
}
