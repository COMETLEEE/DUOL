#pragma once
#include "DUOLGraphicsLibrary/Export.h"
#include "DUOLGraphicsLibrary/Renderer/Texture.h"
#include "DUOLMath/DUOLMath.h"

#include <string>

//전방선언
struct IDWriteFactory;
struct IDWriteFontCollection1;

namespace  DUOLGraphicsLibrary
{
	class IFontEngine;

	class IFont;

	struct DUOLGRAPHICSLIBRARY_EXPORT Rect
	{
		float    left = 0.f;
		float    top = 0.f;
		float    right = 0.f;
		float    bottom = 0.f;
	};

	enum class WeightOption
	{
		BOLD = 1 << 0,
		NORMAL = 1 << 1,
	};

	enum class StyleOption
	{
		NORMAL = 1 << 0
		, ITALIC = 1 << 1
		, OBLIQUE = 1 << 2
	};

	struct DUOLGRAPHICSLIBRARY_EXPORT IResource
	{
		enum class ResourceType
		{
			Sprite = 1 
			, Text = 2
		};

		IResource(ResourceType resourceType);

		virtual ~IResource() = default;

	public:
		const ResourceType _resourceType;
	};

	struct DUOLGRAPHICSLIBRARY_EXPORT TextBox : public IResource
	{
		TextBox():
			IResource(ResourceType::Text)
			,_text()
			,_fontType(nullptr)
			,_rect()
			,_color(255.f, 255.f, 255.f, 255.f)
			, _angle(0)
			,_fontSize(36)
			,_weightOption(WeightOption::NORMAL)
			,_styleOption(StyleOption::NORMAL)
		{
			
		}

		~TextBox() = default;

	public:
		std::wstring _text;

		IFont* _fontType;

		Rect _rect;

		DUOLMath::Vector2 _pivot;

		DUOLMath::Vector4 _color;

		WeightOption _weightOption;

		StyleOption _styleOption;

		float _angle;

		DUOLMath::Vector2 _rotationXY;

		DUOLMath::Vector2 _scale;

		DUOLMath::Vector2 _translation;

		UINT32 _fontSize;
	};

	struct DUOLGRAPHICSLIBRARY_EXPORT Sprite : public IResource 
	{
	public:
		Sprite():
			IResource(ResourceType::Sprite)
			,_pivot()
			,_rect()
			, _scale(1.0f,1.0f)
			, _color(1.0f,1.0f, 1.f, 1.f)
			,_angle(0)
			,_texture(nullptr)
		{
		}

		~Sprite() = default;

	//public:
	//	virtual void SetTexture(Texture* texture) abstract;

	//	virtual Texture* GetTexture();

	//	virtual void SetColor(DUOLMath::Vector4& color) abstract;

	//	virtual const DUOLMath::Vector4& GetColor();

	public:
		float _angle;

		DUOLMath::Vector2 _rotationXY;

		DUOLMath::Vector2 _scale;

		DUOLMath::Vector2 _translation;

		DUOLMath::Vector2 _pivot;

		Rect _rect;

		DUOLMath::Vector4 _color;

		Texture* _texture;
	};

	/**
		@class   IFont
		@brief   Font For D3D11. 폰트를 가지고있는 DirectWrite의 wrapper class.
		@details ~
		@author  KyungMin Oh

	**/
	class DUOLGRAPHICSLIBRARY_EXPORT IFont
	{
	public:
		IFont() = default;
		virtual ~IFont() = default;

	};

	/**
		@class   IBrush
		@brief   Brush For D3D11. 폰트를 그릴때 사용할 색상, 변환정보를 갖습니다.
		@details ~
		@author  KyungMin Oh

	**/
	class DUOLGRAPHICSLIBRARY_EXPORT IBrush
	{
	public:
		IBrush() = default;

		virtual ~IBrush() = default;

	};

	/**
	@class   ICanvas
	@brief   Canvas For D3D11. 텍스트를 그릴 렌더타겟(텍스쳐)의 정보를 정합니다. 백버퍼에 그리거나, 3D WorldSpace에서 그릴 수 있도록.
	@details ~
	@author  KyungMin Oh
	**/
	class DUOLGRAPHICSLIBRARY_EXPORT ICanvas
	{
	public:
		ICanvas() = default;

		virtual ~ICanvas() = default;

	public:
		virtual Texture* GetTexture() abstract;

		virtual void DrawTexts(TextBox* const textBox, UINT32 orderInLayer) abstract;

		virtual void DrawSprite(Sprite* const sprite, UINT32 orderInLayer) abstract;
	};

	enum class CanvasRenderMode
	{
		BackBuffer
		, Texture
	};

	/**
		@class   IFontEngine
		@brief   Font Engine For D3D11. 폰트 출력을 담당하는 DirectWrite의 wrapper class.
		@details ~
		@author  KyungMin Oh

	**/
	class DUOLGRAPHICSLIBRARY_EXPORT IFontEngine
	{
	public:
		IFontEngine() = default;

		virtual ~IFontEngine() = default;

	public:
		virtual IFont* CreateFontFromTTF(const std::wstring& fontpath) abstract;

		virtual IFont* GetFont(const std::wstring& fontpath) abstract;

		virtual void DrawCanvas(ICanvas* canvas) abstract;

		virtual void Execute() abstract;

		virtual void RegistSprite(Texture* texture) abstract;

		virtual void DeleteSprite(Texture* texture) abstract;

		/**
			@brief  canvas rendermode에 따라 백버퍼를 캔버스로 사용하거나 d3d의 resource를 통해 캔버스를 만듭니다.
			@param  canvasName -
			@param  rendertype -
			@retval            -
		**/
		virtual ICanvas* CreateCanvas(const std::wstring& canvasName, CanvasRenderMode rendertype, Texture* const texture) abstract;

		virtual bool DeleteCanvas(const std::wstring& canvasName) abstract;

		virtual void RebuildCanvases() abstract;
	};


}
