#include "Core/Resource/Resource/PixelShader.h"

#include "util/TypeDefine.h"

MuscleGrapics::PixelShader::PixelShader(ID3D11PixelShader* shader, ID3D11ClassInstance** dynamicLinkageArray,
	unsigned instanceNum) :
	_ps(shader),
	_dynamicLinkageArray(dynamicLinkageArray),
	_instanceNum(instanceNum)
{
}

MuscleGrapics::PixelShader::~PixelShader()
{
	ReleaseCOM(_ps);

	delete[] _dynamicLinkageArray;
}

ID3D11PixelShader* MuscleGrapics::PixelShader::Get()
{
	return _ps;
}

ID3D11ClassInstance** MuscleGrapics::PixelShader::GetDynamicLinkageArray()
{
	return _dynamicLinkageArray;
}

unsigned MuscleGrapics::PixelShader::GetInstanceNum()
{
	return _instanceNum;
}
