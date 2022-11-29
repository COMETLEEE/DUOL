/**

	@file      ResourceManager.h
	@brief     리소스 매니저... 솔루션 마다 개별 이름을 지어주고 싶었는데 생각나는 이름이 없다..!
	@details   ~
	@author    SinSeongHyeon
	@date      15.11.2022
	@copyright © SinSeongHyeon, 2022. All right reserved.

**/
#pragma once

class ResourceManager
{
public:
	ResourceManager() = default;

	~ResourceManager() = default;

public:
	static ResourceManager _instance;

	static ResourceManager& Get();

public:
	void Initialize();

	void Finalize();
};
