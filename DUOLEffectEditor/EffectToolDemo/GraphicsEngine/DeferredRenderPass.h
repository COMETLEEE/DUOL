#pragma once
/**

	@class   DeferredRenderPass
	@brief   ���۵� ������ ���� �н��� ���ҽ� �Ŵ������� �������� �ʴ´�. ��ȸ���̱� ����.
	@details ~

**/
class DeferredRenderPass : public PassBase<std::vector<pair<ID3D11ShaderResourceView*, int>>>
{
public:
	DeferredRenderPass();

	virtual ~DeferredRenderPass() override;

protected:
	virtual void SetConstants(std::vector<pair<ID3D11ShaderResourceView*, int>>& renderingData) override;

public:
	virtual void Draw(std::vector<pair<ID3D11ShaderResourceView*, int>>& renderingData) override;
};

