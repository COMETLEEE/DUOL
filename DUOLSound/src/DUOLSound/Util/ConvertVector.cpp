#include "DUOLSound/Util/ConverVector.h"

namespace DUOLSound
{
	// FMOD Right hand => DirectX Left hand

	FMOD_VECTOR ConverVector3(const DUOLMath::Vector3& vector)
	{
		return FMOD_VECTOR(vector.x, -vector.z, vector.y);
	}

	DUOLMath::Vector3 ConvertFMODVector(const FMOD_VECTOR& vector)
	{
		return DUOLMath::Vector3(vector.x, vector.z, -vector.y);
	}
}