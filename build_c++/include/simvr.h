﻿/**************************************************************************
*
*              Copyright (c) 2014-2017 by Wizapply.
*
*  This software is copyrighted by and is the sole property of Wizapply
*  All rights, title, ownership, or other interests in the software
*  remain the property of Wizapply.  This software may only be used
*  in accordance with the corresponding license agreement.
*  Any unauthorized use, duplication, transmission, distribution,
*  or disclosure of this software is expressly forbidden.
*
*  This Copyright notice may not be removed or modified without prior
*  written consent of Wizapply.
*
*  Wizpply reserves the right to modify this software without notice.
*  Unity : TM & Copyright Unity Technologies. All Rights Reserved
*
*  Wizapply                                info@wizapply.com
*  5F, KS Building,                        http://wizapply.com
*  3-7-10 Ichiokamotomachi, Minato-ku,
*  Osaka, 552-0002, Japan
*
***************************************************************************/

#ifndef __SIMVR__
#define __SIMVR__

namespace SIMVRSDK {

#ifndef __SIMVR_DATAPACKET__
#define __SIMVR_DATAPACKET__

//! SIMVR Data packet
typedef struct _simvr_data_packet
{
	//Axis position controls
	float axis1;
	float axis2;
	float axis3;
	float axis4;
	float axis5;
	float axis6;

	//Axis speed/accel controls
	float speedAxis123;
	float accelAxis123;
	float speedAxis4;
	float accelAxis4;

	//Axis Processing
	float roll;
	float pitch;
	float yaw;
	float heave;
	float sway;
	float surge;

	float rotationMotionRatio;
	float gravityMotionRatio;

	int commandCount;
	int commandStride; //コマンド毎のオフセットバイト数
	char command[256];

} SIMVRDataPacket;
#endif /*__SIMVR_DATAPACKET__*/

/////////// VARS AND DEFS ///////////

#ifdef WIN32
	#ifdef _SIMVR_EXPORTS
    #define SIMVRPORT __declspec(dllexport)
    #else
	#define SIMVRPORT __declspec(dllimport)
    #endif
#endif /*WIN32*/

#ifdef MACOSX
	#define SIMVRPORT 
#endif	/*MACOSX*/
	
#ifdef LINUX
	#define SIMVRPORT
#endif	/*LINUX*/

struct Property;

#define SIMVR_SDKVERSION "1.2.0"

class SIMVRPORT SIMVR
{
public:
	//!Constructor
	SIMVR();
	//!Destructor
	~SIMVR();

	//Initialize
	/*!	@brief Open the SIMVR System
		@param appCode : Appication Code(Not a serial number)
		@return If successful, the return value is 0< */
	int Open(const char* appCode);
	/*!	@brief Close the SIMVR System */
	int Close();

	//Write
	/*!	@brief This function sets data to SIMVR. 
		@param qt Set an image processing method. */
	int Write(const SIMVRDataPacket* packet);

	//Callback
	/*!	@brief This function sets data to SIMVR.
		@param qt Set an image processing method. */
	void SetCallbackUpdateFunction(void(*func)());

	//Properties
	/*!	@brief This function sets data to SIMVR.
		@param qt Set an image processing method. */
	void SetOriginMode(bool value);
	bool GetOriginMode();

	void SetAxisProcessingMode(bool value);
	bool GetAxisProcessingMode();

	const char* GetAppCode() const;
	const char* GetSIMVRSerialNumber() const;

	int GetState();
	const char* GetVersion() const;
	bool IsRunning() const;

	int GetStatusEXT4() const;

private:
	void Update(SIMVRDataPacket& packet);
	void ThreadUpdate();
	void LogError();

	void LANStart();
	void LANStop();
	void LANUpdate(SIMVRDataPacket& packet);

	Property* property;
};

}

#endif
