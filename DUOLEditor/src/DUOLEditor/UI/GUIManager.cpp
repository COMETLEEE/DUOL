#include "DUOLEditor/UI/GUIManager.h"
#include "DUOLEditor/UI/Page/Page.h"

#include "DUOLEditor/Util/Font.h"

#include <d3d11.h>

#include "DUOLCommon/ImGui/imgui_impl_dx11.h"
#include "DUOLCommon/ImGui/imgui_impl_win32.h"

#include "DUOLGameEngine/Manager/GraphicsManager.h"
#include "DUOLGameEngine/Engine.h"

namespace DUOLEditor
{
	GUIManager::GUIManager() :
		_isDockingEnable(true)
	{
		
	}

	GUIManager::~GUIManager()
	{

	}

	void GUIManager::Initialize(HWND hWnd)
	{
		// D3D 디바이스를 열었습니다.
		isDeviceOpen = true;
		 
		// ImGui Context를 셋업합니다.
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;		// Enable Keyboard Controls
		//io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;			// Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;			// Enable Multi-Viewport / Platform Windows

		// ImGui Style을 셋업합니다.
		ImGui::StyleColorsDark();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		// 플랫폼 / 렌더러 백엔드를 셋업합니다.
		ImGui_ImplWin32_Init(hWnd);
		ImGui_ImplDX11_Init(static_cast<ID3D11Device*>(DUOLGameEngine::GraphicsManager::GetInstance()->GetGraphicsDevice()),
			static_cast<ID3D11DeviceContext*>(DUOLGameEngine::GraphicsManager::GetInstance()->GetGraphicsDeviceContext()));

		// 에디터 기본 스킨에 맞게 외형을 조정합니다.
		ApplyEditorStyle();

		// 폰트를 로드합니다.
		// 만약 아무런 폰트가 로드되어 있지 않다면 기본 폰트를 사용합니다.
		io.Fonts->AddFontDefault();
	}

	void GUIManager::UnInitialize()
	{
		// Clean Up IMGUI
		ImGui_ImplDX11_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();

		isDeviceOpen = false;
	}

