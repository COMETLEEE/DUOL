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
    /**

        @class   DeviceHelper
        @brief   Hardware Device 정보를 얻기 위한 Helper 클래스
        @details -

    **/
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
        /**
            @brief   DeviceHelper 클래스 default 생성자
            @details -
        **/
        DeviceHelper() = default;

    public:
        /**
            @brief   DeviceHelper 클래스 default 소멸자
            @details -
        **/
        ~DeviceHelper() = default;

    private:
        static CPUInformation _cpuInfo;

    public:
        /**
            @brief   CPU Information Getter
            @details -
            @retval  CPU Information 구조체
        **/
        static const CPUInformation& GetCPUInfo() { return _cpuInfo; }
    };
}