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
		 * \brief ControllableView 는 상호작용 가능한 카메라를 통해 View를 움직입니다.
		 */
		std::shared_ptr<DUOLGameEngine::GameObject> _cameraObject;

	public:
		/**
		 * \brief ControllableView는 아이템이 Hover 상태에서 우클릭이 들어왔을 때 컨트롤링을 실시합니다. 
		 * \param deltaTime 프레임 간격 시간
		 */
		virtual void Update(float deltaTime) override;
	};
}