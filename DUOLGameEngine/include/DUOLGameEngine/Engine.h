#pragma once
#include <memory>
#include "DUOLGameEngine/Util/Defines.h"
#include "DUOLGameEngine/Util/EngineSpecification.h"

#include "DUOLMath/DUOLMath.h"
#include "DUOLCommon/Event/Event.h"
#include "Util/SingletonBase.h"

namespace DUOLGameEngine
{
	/**
	 * \brief ���� ����. .exe�� ����Ǿ� �ʱ� ���� ������ ���� �ʱ�ȭ�� �ϰ� Game Loop�� �ǽ��Ѵ�.
	 */
	class Engine : public SingletonBase<Engine>
	{
		DECLARE_SINGLETON(Engine);

		DELETE_COPY_MOVE(Engine)

	private:
		virtual ~Engine() override;

	public:
		void Initialize(const EngineSpecification& gameSpecification);

		void UnInitialize();

	private:
		EngineSpecification _engineSpec;
		
#pragma region EVENTS
	private:
		/**
		 * \brief 'DUOLGame.exe' Window �� ũ�Ⱑ ����Ǿ��� �� ȣ���� �̺�Ʈ
		 */
		void Resize(const uint32_t& screenWidth, const uint32_t& screenHeight);

		/**
		 * \brief Resize �̺�Ʈ�� ȣ��ǵ��� ��ϵ� �̺�Ʈ �ڵ鷯���� ����
		 */
		DUOLCommon::Event<void, const uint32_t&, const uint32_t&> _resizeEvent;

		DUOLCommon::Event<void, const uint32_t&, const uint32_t&>& GetResizeEvent() { return _resizeEvent; }
#pragma endregion

	public:
		/**
		 * \brief Call from Game application's message handler. Keep game message handler unless this function returns TRUE.
		 * \param hWnd Window handle.
		 * \param msg message from platform handler.
		 * \param wParam W parameter.
		 * \param lParam L parameter.
		 * \return if it returns true, than program going on 
		 */
		bool DUOLGameEngine_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);

		/**
		 * \brief ���� ���� ���� �ֱ⿡ ���� ������ �ǽ��մϴ�.
		 */
		void Update();

#pragma region FRIEND_CLASS
		friend class GraphicsManager;
#pragma endregion
	};
}
