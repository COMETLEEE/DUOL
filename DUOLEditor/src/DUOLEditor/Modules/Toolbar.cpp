#include "DUOLEditor/Modules/Toolbar.h"

#include "DUOLEditor/UI/Widgets/Buttons/ButtonImage.h"
#include "DUOLGameEngine/Manager/GraphicsManager.h"
#include "DUOLGameEngine/Manager/ResourceManager.h"

#include "DUOLEditor/Modules/EditorEventManager.h"
#include "DUOLEditor/Util/RawTextures.h"

namespace DUOLEditor
{
	Toolbar::Toolbar(const DUOLCommon::tstring& title, bool isOpened,
		const DUOLEditor::PanelWindowSetting& windowSetting) :
		PanelWindow(title, isOpened, windowSetting)
	{
		// 1. 툴바 리소스 로드
		std::vector<uint64_t> playButtonData = BUTTON_PLAY;
		DUOLGameEngine::ResourceManager::GetInstance()->CreateTexture(TEXT("Button_Play"), 64, 64, playButtonData.size() * sizeof(uint64_t), playButtonData.data());

		std::vector<uint64_t> pauseButtonData = BUTTON_PAUSE;
		DUOLGameEngine::ResourceManager::GetInstance()->CreateTexture(TEXT("Button_Pause"), 64, 64, pauseButtonData.size() * sizeof(uint64_t), pauseButtonData.data());

		std::vector<uint64_t> stopButtonData = BUTTON_STOP;
		DUOLGameEngine::ResourceManager::GetInstance()->CreateTexture(TEXT("Button_Stop"), 64, 64, stopButtonData.size() * sizeof(uint64_t), stopButtonData.data());

		std::vector<uint64_t> nextButtonData = BUTTON_NEXT;
		DUOLGameEngine::ResourceManager::GetInstance()->CreateTexture(TEXT("Button_Next"), 64, 64, nextButtonData.size() * sizeof(uint64_t), nextButtonData.data());

		// 2. 버튼들 배치
		_playButton = AddWidget<DUOLEditor::ButtonImage>(DUOLGameEngine::GraphicsManager::GetInstance()->GetShaderResourceAddress(TEXT("Button_Play")), DUOLMath::Vector2(20.f, 20.f));
		_playButton->SetIsLineBreak(false);

		_pauseButton = AddWidget<DUOLEditor::ButtonImage>(DUOLGameEngine::GraphicsManager::GetInstance()->GetShaderResourceAddress(TEXT("Button_Pause")), DUOLMath::Vector2(20.f, 20.f));
		_pauseButton->SetIsLineBreak(false);

		_stopButton = AddWidget<DUOLEditor::ButtonImage>(DUOLGameEngine::GraphicsManager::GetInstance()->GetShaderResourceAddress(TEXT("Button_Stop")), DUOLMath::Vector2(20.f, 20.f));
		_stopButton->SetIsLineBreak(false);

		_nextButton = AddWidget<DUOLEditor::ButtonImage>(DUOLGameEngine::GraphicsManager::GetInstance()->GetShaderResourceAddress(TEXT("Button_Next")), DUOLMath::Vector2(20.f, 20.f));
		_nextButton->SetIsLineBreak(false);

		// 3. 버튼들 이벤트 함수 지정
		_playButton->_clickedEvent += std::bind(&DUOLEditor::EditorEventManager::StartGame, DUOLEditor::EditorEventManager::GetInstance().get());
		_pauseButton->_clickedEvent += std::bind(&DUOLEditor::EditorEventManager::PauseGame, DUOLEditor::EditorEventManager::GetInstance().get());
		_stopButton->_clickedEvent += std::bind(&DUOLEditor::EditorEventManager::StopGame, DUOLEditor::EditorEventManager::GetInstance().get());
		_nextButton->_clickedEvent += std::bind(&DUOLEditor::EditorEventManager::NextFrame, DUOLEditor::EditorEventManager::GetInstance().get());

		// 4. 에디터 모드가 바뀔 때 버튼의 생김새 및 활성화 바뀌어야 한다. 에디터 이벤트 매니저에 이벤트 바인딩
		DUOLEditor::EditorEventManager::GetInstance()->GetEditorModeChangedEvent() += [this](DUOLEditor::EditorMode editorMode)
		{
			auto enableButton = [](DUOLEditor::ButtonImage* buttonImage, bool enable)
			{
				buttonImage->_isDisabled = !enable;

				buttonImage->_tintColor = enable ? Color{ 1.0f, 1.0f, 1.0f, 1.0f } : Color{ 1.0f, 1.0f, 1.0f, 0.15f };
			};

			switch (editorMode)
			{
				case EditorMode::Edit:
				{
					enableButton(_playButton, true);
					enableButton(_pauseButton, false);
					enableButton(_stopButton, false);
					enableButton(_nextButton, false);

					break;
				}

				case EditorMode::Play:
				{
					enableButton(_playButton, false);
					enableButton(_pauseButton, true);
					enableButton(_stopButton, true);
					enableButton(_nextButton, true);

					break;
				}

				case EditorMode::Pause:
				{
					enableButton(_playButton, true);
					enableButton(_pauseButton, false);
					enableButton(_stopButton, true);
					enableButton(_nextButton, true);

					break;
				}

				case EditorMode::FRAME_BY_FRAME:
				{
					enableButton(_playButton, true);
					enableButton(_pauseButton, false);
					enableButton(_stopButton, true);
					enableButton(_nextButton, true);
					break;
				}
			}
		};

		// 5. 처음 시작은 에디트 모드이다.
		DUOLEditor::EditorEventManager::GetInstance()->SetEditorMode(DUOLEditor::EditorMode::Edit);

		// 6. Alignment callbacks
		_panelWindowCallbacksBefore += std::bind(&DUOLEditor::Toolbar::SetAlignmentButtons, this);
	}

	Toolbar::~Toolbar()
	{

	}

	void Toolbar::Draw_Impl()
	{
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(10, 10));

		PanelWindow::Draw_Impl();

		ImGui::PopStyleVar();
	}

	void Toolbar::SetAlignmentButtons()
	{
#pragma region Setting 4-Button Alignement
		// Set 4-Button Alignment
		ImGuiStyle& style = ImGui::GetStyle();

		float width = style.FramePadding.x + 20.f + style.FramePadding.x + 20.f + style.FramePadding.x + 20.f + style.FramePadding.x + 20.f + style.FramePadding.x +  (4 * style.ItemSpacing.x);

		float avail = ImGui::GetContentRegionAvail().x;
		float off = (avail - width) * 0.5f;
		if (off > 0.0f)
			ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);
#pragma endregion
	}
}
