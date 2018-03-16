// Fill out your copyright notice in the Description page of Project Settings.

#include "SIMVRPluginPrivatePCH.h"
#include "SIMVRMover.h"

// Sets default values for this component's properties
USIMVRMover::USIMVRMover()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	Controller = nullptr;
	TrackingTarget = nullptr;

	updateTime = 0.032f;
	wscale = FVector4(1.0f, 1.0f, 1.0f, 1.0f);

	//private var
	currTime = 0;
}


// Called when the game starts
void USIMVRMover::BeginPlay()
{
	Super::BeginPlay();
	Controller = GetOwner()->FindComponentByClass<USIMVRComponent>();

	if (Controller == nullptr) {
		UE_LOG(SIMVRLog, Warning, TEXT("%s"), L"There is no SIMVRComponent. Please add SIMVRComponent.");
		return;
	}
	if (TrackingTarget == nullptr) {
		UE_LOG(SIMVRLog, Warning, TEXT("%s"), L"TrackingTarget value is not specified.");
		return;
	}

	currTime = updateTime;
	previousPos = TrackingTarget->GetTransform().GetLocation();
	previousYaw = 0.0f;

	previousVec = FVector(0.0f, 0.0f, 0.0f);

	saveRoll = savePitch = saveYaw = saveHeave = saveSway = saveSurge = 0.0f;
}

void USIMVRMover::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void USIMVRMover::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );

	if (Controller == nullptr)
		return;
	if (TrackingTarget == nullptr)
		return;

	currTime -= DeltaTime;
	if (currTime <= 0.0f)
	{
		FVector vec = TrackingTarget->GetTransform().GetLocation() - previousPos;
		previousPos = TrackingTarget->GetTransform().GetLocation();

		FVector vecChange = vec - previousVec;
		previousVec = vec;

		//G_calc
		FVector surge, sway, heave;
		surge.X = vecChange.X * TrackingTarget->GetActorForwardVector().X;
		surge.Y = vecChange.Y * TrackingTarget->GetActorForwardVector().Y;
		surge.Z = vecChange.Z * TrackingTarget->GetActorForwardVector().Z;
		sway.X = vecChange.X * TrackingTarget->GetActorRightVector().X;
		sway.Y = vecChange.Y * TrackingTarget->GetActorRightVector().Y;
		sway.Z = vecChange.Z * TrackingTarget->GetActorRightVector().Z;
		heave.X = vecChange.X * TrackingTarget->GetActorUpVector().X;
		heave.Y = vecChange.Y * TrackingTarget->GetActorUpVector().Y;
		heave.Z = vecChange.Z * TrackingTarget->GetActorUpVector().Z;

		//G
		saveSurge = Controller->Surge = ToRoundDown((surge.X + surge.Y + surge.Z) * wscale.X, 2);
		saveSway = Controller->Sway = ToRoundDown((sway.X + sway.Y + sway.Z) * wscale.Y, 2);
		saveHeave = Controller->Heave = ToRoundDown((heave.X + heave.Y + heave.Z) * wscale.Z, 2);

		//YAW_G calc
		float yaws = FMath::FindDeltaAngle(-TrackingTarget->GetTransform().GetRotation().Euler().Z, previousYaw) / 10.0f * wscale.W;	//+-10“x = +-1.0
		previousYaw = TrackingTarget->GetTransform().GetRotation().Euler().Z;
		//YAW_G
		saveYaw = Controller->Yaw = ToRoundDown(yaws, 2);

		//Roll, Pitch
		float rolls = FMath::FindDeltaAngle(-TrackingTarget->GetTransform().GetRotation().Euler().X, 0.0f) / 10.0f;		//+-10“x
		float pitchs = FMath::FindDeltaAngle(-TrackingTarget->GetTransform().GetRotation().Euler().Y, 0.0f) / 10.0f;	//+-10“x

		saveRoll = Controller->Roll = ToRoundDown(rolls, 2);
		savePitch = Controller->Pitch = ToRoundDown(pitchs, 2);

		currTime = updateTime;
	}
	else {
		Controller->Roll = saveRoll;
		Controller->Pitch = savePitch;
		Controller->Yaw = saveYaw;
		Controller->Heave = saveHeave;
		Controller->Sway = saveSway;
		Controller->Surge = saveSurge;
	}

	Controller->Roll += RollForce;
	Controller->Pitch += PitchForce;
	Controller->Yaw += YawForce;
	Controller->Heave += HeaveForce;
	Controller->Sway += SwayForce;
	Controller->Surge += SurgeForce;

}

float USIMVRMover::ToRoundDown(float dValue, int iDigits)
{
	float dCoef = FMath::Pow(10, iDigits);

	return dValue > 0 ? FMath::Floor(dValue * dCoef) / dCoef :
		FMath::Ceil(dValue * dCoef) / dCoef;
}