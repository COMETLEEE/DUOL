#pragma once
#include "ResourceBase.h"

class ID3D11ClassInstance;

namespace MuscleGrapics
{
	class ShaderClassInstance : public ResourceBase
	{
	public:
		ShaderClassInstance(ID3D11ClassInstance* instance, unsigned int index);

		virtual ~ShaderClassInstance() override;

	private:
		ID3D11ClassInstance* _instance;

		unsigned int _index;

	public:
		ID3D11ClassInstance* Get();

		unsigned int GetIndex();
	};
}

