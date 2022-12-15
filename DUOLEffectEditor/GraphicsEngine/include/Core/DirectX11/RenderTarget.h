#pragma once
#define Mutil_Render_Count 6


class ID3D11RenderTargetView;

class ID3D11DepthStencilView;

namespace MuscleGrapics
{
	enum class MutilRenderTexture
	{
		Depth,
		Normal,
		Position,
		Albedo,
		MatDiffuse,
		MatSpecular,
		ObjectID
	};
	class RenderTexture;

	class DeferredRenderPass;

	class TextureRenderPass;

	class ObjectIDTexture;

	class RenderTarget
	{
	public:
		RenderTarget();

		~RenderTarget();

	private:
		DeferredRenderPass* _deferredRenderPass;

		TextureRenderPass* _textureRenderPass;

		RenderTexture* _deferredTexture;

		ID3D11RenderTargetView* _deferredRenderTargetView; // ���۵� ������ ���� ��

		ID3D11RenderTargetView* _renderTargetView;
		// ���� Ÿ�� �� , ���� ���, �׸��� �׸� ��, ȭ�鿡 ���̴� ���� �ƴ�, �� ���۷� ����Ұž�~.
		// ���� ����� �����ִ� ������ �������~
		// ����Ʈ�� ���� ������

		// Multi Render Target�� ���� ������!!
		// Texture ���� ������ ���� Ŭ����.
		static RenderTexture* _renderTexture[Mutil_Render_Count + 1]; // �ٸ� �н����� �ʿ� �� ���� ������ static���� ������..
		// ȭ�鿡 ����ϱ� ���� Ŭ����
		static ObjectIDTexture* _objectIDTxture;
	public:
		void BeginRender();

		void ExecuteRender();

		void OnResize();

		// ���ε��� ���� Ÿ���� ������ �並 �ѱ��.
		void SetRenderTargetView(ID3D11DepthStencilView* _depthStencilView, int _num, ...);

		static RenderTexture** GetRenderTexture() { return _renderTexture; }

		ID3D11RenderTargetView* GetRenderTargetView(); // �� ����.

		void PopShaderResource();

		unsigned int PickObjectID(int x, int y);

	private:
		void ClearRenderTarget(); // ��� ���� Ÿ�� �����

		void CreateDeferredTexture();

		void RenderDebugWindow(); // ���۵� ������ ����� ȭ�� ���.

		void RenderDeferredWindow(); // ���۵� ������ �� ȭ���� ���

		void Release();
	};
}

