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
		 * \brief ControllableView 는 상호작용 가능한 카메라를 통해 View를 움직입니다.
		 * Perspective projection camera object 입니다.
		 */
		std::shared_ptr<DUOLGameEngine::GameObject> _perspCameraObject;

		/**
		 * \brief ControllableView 는 상호작용 가능한 카메라를 통해 View를 움직입니다.
		 *
		 */
		std::shared_ptr<DUOLGameEngine::GameObject> _orthoCameraObject;

		/**
		 * \brief 선택된 게임 오브젝트입니다.
		 */
		DUOLGameEngine::GameObject* _selectedGameObject;

		DUOLGameEngine::Camera* _perspectiveCamera;

		DUOLGameEngine::Camera* _orthographicCamera;

	protected:
		/**
		 * \brief ControllableView에서는 클릭을 통해 뷰 상에서 오브젝트를 선택할 수 있습니다.
		 * \param currentTextureSize The size of the current screen texture size.
		 * \param mousePosition The position vector of mouse.
		 */
		uint64_t ObjectPicking(const DUOLMath::Vector2& currentTextureSize, const DUOLMath::Vector2& mousePosition);

	public:
		/**
		 * \brief ControllableView는 아이템이 Hover 상태에서 우클릭이 들어왔을 때 컨트롤링을 실시합니다.
		 * \param deltaTime 프레임 간격 시간
		 */
		virtual void Update(float deltaTime) override;
	};
}