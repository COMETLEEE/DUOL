#include "DUOLCommon/DeviceHelper.h"

#include <windows.h>
#include <tchar.h>

namespace DUOLCommon
{
	CPUInformation DeviceHelper::_cpuInfo = {};

	DWORD ConutSetBits(ULONG_PTR bitMask)
	{
		DWORD lShift = sizeof(ULONG_PTR) * 8 - 1;
		DWORD bitSetCount = 0;
		ULONG_PTR bitTest = (ULONG_PTR)1 << lShift;

		for (DWORD i = 0; i <= lShift; i++)
		{
			bitSetCount += ((bitMask & bitTest) ? 1 : 0);
			bitTest /= 2;
		}

		return bitSetCount;
	}

	class DeviceHelper::DeviceHelperImpl
	{
		using GLPIFN = BOOL(WINAPI*)(PSYSTEM_LOGICAL_PROCESSOR_INFORMATION, PDWORD);

	public:
		DeviceHelperImpl();

	private:
		void Decode(PSYSTEM_LOGICAL_PROCESSOR_INFORMATION ptr, DWORD returnLength);
	} _deviceHelper;

	DeviceHelper::DeviceHelperImpl::DeviceHelperImpl()
	{
		GLPIFN glpi = (GLPIFN)GetProcAddress(GetModuleHandle(_T("kernel32")), "GetLogicalProcessorInformation");

		if (glpi == nullptr)
		{
			// GetLogicalProcessorInformation is not supported.
			return;
		}

		PSYSTEM_LOGICAL_PROCESSOR_INFORMATION buffer = nullptr;
		DWORD returnLength = 0;
		DWORD rc;

		while ((rc = glpi(buffer, &returnLength)) == FALSE)
		{
			if (GetLastError() == ERROR_INSUFFICIENT_BUFFER)
			{
				if (buffer != nullptr)
					free(buffer);

				buffer = (PSYSTEM_LOGICAL_PROCESSOR_INFORMATION)malloc(returnLength);

				if (buffer == nullptr)
				{
					// Error: Allocation failure.
					return;
				}
			}
			else
			{
				// Error. Check with call GetLastError()
				return;
			}
		}

		Decode(buffer, returnLength);

		free(buffer);
	}

	void DeviceHelper::DeviceHelperImpl::Decode(PSYSTEM_LOGICAL_PROCESSOR_INFORMATION ptr, DWORD returnLength)
	{
		DWORD byteOffset = 0;

		while (byteOffset + sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION) <= returnLength)
		{
			switch (ptr->Relationship)
			{

			case RelationProcessorCore:
			{
				DeviceHelper::_cpuInfo._core++;
				DeviceHelper::_cpuInfo._thread += ConutSetBits(ptr->ProcessorMask);

				break;
			}

			case RelationNumaNode:
			{
				DeviceHelper::_cpuInfo._numaNodes++;

				break;
			}

			case RelationCache:
			{
				PCACHE_DESCRIPTOR cache = &ptr->Cache;

				if (cache->Level == 1)
					DeviceHelper::_cpuInfo._L1Cache++;
				else if (cache->Level == 2)
					DeviceHelper::_cpuInfo._L2Cache++;
				else if (cache->Level == 3)
					DeviceHelper::_cpuInfo._L3Cache++;

				break;
			}

			case RelationProcessorPackage:
			{
				DeviceHelper::_cpuInfo._package++;

				break;
			}

			case RelationGroup:
			{

				break;
			}

			default:
			{
				// Error: Unsupported LOGICAL_PROCESSOR_RELATIONSHIP value.
				break;
			}

			}

			byteOffset += sizeof(SYSTEM_LOGICAL_PROCESSOR_INFORMATION);
			ptr++;
		}
	}
}