/**

    @file      EditorHub.h
    @brief     에디터 프로그램이 시작할 때 프로젝트를 고를 수 있도록 하는 패널
    @details   ~
    @author    COMETLEE
    @date      4.11.2022
    @copyright © COMETLEE, 2022. All right reserved.

**/
#pragma once
#include <tuple>
#include "DUOLCommon/StringHelper.h"

namespace DUOLEditor
{
	class EditorHub
	{
	public:
        EditorHub();

        std::tuple<bool, DUOLCommon::tstring, DUOLCommon::tstring> Run();

        // void RegisterProject(const DUOLCommon::tstring projectPath);

	private:
        bool _readyToStart;

        // 어차피 시작하는 것에 필요하다면 미리 세트화해서 정리 ..?
        DUOLCommon::tstring _projectPath;

        DUOLCommon::tstring _projectName;
	};
}