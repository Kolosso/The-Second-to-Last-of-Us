#include "LogManager.h"

#include <Windows.h>

LogManager* LogManager::sp_Instance = 0;

LogManager&
LogManager::GetInstance()
{
	if (sp_Instance == 0)
	{
		sp_Instance = new LogManager();
	}

	return (*sp_Instance);
}

LogManager::LogManager()
{
}


LogManager::~LogManager()
{
	delete sp_Instance;
	sp_Instance = 0;
}

void
LogManager::Log(const char* pcMessage)
{
	OutputDebugStringA(pcMessage);
	OutputDebugStringA("\n");
}
