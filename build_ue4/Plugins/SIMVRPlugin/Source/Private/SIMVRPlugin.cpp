// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.


#include "SIMVRPluginPrivatePCH.h"
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <stdlib.h>	// for mbstowcs_s(), wcstombs_s()

DEFINE_LOG_CATEGORY(SIMVRLog);

//LoadLibrary
extern "C" {
	typedef int(*simvrOpen_ptr)(const char* serialNo);
	typedef int(*simvrClose_ptr)();
	typedef void(*simvrWrite_ptr)(simvrPacket* packet);

	typedef void(*simvrSetOriginMode_ptr)(bool value);
	typedef bool(*simvrGetOriginMode_ptr)();

	typedef void(*simvrSetAxisProcessingMode_ptr)(bool value);
	typedef bool(*simvrGetAxisProcessingMode_ptr)();

	typedef const char*(*simvrGetAppCode_ptr)();

	typedef int(*simvrGetState_ptr)();
	typedef const char*(*simvrGetVersion_ptr)();

	typedef const char*(*simvrGetBackLog_ptr)();
	typedef void(*simvrClearBackLog_ptr)();
	typedef unsigned int(*simvrGetBackLogSize_ptr)();

	typedef bool(*simvrIsRunning_ptr)();
}

class FSIMVRPlugin : public ISIMVRPlugin
{
public:
	/** IModuleInterface implementation */
	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

	//Control
	virtual void Open(const char* serialId) override;
	virtual void Close() override;
	virtual void UpdateBackLog() override;
	virtual void UpdateSIMVR(simvrPacket* data) override;

	virtual void SetAxisProcesser(bool value) override;
	virtual void SetOrigin(bool value) override;
	virtual char* GetAppCode() override;
	virtual int GetState() override;

private:
	HINSTANCE hLibrary;

public:
	simvrOpen_ptr simvrOpen;
	simvrClose_ptr simvrClose;
	simvrWrite_ptr simvrWrite;
	simvrSetOriginMode_ptr simvrSetOrigin;
	simvrGetOriginMode_ptr simvrGetOrigin;
	simvrSetAxisProcessingMode_ptr simvrSetAxisProcessingMode;
	simvrGetAxisProcessingMode_ptr simvrGetAxisProcessingMode;
	simvrGetAppCode_ptr simvrGetAppCode;
	simvrGetState_ptr simvrGetState;
	simvrGetVersion_ptr simvrGetVersion;
	simvrGetBackLog_ptr simvrGetBackLog;
	simvrClearBackLog_ptr simvrClearBackLog;
	simvrGetBackLogSize_ptr simvrGetBackLogSize;
	simvrIsRunning_ptr simvrIsRunning;

	bool isDllLoaded;
	bool isOpen;
	bool endSIMVR;
};

IMPLEMENT_MODULE(FSIMVRPlugin, SIMVRPlugin)


void FSIMVRPlugin::StartupModule()
{
	isDllLoaded = false;
	isOpen = false;

	hLibrary = LoadLibrary(TEXT("simvr.dll"));
	if (hLibrary != NULL)
	{
		simvrOpen = reinterpret_cast<simvrOpen_ptr>(::GetProcAddress(hLibrary, "simvrOpen"));
		simvrClose = reinterpret_cast<simvrClose_ptr>(::GetProcAddress(hLibrary, "simvrClose"));
		simvrWrite = reinterpret_cast<simvrWrite_ptr>(::GetProcAddress(hLibrary, "simvrWrite"));
		simvrSetOrigin = reinterpret_cast<simvrSetOriginMode_ptr>(::GetProcAddress(hLibrary, "simvrSetOriginMode"));
		simvrGetOrigin = reinterpret_cast<simvrGetOriginMode_ptr>(::GetProcAddress(hLibrary, "simvrGetOriginMode"));
		simvrSetAxisProcessingMode = reinterpret_cast<simvrSetAxisProcessingMode_ptr>(::GetProcAddress(hLibrary, "simvrSetAxisProcessingMode"));
		simvrGetAxisProcessingMode = reinterpret_cast<simvrGetAxisProcessingMode_ptr>(::GetProcAddress(hLibrary, "simvrGetAxisProcessingMode"));
		simvrGetAppCode = reinterpret_cast<simvrGetAppCode_ptr>(::GetProcAddress(hLibrary, "simvrGetAppCode"));
		simvrGetState = reinterpret_cast<simvrGetState_ptr>(::GetProcAddress(hLibrary, "simvrGetState"));
		simvrGetVersion = reinterpret_cast<simvrGetVersion_ptr>(::GetProcAddress(hLibrary, "simvrGetVersion"));
		simvrGetBackLog = reinterpret_cast<simvrGetBackLog_ptr>(::GetProcAddress(hLibrary, "simvrGetBackLog"));
		simvrClearBackLog = reinterpret_cast<simvrClearBackLog_ptr>(::GetProcAddress(hLibrary, "simvrClearBackLog"));
		simvrGetBackLogSize = reinterpret_cast<simvrGetBackLogSize_ptr>(::GetProcAddress(hLibrary, "simvrGetBackLogSize"));
		simvrIsRunning = reinterpret_cast<simvrIsRunning_ptr>(::GetProcAddress(hLibrary, "simvrIsRunning"));

		isDllLoaded = true;
	}
	else
	{
		::MessageBox(NULL, TEXT("simvr.dll file is not found! \'Binaries/Win64/simvr.dll\'"), TEXT("simvr.dll load error!"), MB_OK);
	}

}

void FSIMVRPlugin::ShutdownModule()
{
	if (hLibrary != NULL) {
		Close();
		FreeLibrary(hLibrary);
	}
}

void FSIMVRPlugin::Open(const char* serialId)
{
	if (!isDllLoaded)
		return;

	if (simvrOpen(serialId) < 0) {
		return;
	}

}

void FSIMVRPlugin::Close()
{
	if (!isDllLoaded)
		return;

	simvrClose();
}

void FSIMVRPlugin::UpdateBackLog()
{
	if (!isDllLoaded)
		return;

	int size = (int)simvrGetBackLogSize();
	if (size > 0)
	{
		WCHAR	wStrW[512];
		size_t wLen = 0;
		const char* buffer = simvrGetBackLog();
		mbstowcs_s(&wLen, wStrW, 512, buffer, _TRUNCATE);

		UE_LOG(SIMVRLog, Log, TEXT("%s"), wStrW);
		simvrClearBackLog();
	}

}

void FSIMVRPlugin::UpdateSIMVR(simvrPacket* data)
{
	if (!isDllLoaded)
		return;

	if (simvrIsRunning() == false)
		return;

	simvrWrite(data);
}

void FSIMVRPlugin::SetAxisProcesser(bool value) 
{
	if (!isDllLoaded)
		return;

	simvrSetAxisProcessingMode(value);
}

void FSIMVRPlugin::SetOrigin(bool value)
{
	if (!isDllLoaded)
		return;

	simvrSetOrigin(value);
}

char* FSIMVRPlugin::GetAppCode()
{
	if (!isDllLoaded)
		return NULL;

	return (char*)simvrGetAppCode();
}

int FSIMVRPlugin::GetState()
{
	if (!isDllLoaded)
		return -1;

	return simvrGetState();
}