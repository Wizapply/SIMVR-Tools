// Fill out your copyright notice in the Description page of Project Settings.

#include "SIMVRPluginPrivatePCH.h"
#include "SIMVRMover.h"

// Sets default values for this component's properties
USIMVRMover::USIMVRMover()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;

	Controller = nullptr;
	TrackingTarget = nullptr;

	updateTime = 0.08f;
	wscale = FVector4(1.0f, 1.0f, 1.0f, 1.0f);
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
	if (TrackingTarget != nullptr) {
		previousPos = TrackingTarget->GetActorLocation();
		previousYaw = -TrackingTarget->GetActorRotation().Euler().Z;
		previousVec = FVector(0.0f, 0.0f, 0.0f);
	}else{
		UE_LOG(SIMVRLog, Warning, TEXT("%s"), L"TrackingTarget value is not specified.");
		previousPos = FVector(0.0f, 0.0f, 0.0f);
		previousYaw = 0.0f;
		previousVec = FVector(0.0f, 0.0f, 0.0f);
	}

	saveRoll = savePitch = saveYaw = saveHeave = saveSway = saveSurge = 0.0f;

	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &USIMVRMover::UpdateComponentSIMVR, updateTime, true, 1.0f);
}

void USIMVRMover::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	GetWorld()->GetTimerManager().ClearTimer(TimerHandle);
}

// Called every frame
void USIMVRMover::UpdateComponentSIMVR()
{
	if (Controller == nullptr)
		return;
	if (TrackingTarget == nullptr)
		return;
	
	FVector vec = TrackingTarget->GetActorLocation() - previousPos;
	previousPos = TrackingTarget->GetActorLocation();
	
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
	saveSurge += ToRoundDown((surge.X + surge.Y + surge.Z) * wscale.X, 2);
	saveSway +=  ToRoundDown((sway.X + sway.Y + sway.Z) * wscale.Y, 2);
	saveHeave += ToRoundDown((heave.X + heave.Y + heave.Z) * wscale.Z, 2);
		
	//YAW_G calc
	float yaws = FMath::FindDeltaAngle(-TrackingTarget->GetActorRotation().Euler().Z, previousYaw) / 10.0f * wscale.W;	//+-10“x = +-1.0
	previousYaw = -TrackingTarget->GetActorRotation().Euler().Z;
	//YAW_G
	saveYaw += ToRoundDown(yaws, 2);

	//Roll, Pitch
	float rolls = FMath::FindDeltaAngle(-TrackingTarget->GetActorRotation().Euler().X, 0.0f) / 10.0f;		//+-10“x
	float pitchs = FMath::FindDeltaAngle(-TrackingTarget->GetActorRotation().Euler().Y, 0.0f) / 10.0f;	//+-10“x
	saveRoll = ToRoundDown(rolls, 2);
	savePitch = ToRoundDown(pitchs, 2);

	Controller->Roll = saveRoll;
	Controller->Pitch = savePitch;
	Controller->Yaw = saveYaw;
	Controller->Heave = saveHeave;
	Controller->Sway = saveSway;
	Controller->Surge = saveSurge;

	//UE_LOG(LogTemp, Log, TEXT("pos %f,%f,%f"), saveSurge, saveSway, saveHeave);

	//G Half-life
	saveYaw *= 0.5f;
	saveHeave *= 0.5f;
	saveSway *= 0.5f;
	saveSurge *= 0.5f;

	//Force power
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

	return dValue > 0 ? FMath::FloorToFloat(dValue * dCoef) / dCoef :
		FMath::CeilToFloat(dValue * dCoef) / dCoef;
}