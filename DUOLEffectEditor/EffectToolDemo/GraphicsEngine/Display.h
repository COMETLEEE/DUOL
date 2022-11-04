#pragma once
class Display
{
public:
	Display();
	Display(const Display&);
	~Display();

	bool Initialize(int, int, int, int);
	void Shutdown();
	bool Render(int, int);

	int GetIndexCount();

	ID3D11InputLayout* m_InputLayout;


private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	bool UpdateBuffers(ID3D11DeviceContext*, int, int);
	void RenderBuffers(ID3D11DeviceContext*);

private:
	ID3D11Buffer* m_vertexBuffer, * m_indexBuffer;
	int m_vertexCount, m_indexCount;
	int m_screenWidth, m_screenHeight;
	int m_bitmapWidth, m_bitmapHeight;
	int m_previousPosX, m_previousPosY;
};

