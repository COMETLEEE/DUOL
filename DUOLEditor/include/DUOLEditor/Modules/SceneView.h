#pragma once
#include "DUOLEditor/Modules/ViewBase.h"

namespace DUOLEditor
{
	/**
	 * \brief 레벨을 구성할 때 참조할 수 있는 게임 공간 화면을 나타내는 UI Object.
	 */
	class SceneView : public DUOLEditor::ViewBase
	{
	public:
		SceneView(const DUOLCommon::tstring& title, bool isOpened, const PanelWindowSetting& windowSetting);

		virtual ~SceneView() override;

	public:
		// 현재 오퍼레이션에 따른 기즈모 렌더링
		// 그리드 렌더링 등은 .. 전부 게임 엔진에서 알아서 한다 .. 로 갑시다 일단.
		void OnSceneTextureChanged(void* textureID);
 	};
}
