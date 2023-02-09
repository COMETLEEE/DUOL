#include "Core/Resource/Resource/ComputeShader.h"

#include "util/TypeDefine.h"

MuscleGrapics::ComputeShader::ComputeShader(ID3D11ComputeShader* shader, ID3D11ClassInstance** dynamicLinkageArray,
	unsigned instanceNum) :
	_cs(shader),
	_dynamicLinkageArray(dynamicLinkageArray),
	_instanceNum(instanceNum)
{
}

MuscleGrapics::ComputeShader::~ComputeShader()
{
	ReleaseCOM(_cs);

	delete[] _dynamicLinkageArray;
}

ID3D11ComputeShader* MuscleGrapics::ComputeShader::Get()
{
	return _cs;
}

ID3D11ClassInstance** MuscleGrapics::ComputeShader::GetDynamicLinkageArray()
{
	return _dynamicLinkageArray;
}

unsigned MuscleGrapics::ComputeShader::GetInstanceNum()
{
	return _instanceNum;
}
