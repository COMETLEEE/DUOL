#include "DUOLEditor/Modules/ViewBase.h"

namespace DUOLGameEngine
{
	class GameObject;

	class Camera;
}

namespace DUOLEditor
{
	class ControllableViewBase : public DUOLEditor::ViewBase
	{
	public:
		ControllableViewBase(const DUOLCommon::tstring& title, bool isOpened, const PanelWindowSetting& windowSettings);

		virtual ~ControllableViewBase() override;

	protected:
		/**
		 * \brief ControllableView �� ��ȣ�ۿ� ������ ī�޶� ���� View�� �����Դϴ�.
		 * Perspective projection camera object �Դϴ�.
		 */
		std::shared_ptr<DUOLGameEngine::GameObject> _perspCameraObject;

		/**
		 * \brief ControllableView �� ��ȣ�ۿ� ������ ī�޶� ���� View�� �����Դϴ�.
		 *
		 */
		std::shared_ptr<DUOLGameEngine::GameObject> _orthoCameraObject;

		/**
		 * \brief ���õ� ���� ������Ʈ�Դϴ�.
		 */
		DUOLGameEngine::GameObject* _selectedGameObject;

		DUOLGameEngine::Camera* _perspectiveCamera;

		DUOLGameEngine::Camera* _orthographicCamera;

	protected:
		/**
		 * \brief ControllableView������ Ŭ���� ���� �� �󿡼� ������Ʈ�� ������ �� �ֽ��ϴ�.
		 * \param currentTextureSize The size of the current screen texture size.
		 * \param mousePosition The position vector of mouse.
		 */
		uint64_t ObjectPicking(const DUOLMath::Vector2& currentTextureSize, const DUOLMath::Vector2& mousePosition);

	public:
		/**
		 * \brief ControllableView�� �������� Hover ���¿��� ��Ŭ���� ������ �� ��Ʈ�Ѹ��� �ǽ��մϴ�.
		 * \param deltaTime ������ ���� �ð�
		 */
		virtual void Update(float deltaTime) override;
	};
}