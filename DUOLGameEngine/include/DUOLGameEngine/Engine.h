#pragma once
#include "DUOLGameEngine/Util/Defines.h"

namespace DUOLGameEngine
{
	/**
	 * \brief ���� ����. ������ �ʱ�ȭ�� ���� �ʿ��� �������� ��Ƽ� �����Ѵ�.
	 */
	struct EngineSpecification
	{
		DUOLGameEngine::tstring startSceneName;

		HWND hWnd;

		uint32_t screenWidth;

		uint32_t screenHeight;
	};

	/**
	 * \brief ���� ����. .exe�� ����Ǿ� �ʱ� ���� ������ ���� �ʱ�ȭ�� �ϰ� Game Loop�� �ǽ��Ѵ�.
	 */
	class Engine
	{
	public:
		Engine(const EngineSpecification& gameSpecification);

		~Engine();

		DELETE_COPY_MOVE(Engine)

	private:
		void Initialize();

		void UnInitialize();

	public:
		/**
		 * \brief ���� ���� ���� �ֱ⿡ ���� ������ �ǽ��մϴ�.
		 */
		void Update();
	};
}