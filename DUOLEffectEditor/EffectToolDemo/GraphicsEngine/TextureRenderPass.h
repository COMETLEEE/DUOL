#pragma once
/**
	@class   TextureRenderPass
	@brief   ���۵� ������ ���� �н��� ���ҽ� �Ŵ������� �������� �ʴ´�. ��ȸ���̱� ����.
	@details ~

**/
class TextureRenderPass : public PassBase<pair<ID3D11ShaderResourceView*, int>>
{
public:
	TextureRenderPass();

	virtual ~TextureRenderPass();

private:
	ID3D11SamplerState* _wrapSamplerState;

protected:
	virtual void SetConstants(pair<ID3D11ShaderResourceView*, int>& renderingData) override;

public:
	virtual void Draw(pair<ID3D11ShaderResourceView*, int>& renderingData) override;


};

