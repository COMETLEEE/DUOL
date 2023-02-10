#pragma once
#include "ResourceBase.h"

class ID3D11ShaderResourceView;

namespace MuscleGrapics
{
	class ShaderResourceView : public ResourceBase
	{
	public:
		ShaderResourceView(ID3D11ShaderResourceView* srv);

		virtual ~ShaderResourceView() override;

	private:
		ID3D11ShaderResourceView* _srv;

	public:
		ID3D11ShaderResourceView* Get();
	};
}
