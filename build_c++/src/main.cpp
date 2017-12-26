/**************************************************************************
*
*              Copyright (c) 2014-2016 by Wizapply.
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


/* -- Include files ----------------------------------------------------- */

#include <simvr.h>	//SIMVR SDK
#include <simvr_state.h>

#include <string>
#include <iostream>
#include <thread>

/* -- Global variable definition ---------------------------------------- */

//Objects
SIMVRSDK::SIMVR* g_pSIMVRSystem = NULL;

/* -- Main -------------------------------------------------------------- */
SIMVRSDK::SIMVRDataPacket DefaultPacket()
{
	SIMVRSDK::SIMVRDataPacket packet = {};
	packet.axis1 = 0.5f;
	packet.axis2 = 0.5f;
	packet.axis3 = 0.5f;
	packet.axis4 = 0.5f;

	//Axis speed/accel controls
	packet.speedAxis123 = 1.0f;
	packet.accelAxis123 = 0.5f;
	packet.speedAxis4 = 1.0f;
	packet.accelAxis4 = 0.5f;

	//Axis Processing
	packet.roll = 0.0f;
	packet.pitch = 0.0f;
	packet.yaw = 0.0f;
	packet.heave = 0.0f;
	packet.sway = 0.0f;
	packet.surge = 0.0f;

	packet.rotationMotionRatio = 1.0f;
	packet.gravityMotionRatio = 0.0f;

	packet.commandCount = 0;

	return packet;
}

bool updateSIMVRStatus()
{
	int stateNo = g_pSIMVRSystem->GetState();
	if (stateNo != -1 && stateNo != (int)SIMVRSDK::State::Running && stateNo != (int)SIMVRSDK::State::StopActuator)
	{
		return false;
	}
	return true;
}

int main(int argc, char *argv[])
{
	g_pSIMVRSystem = new SIMVRSDK::SIMVR();
	g_pSIMVRSystem->Open("");

	std::cout << "SIMVR-START..." << std::endl;

	std::this_thread::sleep_for(std::chrono::seconds(1));

	g_pSIMVRSystem->SetOriginMode(false);
	g_pSIMVRSystem->SetAxisProcessingMode(true);	//Axis mode

	auto packet = DefaultPacket();

	std::cout << "This program can change ROLL, PITCH, YAW of SIMVR. \nSpecification value [-1.0 to 1.0]. And, this is ended in an [exit] input. " << std::endl;

	while (updateSIMVRStatus())
	{
		std::string input;

		//ROLL
		std::cout << "ROLL -> ";
		std::cin >> input;
		if (input == "exit") break;

		auto d = static_cast<float>(atof(input.c_str()));
		packet.roll = d;

		//PITCH
		std::cout << "PITCH -> ";
		std::cin >> input;
		if (input == "exit") break;

		d = static_cast<float>(atof(input.c_str()));
		packet.pitch = d;

		//YAW
		std::cout << "YAW -> ";
		std::cin >> input;
		if (input == "exit") break;

		d = static_cast<float>(atof(input.c_str()));
		packet.yaw = d;

		g_pSIMVRSystem->SetOriginMode(false);
		g_pSIMVRSystem->Write(&packet);
	}

	std::cout << "SIMVR-SHUTDOWN" << std::endl;

	delete g_pSIMVRSystem;

	return 0;
}
