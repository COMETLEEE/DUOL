#pragma once




namespace DUOLGraphicsLibrary
{
	class PipelineState;
	class RenderTarget;
	class Texture;
}

namespace DUOLGraphicsEngine
{
 /**

     @class   LightManager
     @brief   그림자맵을 관리할 예정입니다. 하나의 커다란 텍스쳐에 뷰포트 이동을 통해 섀도우를 컨트롤합니다.
			  
     @details ~
     @author  KyungMin Oh

 **/
	class LightManager
	{
	public:
		LightManager();

		~LightManager();

	private:
		DUOLGraphicsLibrary::Texture* _shadowMapAtlas;

	};
}