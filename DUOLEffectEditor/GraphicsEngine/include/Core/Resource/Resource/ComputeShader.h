#pragma once
#include "ResourceBase.h"

class ID3D11ComputeShader;

class ID3D11ClassInstance;

namespace MuscleGrapics
{
	class ComputeShader : public ResourceBase
	{
	public:
		ComputeShader(ID3D11ComputeShader* shader, ID3D11ClassInstance** dynamicLinkageArray, unsigned int instanceNum);

		virtual ~ComputeShader() override;

	private:
		ID3D11ComputeShader* _cs;

		ID3D11ClassInstance** _dynamicLinkageArray;

		unsigned int _instanceNum;

	public:
		ID3D11ComputeShader* Get();

		ID3D11ClassInstance** GetDynamicLinkageArray();

		unsigned int GetInstanceNum();
	};
}