#pragma once
#include <memory>
#include "DUOLGameEngine/Util/Defines.h"

namespace DUOLGameEngine
{
	class InputManager;

	class TimeManager;

	class GraphicsManager;

	/**
	 * \brief ���� ����. ������ �ʱ�ȭ�� ���� �ʿ��� �������� ��Ƽ� �����Ѵ�.
	 */
	struct EngineSpecification
	{
		DUOLCommon::tstring startSceneName;

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

	public:
		void Initialize();

		void UnInitialize();

	private:
		EngineSpecification _engineSpec;

	public:
		/**
		 * \brief ���� ���� ���� �ֱ⿡ ���� ������ �ǽ��մϴ�.
		 */
		void Update();
	};
}