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
		void ResetScene();

		void CreateCanvas(GameObject* object);

		void CreateImage(Image* image);

		// void RemoveCanvas(GameObject* canvas);

		void RemoveImage(Image* image);

		void OnResize(int width, int height);

		void LoadSpriteFile();

		void LoadComponent(DUOLGameEngine::GameObject& gameobject);

		void CreateTextureFile();

		void CreateFontType();
		// Getter

		bool GetIsCanvas() { return _isCanvas; };

		GameObject* GetCanvas();

		DUOLGameEngine::Image* GetImage(DUOLCommon::UUID imageid);

		std::vector<DUOLCommon::tstring>& GetSpriteFileList() { return _imageFileNames; }

		GameObject* GetPickingGameObject() { return _nowPickingObject; }

		DUOLMath::Vector2& GetGameViewPosition() { return _gameViewPosition; }

		DUOLMath::Vector2& GetGameViewSize() { return _gameViewSize; }

		std::vector<DUOLCommon::tstring>& GetFontList() { return _fontList; }

		// Setter

		void SetPickGameObject(GameObject* object) { _nowPickingObject = object; }

		void SetIsCanvas() { _isCanvas = !_isCanvas; }

		void SetGameViewPosition(const DUOLMath::Vector2& position) { _gameViewPosition = position; }

		void SetGameViewSize(const DUOLMath::Vector2& size) { _gameViewSize = size; }

		void MainUI(std::vector<GameObject*> object);

		void DestoryButtonTargetGameObject(DUOLGameEngine::GameObject* gameobject);
	private:
		bool _isCanvas;

		// 게임에서는 단하나의 Canvas만 가진다. 
		static GameObject* _canvas;

		std::vector<Image*> _imageList;

		std::vector<Button*> _buttonList;

		std::vector<Text*> _textList;

		std::vector<DUOLCommon::tstring> _imageFileNames;

		std::vector<std::string> _fileNames;

		GameObject* _nowPickingObject;

		DUOLMath::Vector2 _gameViewPosition;

		DUOLMath::Vector2 _gameViewSize;

		std::vector<DUOLCommon::tstring> _fontList;


	};
}
