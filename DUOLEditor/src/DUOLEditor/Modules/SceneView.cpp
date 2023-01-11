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

		// 0. ������ Game View (== Game) �� �׷�����.

		// 1. SceneView�� �̹����� ȣ���ؾ��� �ؽ�ó�� ������Ʈ�մϴ�.
		_image->_size = GetSafeSize();

		_image->_textureID = DUOLGameEngine::GraphicsManager::GetInstance()->GetShaderResourceAddress(TEXT("SceneView"));
	}
}