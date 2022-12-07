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

	static DUOLCommon::tstring OpenParticleFile(std::shared_ptr<MuscleGrapics::RenderingData_Particle>& data);
	static DUOLCommon::tstring SaveParticleFile(std::shared_ptr<MuscleGrapics::RenderingData_Particle>& data);

private:
	static DUOLCommon::tstring OpenFile(const char* filter);
	static DUOLCommon::tstring SaveFile(const char* filter);
};

