#pragma once
#include "DUOLGameEngine/Util/Defines.h"
#include "DUOLGameEngine/Util/SingletonBase.h"
#include "DUOLMath/DUOLMath.h"

namespace DUOLCommon
{
	struct UUID;
}

namespace DUOLGameEngine
{
	class AudioListener;
	class AudioSource;
	class Text;
	class Button;

	enum class UIEVENTEnum
	{
		LoadScene,
		LoadImage
	};
}

namespace DUOLGameEngine
{
	class Camera;
	class Image;
	class GameObject;
}

namespace DUOLGameEngine
{
	class DUOL_GAMEENGINE_API UIManager : public SingletonBase<UIManager>
	{

		DECLARE_SINGLETON(UIManager)

		DELETE_COPY_MOVE(UIManager)

	private:
		virtual ~UIManager() override;

	public:
		void Initialize();

		void UnInitialize();

		void InitializeCurrentGameScene(const std::list<std::shared_ptr<DUOLGameEngine::GameObject>>& rootObjectsInScene);

		void Update(float deltaTime);

	public:
		void StartCutScene();

		void ResetScene();

		void CreateCanvas(GameObject* object);

		void CreateImage(Image* image);

		void CreateText(Text* text);

		void RemoveCanvas(GameObject* canvas);

		void RemoveImage(Image* image);

		void RemoveText(Text* text);

		Image* FindImage(std::string name);

		Button* FindButton(std::string name);

		Text* FindText(std::string name);

		void SetScrollBarUI(std::string name, float gauge);

		void SetScrollBarUIMaxGauge(std::string name, float gauge);

		void OnResize(int width, int height);

		void LoadSpriteFile();

		void LoadComponent(DUOLGameEngine::GameObject& gameobject);

		void CreateTextureFile();

		void CreateFontType();

		void Resolution(int num, Button* button);

		void ChangeImage(std::string nowimage, std::string changeimage);

		void SetActiveImage(std::string path, bool value);

		void SetActiveButton(std::string path, bool value);

		void SetActiveText(std::string path, bool value);

		void SetText(std::string path, std::string text);

		void LoadImage(std::string path);
		// Getter

		bool GetIsCanvas() { return _isCanvas; };

		GameObject* GetCanvas();

		DUOLGameEngine::Image* GetImage(DUOLCommon::UUID imageid);

		std::vector<DUOLCommon::tstring>& GetSpriteFileList() { return _imageFileNames; }

		GameObject* GetPickingGameObject() { return _nowPickingObject; }
		
		DUOLMath::Vector2& GetGameViewPosition() { return _gameViewPosition; }

		DUOLMath::Vector2& GetGameViewSize() { return _gameViewSize; }

		std::vector<DUOLCommon::tstring>& GetFontList() { return _fontList; }

		float GetScrollButtonData();

		DUOLMath::Vector2 GetPreScreenSize();
		// Setter

		void SetPickGameObject(GameObject* object) { _nowPickingObject = object; }

		void SetIsCanvas() { _isCanvas = !_isCanvas; }

		void SetGameViewPosition(const DUOLMath::Vector2& position) { _gameViewPosition = position; }

		void SetGameViewSize(const DUOLMath::Vector2& size) { _gameViewSize = size; }

		void SetScrollGauge(float& gauge) { _scrollGauge = gauge; }

		void SetPreScreenSize(DUOLMath::Vector2& prescreensize);

		void ReSetScrollGauge() { _scrollGauge = -1.0f; }

		void DestoryButtonTargetGameObject(DUOLGameEngine::GameObject* gameobject);

		void LoadResolution();

	private:
		bool _isCutScene;

		bool _isCanvas;

		std::vector<GameObject*> _canvasList;

		std::vector<Image*> _imageList;

		std::vector<Button*> _buttonList;

		std::vector<Text*> _textList;

		std::vector<DUOLCommon::tstring> _imageFileNames;

		std::vector<std::string> _fileNames;

		GameObject* _nowPickingObject;

		DUOLMath::Vector2 _gameViewPosition;

		DUOLMath::Vector2 _gameViewSize;

		std::vector<DUOLCommon::tstring> _fontList;

		DUOLMath::Vector2 _nowResolution;

		std::vector<DUOLMath::Vector2> _resolutions;

		DUOLMath::Vector2 _preScreenSize;

		float _scrollGauge;



	};

}
