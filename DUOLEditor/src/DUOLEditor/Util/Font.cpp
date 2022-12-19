#include "DUOLEditor/Util/Font.h"

namespace DUOLEditor
{
	Font::Font(const DUOLCommon::tstring& fontID, ImFont* imFont) :
		_fontID(fontID)
		, _imFont(imFont)
	{
		
	}

	ImFont* Font::GetImFont()
	{
		return _imFont;
	}

	DUOLCommon::tstring Font::GetFontID()
	{
		return _fontID;
	}
}
