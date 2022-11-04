/**

    @file    DeviceHelper.h
    @brief   Hardware Device ������ ��� ���� Helper
    @details -
    @author  JKim
    @date    26.10.2022

**/
#pragma once

namespace DUOLCommon
{
    /**

        @class   DeviceHelper
        @brief   Hardware Device ����� ��� ��� Helper Ŭ����
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
            @brief   DeviceHelper Ŭ���� default ����
            @details -
        **/
        DeviceHelper() = default;

    public:
        /**
            @brief   DeviceHelper Ŭ���� default �Ҹ���
            @details -
        **/
        ~DeviceHelper() = default;

    private:
        static CPUInformation _cpuInfo;

    public:
        /**
            @brief   CPU Information Getter
            @details -
            @retval  CPU Information ���ü
        **/
        static const CPUInformation& GetCPUInfo() { return _cpuInfo; }
    };
}