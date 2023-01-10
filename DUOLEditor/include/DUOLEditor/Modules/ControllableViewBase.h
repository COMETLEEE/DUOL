#include "DUOLEditor/Modules/ViewBase.h"

namespace DUOLGameEngine
{
	class GameObject;
}

namespace DUOLEditor
{
	class ControllableViewBase : public DUOLEditor::ViewBase
	{
	public:
		ControllableViewBase(const DUOLCommon::tstring& title, bool isOpened, const PanelWindowSetting& windowSettings);

		virtual ~ControllableViewBase();

	private:
		/**
		 * \brief ControllableView �� ��ȣ�ۿ� ������ ī�޶� ���� View�� �����Դϴ�.
		 */
		std::shared_ptr<DUOLGameEngine::GameObject> _cameraObject;

	public:
		/**
		 * \brief ControllableView�� �������� Hover ���¿��� ��Ŭ���� ������ �� ��Ʈ�Ѹ��� �ǽ��մϴ�. 
		 * \param deltaTime ������ ���� �ð�
		 */
		virtual void Update(float deltaTime) override;
	};
}