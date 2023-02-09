#pragma once
#include "ResourceBase.h"

class ID3D11PixelShader;

class ID3D11ClassInstance;

namespace MuscleGrapics
{
	class PixelShader : public ResourceBase
	{
	public:
		PixelShader(ID3D11PixelShader* shader, ID3D11ClassInstance** dynamicLinkageArray, unsigned int instanceNum);

		virtual ~PixelShader() override;

	private:
		ID3D11PixelShader* _ps;

		ID3D11ClassInstance** _dynamicLinkageArray;

		unsigned int _instanceNum;

	public:
		ID3D11PixelShader* Get();

		ID3D11ClassInstance** GetDynamicLinkageArray();

		unsigned int GetInstanceNum();
	};
}