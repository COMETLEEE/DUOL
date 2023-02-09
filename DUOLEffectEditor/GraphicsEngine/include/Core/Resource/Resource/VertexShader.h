#pragma once
#include "ResourceBase.h"

class ID3D11VertexShader;

class ID3D11InputLayout;

class ID3D11ClassInstance;

namespace MuscleGrapics
{
	class VertexShader : public ResourceBase
	{
	public:
		VertexShader(ID3D11VertexShader* shader, ID3D11InputLayout* inputLayout, ID3D11ClassInstance** dynamicLinkageArray, unsigned int instanceNum);

		virtual ~VertexShader() override;

	private:
		ID3D11VertexShader* _vs;

		ID3D11InputLayout* _il;

		ID3D11ClassInstance** _gsDynamicLinkageArray;

		unsigned int _instanceNum;
	public:
		ID3D11VertexShader* Get();

		ID3D11InputLayout* GetInputLayOut();

		ID3D11ClassInstance** GetDynamicLinkageArray();

		unsigned int GetInstanceNum();
	};
}