#include "Core/Resource/Resource/GeometryShader.h"

#include "util/TypeDefine.h"

MuscleGrapics::GeometryShader::GeometryShader(ID3D11GeometryShader* shader, ID3D11ClassInstance** dynamicLinkageArray,
	unsigned instanceNum) : 
	_gs(shader),
	_gsDynamicLinkageArray(dynamicLinkageArray),
	_instanceNum(instanceNum)
{
}

MuscleGrapics::GeometryShader::~GeometryShader()
{
	ReleaseCOM(_gs);

	delete[] _gsDynamicLinkageArray;
}

ID3D11GeometryShader* MuscleGrapics::GeometryShader::Get()
{
	return _gs;
}

ID3D11ClassInstance** MuscleGrapics::GeometryShader::GetDynamicLinkageArray()
{
	return _gsDynamicLinkageArray;
}

unsigned MuscleGrapics::GeometryShader::GetInstanceNum()
{
	return _instanceNum;
}
