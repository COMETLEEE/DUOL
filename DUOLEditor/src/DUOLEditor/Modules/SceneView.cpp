#include "DUOLEditor/Modules/SceneView.h"

#include "DUOLEditor/UI/Widgets/Display/Image.h"

namespace DUOLEditor
{
	void SceneView::OnSceneTextureChanged(void* textureID)
	{
		// 씬을 나타내는 텍스쳐의 쉐이더 리소스 뷰 주소가 바뀌면
		// 해당 함수를 호출하여 갈아껴줍니다.
		_image->_textureID = textureID;
	}
}
