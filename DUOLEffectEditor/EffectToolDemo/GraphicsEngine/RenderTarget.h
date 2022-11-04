#pragma once
#define Mutil_Render_Count 8
class RenderTarget
{
public:
	RenderTarget();
	~RenderTarget();
private:
	Display* m_DeferredWindow;
	RenderTexture* m_DeferredTexture;
	ID3D11RenderTargetView* m_DeferredRenderTargetView;

	ID3D11RenderTargetView* m_RenderTargetView;
	// ���� Ÿ�� �� , ���� ���, �׸��� �׸� ��, ȭ�鿡 ���̴� ���� �ƴ�, �� ���۷� ����Ұž�~.
	// ���� ����� �����ִ� ������ �������~
	// ����Ʈ�� ���� ������


	// Multi Render Target�� ���� ������!!
	// Texture ���� ������ ���� Ŭ����.
	RenderTexture* m_RederTexture[Mutil_Render_Count];
	// Shader�� �Ѱ��� �� �迭�� �Ѱ�����Ѵ�.
	ID3D11RenderTargetView* m_TextureRenderTargetView[Mutil_Render_Count];
	// ȭ�鿡 ����ϱ� ���� Ŭ����
	Display* m_DebugWindows[Mutil_Render_Count];
public:
	void BeginRender();
	void EndRender();
	void OnResize();
	void SetRenderTargetView(int _Num);
private:
	void ClearRenderTarget(); // ��� ���� Ÿ�� �����
	void CreateDeferredTexture();
	void SetBackBufferRenderTarget(); // �ؽ�ó�� �׸��� ���ۿ��� ����۷� ��ȯ.
	void RenderDebugWindow(); // ���۵� ������ ����� ȭ�� ���.
	void RenderDeferredWindow(); // ���۵� ������ �� ȭ���� ���
	void Release();
};
