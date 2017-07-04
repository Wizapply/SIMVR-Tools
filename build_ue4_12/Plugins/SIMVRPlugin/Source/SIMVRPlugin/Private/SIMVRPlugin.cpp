// Copyright 1998-2015 Epic Games, Inc. All Rights Reserved.


#include "SIMVRPluginPrivatePCH.h"
#include <stdlib.h>	// for mbstowcs_s(), wcstombs_s()
#include <locale.h>	// for setlocale()

//LoadLibrary
extern "C" {
	typedef int(*simvrOpen_ptr)(const char* serialNo);
	typedef int(*simvrClose_ptr)();
	typedef void(*simvrWrite_ptr)(SIMVRPacket* packet);

	typedef void(*simvrSetOriginMode_ptr)(bool value);
	typedef bool(*simvrGetOriginMode_ptr)();

	typedef void(*simvrSetAxisProcessingMode_ptr)(bool value);
	typedef bool(*simvrGetAxisProcessingMode_ptr)();

	typedef const char*(*simvrGetSerialNumber_ptr)();

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
	virtual void UpdateSIMVR(SIMVRPacket* data) override;

	virtual void SetAxisProcesser(bool value) override;
	virtual void SetOrigin(bool value) override;
	virtual char* GetSerialNumber() override;

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
	simvrGetSerialNumber_ptr simvrGetSerialNumber;
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
		simvrGetSerialNumber = reinterpret_cast<simvrGetSerialNumber_ptr>(::GetProcAddress(hLibrary, "simvrGetSerialNumber"));
		simvrGetBackLog = reinterpret_cast<simvrGetBackLog_ptr>(::GetProcAddress(hLibrary, "simvrGetBackLog"));
		simvrClearBackLog = reinterpret_cast<simvrClearBackLog_ptr>(::GetProcAddress(hLibrary, "simvrClearBackLog"));
		simvrGetBackLogSize = reinterpret_cast<simvrGetBackLogSize_ptr>(::GetProcAddress(hLibrary, "simvrGetBackLogSize"));
		simvrIsRunning = reinterpret_cast<simvrIsRunning_ptr>(::GetProcAddress(hLibrary, "simvrIsRunning"));

		setlocale(LC_ALL, "japanese");

		isDllLoaded = true;
	}
	else
	{
		::MessageBox(NULL, TEXT("simvr.dll file is not found!"), TEXT("simvr.dll load error!"), MB_OK);
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

	if (simvrOpen("") < 0) {
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
		UE_LOG(LogTemp, Warning, TEXT("%s"), wStrW);
		simvrClearBackLog();
	}

}

void FSIMVRPlugin::UpdateSIMVR(SIMVRPacket* data)
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

char* FSIMVRPlugin::GetSerialNumber()
{
	if (!isDllLoaded)
		return NULL;

	return (char*)simvrGetSerialNumber();
}
