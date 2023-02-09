#include "Core/Resource/Resource/VertexShader.h"

#include "util/TypeDefine.h"

MuscleGrapics::VertexShader::VertexShader(ID3D11VertexShader* shader, ID3D11InputLayout* inputLayout,
	ID3D11ClassInstance** dynamicLinkageArray, unsigned instanceNum) :
	_vs(shader),
	_il(inputLayout),
	_gsDynamicLinkageArray(dynamicLinkageArray),
	_instanceNum(instanceNum)
{
}

MuscleGrapics::VertexShader::~VertexShader()
{
	ReleaseCOM(_vs);
	ReleaseCOM(_il);
	delete[] _gsDynamicLinkageArray;
}

ID3D11VertexShader* MuscleGrapics::VertexShader::Get()
{
	return _vs;
}

ID3D11InputLayout* MuscleGrapics::VertexShader::GetInputLayOut()
{
	return _il;
}

ID3D11ClassInstance** MuscleGrapics::VertexShader::GetDynamicLinkageArray()
{
	return _gsDynamicLinkageArray;
}

unsigned MuscleGrapics::VertexShader::GetInstanceNum()
{
	return _instanceNum;
}
