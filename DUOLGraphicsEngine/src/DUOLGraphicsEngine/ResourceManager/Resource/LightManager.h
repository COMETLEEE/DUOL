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
     @brief   �׸��ڸ��� ������ �����Դϴ�. �ϳ��� Ŀ�ٶ� �ؽ��Ŀ� ����Ʈ �̵��� ���� �����츦 ��Ʈ���մϴ�.
			  
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