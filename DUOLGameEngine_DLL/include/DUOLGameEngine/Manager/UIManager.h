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
	class Button;

	enum class UIEVENTEnum
	{
		MAIN,
		INGAME,
		OPTION,
		ENDDING,
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
		void LoadScene();

		void CreateCanvas(GameObject* object);

		void CreateImage(Image* image);

		void RemoveCanvas(GameObject* canvas);

		void RemoveImage(Image* image);

		void OnResize(int width, int height);

		void LoadSpriteFile();

		void LoadSpriteImage();

		void CreateTextureFile();

		// Getter

		bool GetIsCanvas() { return _isCanvas; };

		GameObject* GetCanvas() { if (_canvasList.empty()) return nullptr;  return _canvasList.back(); }

		DUOLGameEngine::Image* GetImage(DUOLCommon::UUID imageid);

		std::vector<DUOLCommon::tstring>& GetSpriteFileList() { return _imageFileNames; }

		GameObject* GetPickingGameObject() { return _nowPickingObject; }
		// Setter

		void SetPickGameObject(GameObject* object) { _nowPickingObject = object; }
		
		void SetIsCanvas() { _isCanvas = !_isCanvas; }

		//--------------------UI Event------------------------//
		//UIEVENTEnum UIEventStatus(UIEVENTEnum uieventid);

		

	private:
		bool _isCanvas;

		std::vector<GameObject*> _canvasList;

		std::vector<Image*> _imageList;

		std::vector<Button*> _buttonList;

		std::vector<DUOLCommon::tstring> _imageFileNames;

		std::vector<std::string> _fileNames;

		GameObject* _nowPickingObject;

	};
}
