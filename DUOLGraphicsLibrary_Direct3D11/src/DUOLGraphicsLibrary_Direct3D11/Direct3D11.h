#pragma once

#define D3D11_ENABLE_FEATURELEVEL 0

#if D3D11_ENABLE_FEATURELEVEL >= 3
#   include <d3d11_3.h>
#elif D3D11_ENABLE_FEATURELEVEL >= 2
#   include <d3d11_2.h>
#elif D3D11_ENABLE_FEATURELEVEL >= 1
#   include <d3d11_1.h>
#else
#   include <d3d11.h>
#endif

