#include "DUOLEditor/Modules/SceneView.h"

#include "DUOLEditor/UI/Widgets/Display/Image.h"

#include "DUOLGameEngine/Manager/GraphicsManager.h"

namespace DUOLEditor
{
	SceneView::SceneView(const DUOLCommon::tstring& title, bool isOpened, const PanelWindowSetting& windowSetting) :
		ControllableViewBase(title, isOpened, windowSetting)
	{

	}

	SceneView::~SceneView()
	{

	}

	void SceneView::Update(float deltaTime)
	{
		ControllableViewBase::Update(deltaTime);

		// 0. 이전에 Game View (== Game) 가 그려졌다.

		// 1. SceneView의 이미지가 호출해야할 텍스처를 업데이트합니다.
		_image->_size = GetSafeSize();

		_image->_textureID = DUOLGameEngine::GraphicsManager::GetInstance()->GetShaderResourceAddress(TEXT("SceneView"));
	}
}