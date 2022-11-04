#pragma once
class RasterizerState
{
public:
	RasterizerState();
	~RasterizerState();
public:
	static ID3D11RasterizerState* m_WireframeRS;
	static ID3D11RasterizerState* m_SolidRS;

};

