/**

    @file      ErrorCheck.h
    @brief     Checking error util.
    @details   ~
    @author    COMETLEE
    @date      4.04.2023
    @copyright © COMETLEE, 2023. All right reserved.

**/
#pragma once
#include <FMOD/fmod_common.h>

namespace DUOLSound
{
    void ErrorCheck(FMOD_RESULT result);
}

#if defined(_DEBUG)
	#define ERROR_CHECK(_result) DUOLSound::ErrorCheck(_result);
#else
	#define ERROR_CHECK(_result)
#endif