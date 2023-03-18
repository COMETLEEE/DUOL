#include "DUOLEditor/Modules/ContentsBrowser.h"

#include "DUOLGameEngine/Manager/GraphicsManager.h"
#include "DUOLGameEngine/Manager/InputManager.h"
#include "DUOLGameEngine/Manager/ResourceManager.h"


namespace DUOLEditor
{
	ContentsBrowser::ContentsBrowser(const DUOLCommon::tstring panelName, bool isOpened,
		PanelWindowSetting panelSettings) :
		PanelWindow(panelName, isOpened, panelSettings)
	{
		// Project Path, Asset 스캔
		std::filesystem::path assetPath = TEXT("Asset");

		_rootDirectory = assetPath;

		_currentDirectory = assetPath;

		DUOLGraphicsLibrary::Texture* directoryIcon = DUOLGameEngine::ResourceManager::GetInstance()->CreateTexture(TEXT("Resource/Icon/DirectoryIcon.png"));

		_directoryIcon = directoryIcon->GetShaderResourceAddress();

		DUOLGraphicsLibrary::Texture* fileIcon = DUOLGameEngine::ResourceManager::GetInstance()->CreateTexture(TEXT("Resource/Icon/FileIcon.png"));

		_fileIcon = fileIcon->GetShaderResourceAddress();

		// 매번, _currentDirectory 체크 후, 달라져 있으면 Refresh() 호출.
		// TODO : 체크는 어떻게 ..? 디렉토리 State 를 매번 저장하면 되려나 ?
		_panelWindowCallbacksBefore += [this]()
		{
			Refresh();
		};
	}

	ContentsBrowser::~ContentsBrowser()
	{

	}

	void ContentsBrowser::Refresh()
	{
		// Back Button
		if (_currentDirectory != _rootDirectory)
		{
			if (ImGui::Button("Go to Back .."))
			{
				_currentDirectory = _currentDirectory.parent_path();
			}
		}

		static float padding = 16.0f;
		static float thumbnailSize = 128.0f;
		float cellSize = thumbnailSize + padding;

		if (GetIsHovered())
		{
			if (DUOLGameEngine::InputManager::GetInstance()->GetKeyPressed(DUOLGameEngine::KeyCode::LeftControl))
			{
				float mouseWheel = ImGui::GetIO().MouseWheel;

				thumbnailSize += mouseWheel;
			}
		}

		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;

		ImGui::Columns(columnCount, 0, false);

		// 일단, 디렉토리 체크법 알기 전까지는 하드코딩 가즈아
		for (auto& content : std::filesystem::directory_iterator(_currentDirectory))
		{
			std::filesystem::path contentPath = content.path();

			void* icon = content.is_directory() ? _directoryIcon : _fileIcon;

			std::string contentName = contentPath.filename().string();

			ImGui::PushID(contentName.c_str());
			ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));

			ImGui::ImageButton((ImTextureID)icon, { thumbnailSize, thumbnailSize }, { 0, 0 }, { 1, 1 });

			if (ImGui::BeginDragDropSource())
			{
				std::filesystem::path relativePath(contentPath);
				const wchar_t* itemPath = relativePath.c_str();
				ImGui::SetDragDropPayload("CONTENTS_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
				ImGui::Text(DUOLCommon::StringHelper::ToString(TEXT("Contents to ...")).c_str());
				ImGui::EndDragDropSource();
			}

			ImGui::PopStyleColor();

			if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
			{
				if (content.is_directory())
					_currentDirectory /= contentPath.stem();
			}

			ImGui::TextWrapped(contentPath.filename().string().c_str());
			ImGui::NextColumn();
			ImGui::PopID();
		}
	}
}
