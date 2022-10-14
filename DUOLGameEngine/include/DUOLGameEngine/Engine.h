#pragma once
#include "DUOLGameEngine/Util/Defines.h"

namespace DUOLGameEngine
{
	/**
	 * \brief ���� ����. .exe�� ����Ǿ� �ʱ�ȭ�� �ϰ� (���ÿ� ��� ��� �Բ� �ʱ�ȭ)
	 * Game Loop�� �ǽ��Ѵ�.
	 */
	class Engine
	{
	public:
		Engine();

		~Engine();

		DELETE_COPY_MOVE(Engine)
	};
}