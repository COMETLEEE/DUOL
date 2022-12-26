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

		DXEngine* _dxEngine; // ĳ��/
	public:
		void OnResize();

		void Finalize();

		void Clear();

		void Draw(); // ���̾ �ϼ��� ���� ��..!

		void Render(std::queue<std::shared_ptr<RenderingData_Particle>>& renderQueueParticle); // ���̾ �׸��� �Լ�. ���̾ ���� ����� Draw�Լ��� ȣ�� �ؾ� �Ѵ�.

		void SetRenderTargetAndDepth();

		int GetDrawCount();

		static OrderIndependentTransparency& Get();
	};
}

