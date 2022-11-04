#pragma once
class DepthStencil
{
public:
	DepthStencil();
	~DepthStencil();
private:
	Depth* m_DepthStencil[2];
	ID3D11DepthStencilState* m_OffDepthStencilState; // µª½º Off¸¦ À§ÇÑ ½ºÅÙ½Ç ºä
public:
	void OnDepthStencil(int _Num); // 0 ÀÏ¹Ý 1 ½¦µµ¿ì µª½º
	ID3D11DepthStencilView* GetDpethStencilView(int _Num); // 0 ÀÏ¹Ý 1 ½¦µµ¿ì µª½º
	void OffDepthStencil();
	void Clear();
	void OnResize();
	void Finalize();
};

