#include "DUOLSound/Util/ConverVector.h"

namespace DUOLSound
{
	FMOD_VECTOR ConverVector3(const DUOLMath::Vector3& vector)
	{
		return FMOD_VECTOR(vector.x, vector.y, vector.z);
	}

	DUOLMath::Vector3 ConvertFMODVector(const FMOD_VECTOR& vector)
	{
		return DUOLMath::Vector3(vector.x, vector.y, vector.z);
	}
}