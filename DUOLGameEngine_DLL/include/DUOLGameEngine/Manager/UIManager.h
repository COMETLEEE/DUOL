#pragma once
#include "DUOLGameEngine/Util/Defines.h"
#include "DUOLGameEngine/Util/SingletonBase.h"
#include "DUOLMath/DUOLMath.h"


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

		void Update(float deltaTime);
		 

	public:
		void LoadScene();

		bool GetIsCanvas() { return _isCanvas; };

		void CreateCanvas(GameObject* object);

		void SetIsCanvas() { _isCanvas = !_isCanvas; }

		GameObject* GetCanvas() { return _canvasList.back(); }

		void CreateImage(Image* image);

		void RemoveCanvas(GameObject* canvas);

		void RemoveImage(Image* image);

		void OnResize(int width, int height);

		std::vector<std::string> _fileNames;

		DUOLGameEngine::Image* GetImage(const std::string& path);

		std::vector<DUOLCommon::tstring>& GetSpriteFileList() { return _imageFileNames; }

		void LoadSpriteFile();

		void CreateTextureFile();

	private:
		bool _isCanvas;

		std::vector<GameObject*> _canvasList;

		std::vector<Image*> _imageList;

		std::vector<DUOLCommon::tstring> _imageFileNames;

	};
}
