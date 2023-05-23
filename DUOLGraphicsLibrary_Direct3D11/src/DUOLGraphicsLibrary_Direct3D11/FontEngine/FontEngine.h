#pragma once
#include <d2d1.h>
#include <d2d1_1.h>
#include <dxgi.h>
#include "DUOLGraphicsLibrary_Direct3D11/ComPtr.h"
#include "DUOLGraphicsLibrary/FontEngine/IFontEngine.h"

#include <memory>

#include "DUOLGraphicsLibrary/Util/ReservablePriorityQueue.h"
#include "DUOLGraphicsLibrary_Direct3D11/Renderer/Buffer/D3D11Buffer.h"

//전방선언
struct IDWriteFontCollection1;

namespace  DUOLGraphicsLibrary
{
	class Canvas;

	class FontEngine;
	/**

		@class   Font
		@brief   Font For D3D11. 폰트를 가지고있는 DirectWrite의 wrapper class.
		@details ~
		@author  KyungMin Oh

	**/
	
	class Font : public IFont
	{
	//각옵션당 MaxByte는 16정도로 하자.

	public:
		Font(IDWriteFactory* factory, const std::wstring& fontName, const std::wstring& fontLocale, ComPtr<IDWriteFontCollection1> customFontCollection);

		~Font() = default;

		IDWriteTextFormat* GetIDWriteTextFormat(UINT32 fontSize, WeightOption weightOption, StyleOption styleOption);

	private:
		struct Impl;

		std::unique_ptr<Impl> _pImpl;

		friend FontEngine;
	};

	//struct DWSprite : public Sprite
	//{
	//	DWSprite(FontEngine* fontEngine) :
	//		Sprite()
	//		,_parentFontManager(fontEngine)
	//	{
	//	}

	//	~DWSprite() = default;

	//public:
	//	virtual void SetTexture(Texture* texture) override;

	//	virtual void SetColor(DUOLMath::Vector4& color) override;

	//	bool HasEffectImage();
	//private:
	//	FontEngine* _parentFontManager;

	//	bool _hasColorTransform;

	//	friend FontEngine;
	//};

	/**
		@class   Brush
		@brief   Brush For D3D11. 폰트를 그릴때 사용할 색상, 변환정보를 갖습니다.
		@details ~
		@author  KyungMin Oh

	**/
	class Brush : public IBrush
	{
	public:
		Brush(ID2D1RenderTarget* renderTarget, const DUOLMath::Vector4& color);

		~Brush();

	public:
		void SetColor(const Canvas* canvas, const DUOLMath::Vector4& color);

		ID2D1SolidColorBrush* GetBrush();

	private:
		struct Impl;

		std::unique_ptr<Impl> _pImpl;

		friend FontEngine;
	};

	/**
	@class   Canvas
	@brief   Canvas For D3D11. 텍스트를 그릴 렌더타겟(텍스쳐)의 정보를 정합니다. 백버퍼에 그리거나, 3D WorldSpace에서 그릴 수 있도록.
	@details ~
	@author  KyungMin Oh
	**/
	class Canvas : public ICanvas
	{
		using TextLayer = std::pair<UINT32, IResource*>;

		//struct CompareLayer
		//{
		//	bool operator()(TextLayer& a, TextLayer& b)
		//	{
		//		if(a.first > b.first)
		//		{
		//			return true;
		//		}
		//		return false;
		//	}
		//};

		//using UIQueue = ReservablePriorityQueue<TextLayer>;
		using UIQueue = std::vector<TextLayer>;

	public:
		Canvas(ID2D1DeviceContext* context, Texture* texture, IDXGISurface* surface, CanvasRenderMode rendertype);

		~Canvas();

	public:
		virtual Texture* GetTexture() override;

		void DrawTexts(TextBox* const textBox, UINT32 orderInLayer) override;

		void DrawSprite(Sprite* const sprite, UINT32 orderInLayer) override;

		void Unload();
		
		void CreateRenderTarget(ID2D1DeviceContext* context, IDXGISurface* surface);

		void RebuildRenderTarget(ID2D1DeviceContext* context);

		UIQueue& GetUIQueue();

		ID2D1Bitmap1* GetRenderTarget();

	private:
		struct Impl;

		std::unique_ptr<Impl> _pImpl;

		friend FontEngine;
	};
	/**
		@class   FontEngine
		@brief   Font Engine For D3D11. 폰트 출력을 담당하는 DirectWrite의 wrapper class.
		@details ~
		@author  KyungMin Oh

	**/
	class FontEngine : public IFontEngine
	{
	public:
		FontEngine(IDXGIDevice* dxgiDevice, IDXGISwapChain* d3dSwapchain, HWND handle);

		~FontEngine();

	public:
		IFont* CreateFontFromTTF(const std::wstring& fontpath) override;

		IFont* GetFont(const std::wstring& fontpath) override;

		ICanvas* CreateCanvas(const std::wstring& canvasName, CanvasRenderMode rendertype, Texture* const texture = nullptr) override;
		
		void DrawCanvas(ICanvas* canvas) override;

		void Execute() override;

		bool DeleteCanvas(const std::wstring& canvasName) override;

		void RegistSprite(Texture* texture) override;

		void DeleteSprite(Texture* texture) override;

		void UnloadBackbuffer();

		void CreateBackbuffer();

		void RebuildCanvases() override;

	private:
		struct Impl;

		std::unique_ptr<Impl> _pImpl;
	};


}
