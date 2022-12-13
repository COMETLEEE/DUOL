#pragma once
#define Mutil_Render_Count 6


class ID3D11RenderTargetView;

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

		ID3D11RenderTargetView* _deferredRenderTargetView;

		ID3D11RenderTargetView* _renderTargetView;
		// ���� Ÿ�� �� , ���� ���, �׸��� �׸� ��, ȭ�鿡 ���̴� ���� �ƴ�, �� ���۷� ����Ұž�~.
		// ���� ����� �����ִ� ������ �������~
		// ����Ʈ�� ���� ������

		// Multi Render Target�� ���� ������!!
		// Texture ���� ������ ���� Ŭ����.
		static RenderTexture* _renderTexture[Mutil_Render_Count + 1]; // �ٸ� �н����� �ʿ� �� ���� ������ static���� ������..
		// Shader�� �Ѱ��� �� �迭�� �Ѱ�����Ѵ�.
		ID3D11RenderTargetView* _textureRenderTargetView[Mutil_Render_Count + 1];
		// ȭ�鿡 ����ϱ� ���� Ŭ����
		static ObjectIDTexture* _objectIDTxture;
	public:
		void BeginRender();

		void ExecuteRender();

		void OnResize();

		void SetRenderTargetView(int _Num);

		static RenderTexture** GetRenderTexture() { return _renderTexture; }

		void PopShaderResource();

		unsigned int PickObjectID(int x, int y);

	private:
		void ClearRenderTarget(); // ��� ���� Ÿ�� �����

		void CreateDeferredTexture();

		void SetBackBufferRenderTarget(); // �ؽ�ó�� �׸��� ���ۿ��� ����۷� ��ȯ.

		void RenderDebugWindow(); // ���۵� ������ ����� ȭ�� ���.

		void RenderDeferredWindow(); // ���۵� ������ �� ȭ���� ���

		void Release();
	};
}

