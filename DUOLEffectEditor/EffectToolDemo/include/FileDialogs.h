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

	static void SaveParticleFile(std::shared_ptr<MuscleGrapics::RenderingData_Particle>& data);

	static void SaveAsParticleFile(std::shared_ptr<MuscleGrapics::RenderingData_Particle>& data);
private:
	static DUOLCommon::tstring OpenFile(const char* filter);
	static DUOLCommon::tstring SaveFile(const char* filter);
};

