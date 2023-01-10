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
		// 블라블라 게임 뷰 렌더 타겟 가져와서 세팅
		// 게임 뷰의 경우에는 플레이 중인지 .. 아닌지도 중요한건가 ..? 아니지 그냥 불러오는게 다일듯 ..?


		// 1. Game View의 이미지가 호출해야할 텍스처를 업데이트합니다.
		_image->_size = GetSafeSize();

		_image->_textureID = DUOLGameEngine::GraphicsManager::GetInstance()->GetShaderResourceAddress(TEXT("GameView"));
	}
}
