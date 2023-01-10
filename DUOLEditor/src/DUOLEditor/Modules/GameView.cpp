#include "DUOLEditor/Modules/GameView.h"

#include "DUOLEditor/UI/Widgets/Display/Image.h"

#include "DUOLGameEngine/Manager/GraphicsManager.h"

namespace DUOLEditor
{
	GameView::GameView(const DUOLCommon::tstring& title, bool isOpened, const PanelWindowSetting& windowSetting) :
		ViewBase(title, isOpened, windowSetting)
	{

	}

	GameView::~GameView()
	{
	}

	void GameView::Update(float deltaTime)
	{
		// ����� ���� �� ���� Ÿ�� �����ͼ� ����
		// ���� ���� ��쿡�� �÷��� ������ .. �ƴ����� �߿��Ѱǰ� ..? �ƴ��� �׳� �ҷ����°� ���ϵ� ..?


		// 1. Game View�� �̹����� ȣ���ؾ��� �ؽ�ó�� ������Ʈ�մϴ�.
		_image->_size = GetSafeSize();

		_image->_textureID = DUOLGameEngine::GraphicsManager::GetInstance()->GetShaderResourceAddress(TEXT("GameView"));
	}
}
