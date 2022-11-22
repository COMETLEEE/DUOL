/**

    @file    PhysicsDefines.h
    @brief   Define°ú °ü·Ã Header
    @details -
    @author  JKim
    @date    22.11.2022

**/
#pragma once

#include <string>

#define ERROR_THROW(errStr)				\
{										\
	std::string errTemp = errStr;		\
	errTemp += "\n[ Line : ";			\
	errTemp += std::to_string(__LINE__);\
	errTemp += " ] ";					\
	errTemp += __FILE__;				\
	throw errTemp;						\
}