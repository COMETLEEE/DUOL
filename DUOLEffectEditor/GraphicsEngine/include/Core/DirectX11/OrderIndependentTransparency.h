/**

	@file      OrderIndependentTransparency.h
	@brief     OIT 구현관련 클래스
	@details   ~ 처음에는 OIT 관리를 위한 클래스로 설계했지만, 시간이 지날수록 파티클 전체를 관리하는 클래스가 되고있다..!
	@author    SinSeongHyeon
	@date      10.01.2023
	@copyright © SinSeongHyeon, 2023. All right reserved.

**/

#pragma once
#include <memory>
#include <queue>
#include <vector>

class ID3D11DepthStencilState;
class ID3D11ShaderResourceView;
class ID3D11RenderTargetView;
class ID3D11DepthStencilView;

constexpr int g_layerCount = 10;

namespace MuscleGrapics
{
	struct RenderingData_Particle;
	class DXEngine;
	class RenderTexture;

	class OrderIndependentTransparency
	{
	public:
		OrderIndependentTransparency() = default;

		~OrderIndependentTransparency();
	private:
		struct PictureInfo
		{
			ID3D11DepthStencilView* _dsv;

			ID3D11ShaderResourceView* _depthSrv;

			ID3D11RenderTargetView* _rtv;

			ID3D11ShaderResourceView* _backSrv;
		};

	private:
		RenderTexture* _colorTexture[g_layerCount];

		std::vector<PictureInfo> _vdxPic;

		ID3D11ShaderResourceView* _nullSRV;

		int _drawCount;

		DXEngine* _dxEngine; // 캐싱/
	private:
		void Clear();

		void Draw(); // 레이어가 완성이 됐을 때..!

		void Render(std::queue<std::shared_ptr<RenderingData_Particle>>& renderQueueParticle); // 레이어를 그리는 함수. 레이어를 먼저 만들고 Draw함수를 호출 해야 한다.

		void PostProcessing();
	public:
		void Execute(std::queue<std::shared_ptr<RenderingData_Particle>>& renderQueueParticle);

		void SetRenderTargetAndDepth();

		int GetDrawCount();

		void Finalize();

		void OnResize();

		static OrderIndependentTransparency& Get();
	};
}

