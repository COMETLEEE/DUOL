#pragma once
/**

	@class   DeferredRenderPass
	@brief   디퍼드 렌더링 관련 패스는 리소스 매니저에서 관리하지 않는다. 일회용이기 때문.
	@details ~

**/
class DeferredRenderPass : public PassBase<std::vector<pair<ID3D11ShaderResourceView*, int>>>
{
public:
	DeferredRenderPass();

	virtual ~DeferredRenderPass() override;

private:
	ID3D11SamplerState* _wrapSamplerState;

protected:
	virtual void SetConstants(std::vector<pair<ID3D11ShaderResourceView*, int>>& renderingData) override;

public:
	virtual void Draw(std::vector<pair<ID3D11ShaderResourceView*, int>>& renderingData) override;
};

