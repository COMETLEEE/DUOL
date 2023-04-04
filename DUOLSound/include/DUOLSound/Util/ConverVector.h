#pragma once
#include <FMOD/fmod_common.h>

#include "DUOLMath/DUOLMath.h"

namespace DUOLSound
{
	FMOD_VECTOR ConverVector3(const DUOLMath::Vector3& vector);

	DUOLMath::Vector3 ConvertFMODVector(const FMOD_VECTOR& vector);
}