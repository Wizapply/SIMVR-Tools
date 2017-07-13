// Fill out your copyright notice in the Description page of Project Settings.

#include "SIMVRPluginPrivatePCH.h"
#include "SIMVRComponent.h"

// Sets default values for this component's properties
USIMVRComponent::USIMVRComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	Roll = 0.0f;
	Pitch = 0.0f;
	Yaw = 0.0f;
	Heave = 0.0f;
	Sway = 0.0f;
	Surge = 0.0f;
	Speed = 1.0f;
	Acceleration = 1.0f;
	SpeedYaw = 1.0f;
	AccelerationYaw = 1.0f;

	AxisProcessing = true;

	RotationMotionRatio = 0.3f;
	GravityMotionRatio = 0.7f;

	isOrigined = false;
	isOpened = false;
	// ...
}


// Called when the game starts
void USIMVRComponent::BeginPlay()
{
	Super::BeginPlay();
	OpenSIMVR();
}

void USIMVRComponent::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
	CloseSIMVR();
}

// Called every frame
void USIMVRComponent::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	simvrPacket p;
	p.roll = Roll;
	p.pitch = Pitch;
	p.yaw = Yaw;
	p.heave = Heave;
	p.sway = Sway;
	p.surge = Surge;
	p.speedAxis123 = Speed;
	p.accelAxis123 = Acceleration;
	p.speedAxis4 = SpeedYaw;
	p.accelAxis4 = AccelerationYaw;

	if (AxisProcessing) {

	}

	p.rotationMotionRatio = RotationMotionRatio;
	p.gravityMotionRatio = GravityMotionRatio;

	p.commandCount = 0;

	ISIMVRPlugin::Get().SetAxisProcesser(AxisProcessing);
	ISIMVRPlugin::Get().SetOrigin(isOrigined);
	ISIMVRPlugin::Get().UpdateSIMVR(&p);

	ISIMVRPlugin::Get().UpdateBackLog();
}

void USIMVRComponent::OpenSIMVR()
{
	ISIMVRPlugin::Get().Open(TCHAR_TO_UTF8(*AppCode));
}

void USIMVRComponent::CloseSIMVR()
{
	ISIMVRPlugin::Get().Close();
	ISIMVRPlugin::Get().UpdateBackLog();
}
