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
		 */
		std::shared_ptr<DUOLGameEngine::GameObject> _cameraObject;

		DUOLGameEngine::Camera* _camera;

	protected:
		/**
		 * \brief ControllableView������ Ŭ���� ���� �� �󿡼� ������Ʈ�� ������ �� �ֽ��ϴ�.
		 * \param mousePosition The position vector of mouse.
		 */
		void ObjectPicking(const DUOLMath::Vector2& mousePosition);

	public:
		/**
		 * \brief ControllableView�� �������� Hover ���¿��� ��Ŭ���� ������ �� ��Ʈ�Ѹ��� �ǽ��մϴ�. 
		 * \param deltaTime ������ ���� �ð�
		 */
		virtual void Update(float deltaTime) override;
	};
}