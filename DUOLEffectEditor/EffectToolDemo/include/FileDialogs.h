#pragma once

#include <memory>
#include "StringHelper.h"

namespace MuscleGrapics
{
	struct RenderingData_Particle;
}

class FileDialogs
{
public:
	static DUOLCommon::tstring OpenTextureFile();

	static MuscleGrapics::RenderingData_Particle LoadParticleFile();

	static void SaveParticleFile(MuscleGrapics::RenderingData_Particle& data);

	static void SaveAsParticleFile(MuscleGrapics::RenderingData_Particle& data);
private:
	static DUOLCommon::tstring OpenFile(const char* filter);
	static DUOLCommon::tstring SaveFile(const char* filter);
};

