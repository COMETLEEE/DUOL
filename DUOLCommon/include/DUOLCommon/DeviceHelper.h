/**

    @file    DeviceHelper.h
    @brief   Hardware Device 정보를 얻기 위한 Helper
    @details -
    @author  JKim
    @date    26.10.2022

**/
#pragma once

namespace DUOLCommon
{

    class DeviceHelper
    {
        class DeviceHelperImpl;

        friend DeviceHelperImpl;

        struct CPUInformation
        {
            unsigned int _core;

            unsigned int _thread;

            unsigned int _L1Cache;

            unsigned int _L2Cache;

            unsigned int _L3Cache;

            unsigned int _numaNodes;

            // Physical Package..??
            unsigned int _package;
        };

    private:
        DeviceHelper() = default;

    public:
        ~DeviceHelper() = default;

    private:
        static CPUInformation _cpuInfo;

    public:
        static const CPUInformation& GetCPUInfo() { return _cpuInfo; }
    };
}