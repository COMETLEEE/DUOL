#pragma once
#include "ResourceBase.h"

class ID3D11GeometryShader;

class ID3D11ClassInstance;

namespace MuscleGrapics
{
	class GeometryShader : public ResourceBase
	{
	public:
		GeometryShader(ID3D11GeometryShader* shader, ID3D11ClassInstance** dynamicLinkageArray, unsigned int instanceNum);

		virtual ~GeometryShader() override;

	private:
		ID3D11GeometryShader* _gs;

		ID3D11ClassInstance** _gsDynamicLinkageArray;

		unsigned int _instanceNum;
	public:
		ID3D11GeometryShader* Get();

		ID3D11ClassInstance** GetDynamicLinkageArray();

		unsigned int GetInstanceNum();
	};
}