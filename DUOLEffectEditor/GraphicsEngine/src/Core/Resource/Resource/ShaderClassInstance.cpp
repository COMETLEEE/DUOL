#include "Core/Resource/Resource/ShaderClassInstance.h"
#include "util/TypeDefine.h"

MuscleGrapics::ShaderClassInstance::ShaderClassInstance(ID3D11ClassInstance* instance, unsigned int index)
	:_instance(instance), _index(index)
{
}

MuscleGrapics::ShaderClassInstance::~ShaderClassInstance()
{
	ReleaseCOM(_instance);
}

ID3D11ClassInstance* MuscleGrapics::ShaderClassInstance::Get()
{
	return _instance;
}

unsigned int MuscleGrapics::ShaderClassInstance::GetIndex()
{
	return _index;
}