	void GUIManager::ApplyEditorStyle()
	{
		ImGuiStyle* style = &ImGui::GetStyle();

		style->WindowPadding = ImVec2(15, 15);
		style->WindowRounding = 5.0f;
		style->FramePadding = ImVec2(5, 5);
		style->FrameRounding = 4.0f;
		style->ItemSpacing = ImVec2(12, 8);
		style->ItemInnerSpacing = ImVec2(8, 6);
		style->IndentSpacing = 25.0f;
		style->ScrollbarSize = 15.0f;
		style->ScrollbarRounding = 9.0f;
		style->GrabMinSize = 5.0f;
		style->GrabRounding = 3.0f;

		style->Colors[ImGuiCol_Text] = ImVec4(0.80f, 0.80f, 0.83f, 1.00f);
		style->Colors[ImGuiCol_TextDisabled] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style->Colors[ImGuiCol_WindowBg] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_ChildBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
		style->Colors[ImGuiCol_PopupBg] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
		style->Colors[ImGuiCol_Border] = ImVec4(0.2f, 0.2f, 0.2f, 0.88f);
		style->Colors[ImGuiCol_BorderShadow] = ImVec4(0.92f, 0.91f, 0.88f, 0.00f);
		style->Colors[ImGuiCol_FrameBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_FrameBgHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style->Colors[ImGuiCol_FrameBgActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.3f, 0.3f, 0.3f, 0.75f);
		style->Colors[ImGuiCol_TitleBgActive] = ImVec4(0.07f, 0.07f, 0.09f, 1.00f);
		style->Colors[ImGuiCol_MenuBarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
		style->Colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_CheckMark] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
		style->Colors[ImGuiCol_SliderGrab] = ImVec4(0.80f, 0.80f, 0.83f, 0.31f);
		style->Colors[ImGuiCol_SliderGrabActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_Button] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_ButtonHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style->Colors[ImGuiCol_ButtonActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_Header] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
		style->Colors[ImGuiCol_HeaderHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_HeaderActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_Separator] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_SeparatorHovered] = ImVec4(0.24f, 0.23f, 0.29f, 1.00f);
		style->Colors[ImGuiCol_SeparatorActive] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_ResizeGrip] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		style->Colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.56f, 0.56f, 0.58f, 1.00f);
		style->Colors[ImGuiCol_ResizeGripActive] = ImVec4(0.06f, 0.05f, 0.07f, 1.00f);
		style->Colors[ImGuiCol_PlotLines] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
		style->Colors[ImGuiCol_PlotLinesHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_PlotHistogram] = ImVec4(0.40f, 0.39f, 0.38f, 0.63f);
		style->Colors[ImGuiCol_PlotHistogramHovered] = ImVec4(0.25f, 1.00f, 0.00f, 1.00f);
		style->Colors[ImGuiCol_TextSelectedBg] = ImVec4(0.25f, 1.00f, 0.00f, 0.43f);
		style->Colors[ImGuiCol_ModalWindowDimBg] = ImVec4(1.00f, 0.98f, 0.95f, 0.73f);

		style->Colors[ImGuiCol_Tab] = style->Colors[ImGuiCol_TabUnfocused];
	}

	bool GUIManager::LoadFont(const DUOLCommon::tstring& fontID, const DUOLCommon::tstring& path, float fontSize)
	{
		if (!_fonts.contains(fontID))
		{
			ImGuiIO& io = ImGui::GetIO();

			ImFont* imFont = io.Fonts->AddFontFromFileTTF(DUOLCommon::StringHelper::ToString(path).c_str(), fontSize);

			if (imFont != nullptr)
			{
				_fonts.insert({ fontID, Font{fontID, imFont} });

				return true;
			}
		}

		return false;
	}

	bool GUIManager::UnloadFont(const DUOLCommon::tstring& fontID)
	{
		// ImGui System에서 현재 있는 폰트를 전부 내리고 이 fontID 만 빼고 다시 로드하는 방식으로
		// unload가 가능하기는 하다만 .. 의미가 있는지는 모르겠습니다. 
		if (_fonts.contains(fontID))
		{
			_fonts.erase(fontID);

			return true;
		}

		return false;
	}

	bool GUIManager::UseFont(const DUOLCommon::tstring& fontID)
	{
		// 폰트가 로드되어 있다면.
		if (_fonts.contains(fontID))
		{
			auto foundFont = _fonts.at(fontID);

			ImGui::GetIO().FontDefault = foundFont.GetImFont();

			return true;
		}

		return false;
	}

	void GUIManager::UseDefaultFont()
	{
		ImGui::GetIO().FontDefault = nullptr;
	}

	void GUIManager::SetIsEditorLayoutSave(bool value)
	{
		if (value)
			ImGui::GetIO().IniFilename = DUOLCommon::StringHelper::ToString(_layoutSaveFileName).c_str();
		else
			ImGui::GetIO().IniFilename = nullptr;
	}

	bool GUIManager::GetIsEditorLayoutSave() const
	{
		return (ImGui::GetIO().IniFilename != nullptr);
	}

	void GUIManager::SetEditorLayoutSaveFileName(const DUOLCommon::tstring& fileName)
	{
		_layoutSaveFileName = fileName;

		if (GetIsEditorLayoutSave())
			ImGui::GetIO().IniFilename = DUOLCommon::StringHelper::ToString(_layoutSaveFileName).c_str();
	}

	void GUIManager::SetEditorLayoutAutosaveFrequency(float frequency)
	{
		ImGui::GetIO().IniSavingRate = frequency;
	}

	float GUIManager::GetEditorLayoutAutosaveFrequency() const
	{
		return ImGui::GetIO().IniSavingRate;
	}

	void GUIManager::SetIsEnableDocking(bool value)
	{
		_isDockingEnable = value;

		if (_isDockingEnable)
			ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_DockingEnable;
		else
			ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_DockingEnable;
	}

	bool GUIManager::GetIsEnableDocking() const
	{
		return _isDockingEnable;
	}

	void GUIManager::ResetLayout(const DUOLCommon::tstring& config) const
	{
		ImGui::LoadIniSettingsFromDisk(DUOLCommon::StringHelper::ToString(config).c_str());
	}

	void GUIManager::SetPage(std::shared_ptr<DUOLEditor::Page> page)
	{
		RemovePage();

		_currentPage = page;
	}

	DUOLEditor::Page* GUIManager::GetPage(const DUOLCommon::tstring& tstringID)
	{
		return _currentPage.get();
	}

	void GUIManager::RemovePage()
	{
		if (_currentPage != nullptr)
			_currentPage.reset();

		_currentPage = nullptr;
	}

	void GUIManager::Update(float deltaTime)
	{
		// Draw Data 를 생성해냅니다.
		if (_currentPage != nullptr)
			_currentPage->Draw();
	}

	void GUIManager::OnResize(const uint32_t& screenWidth, const uint32_t& screenHeight)
	{
		// 현재 존재하는 UI들을 업데이트 !
	}

	void GUIManager::RenderGUI()
	{
		// TODO - 2. PrePresent();
		DUOLGameEngine::GraphicsManager::GetInstance()->PrePresent();

		ImDrawData* drawData = ImGui::GetDrawData();

		if (drawData != nullptr)
			ImGui_ImplDX11_RenderDrawData(drawData);

		ImGuiIO& io = ImGui::GetIO(); (void)io;

		// Update and Render additional Platform Windows
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
		}

		// TODO - 3. Present();
		DUOLGameEngine::GraphicsManager::GetInstance()->Present();
	}
}