#include "DUOLEditor/Modules/SceneView.h"

#include "DUOLEditor/UI/Widgets/Display/Image.h"

namespace DUOLEditor
{
	void SceneView::OnSceneTextureChanged(void* textureID)
	{
		// ���� ��Ÿ���� �ؽ����� ���̴� ���ҽ� �� �ּҰ� �ٲ��
		// �ش� �Լ��� ȣ���Ͽ� ���Ʋ��ݴϴ�.
		_image->_textureID = textureID;
	}
}
