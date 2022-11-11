#pragma once
/**
	@class   TextureRenderPass
	@brief   디퍼드 렌더링 관련 패스는 리소스 매니저에서 관리하지 않는다. 일회용이기 때문.
	@details ~

**/
class TextureRenderPass : public PassBase<pair<ID3D11ShaderResourceView*, int>>
{
public:
	TextureRenderPass();

	virtual ~TextureRenderPass();

private:
	//float left, right, top, bottom;
	float _left = -1.0f;
	float _right = 1.0f;
	float _top = 1.0f;
	float _bottom = -1.0f;

protected:
	virtual void SetConstants(pair<ID3D11ShaderResourceView*, int>& renderingData) override;

public:
	virtual void Draw(pair<ID3D11ShaderResourceView*, int>& renderingData) override;

	void SetDrawRectangle(float left, float right, float top, float bottom);
};

