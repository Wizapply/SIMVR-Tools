// Fill out your copyright notice in the Description page of Project Settings.

#include "SIMVRMover.h"
#include "SIMVRPluginPrivatePCH.h"

class SIMVRMover_Status6DOF
{
private:
	float s_maxRoll = 0.0f;
	float s_maxPitch = 0.0f;
	float s_maxYaw = 0.0f;
	float s_maxHeave = 0.0f;
	float s_maxSway = 0.0f;
	float s_maxSurge = 0.0f;

	float s_minRoll = 0.0f;
	float s_minPitch = 0.0f;
	float s_minYaw = 0.0f;
	float s_minHeave = 0.0f;
	float s_minSway = 0.0f;
	float s_minSurge = 0.0f;

public:
	void ZeroResetStatusUpdate()
	{
		s_maxRoll = 0.0f;
		s_maxPitch = 0.0f;
		s_maxYaw = 0.0f;
		s_maxHeave = 0.0f;
		s_maxSway = 0.0f;
		s_maxSurge = 0.0f;

		s_minRoll = 0.0f;
		s_minPitch = 0.0f;
		s_minYaw = 0.0f;
		s_minHeave = 0.0f;
		s_minSway = 0.0f;
		s_minSurge = 0.0f;
	}

	void MaxStatusUpdate(float roll, float pitch, float yaw, float heave, float sway, float surge)
	{
		s_maxRoll = FMath::Max(s_maxRoll, roll);
		s_minRoll = FMath::Min(s_minRoll, roll);

		s_maxPitch = FMath::Max(s_maxPitch, pitch);
		s_minPitch = FMath::Min(s_minPitch, pitch);

		s_maxYaw = FMath::Max(s_maxYaw, yaw);
		s_minYaw = FMath::Min(s_minYaw, yaw);

		s_maxHeave = FMath::Max(s_maxHeave, heave);
		s_minHeave = FMath::Min(s_minHeave, heave);

		s_maxSway = FMath::Max(s_maxSway, sway);
		s_minSway = FMath::Min(s_minSway, sway);

		s_maxSurge = FMath::Max(s_maxSurge, surge);
		s_minSurge = FMath::Min(s_minSurge, surge);
	}

	float Roll()
	{
		return s_maxRoll + s_minRoll;
	}
	float Pitch()
	{
		return s_maxPitch + s_minPitch;
	}
	float Yaw()
	{
		return s_maxYaw + s_minYaw;
	}
	float Heave()
	{
		return s_maxHeave + s_minHeave;
	}
	float Sway()
	{
		return s_maxSway + s_minSway;
	}
	float Surge()
	{
		return s_maxSurge + s_minSurge;
	}
};

// Sets default values for this component's properties
USIMVRMover::USIMVRMover()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	Controller = nullptr;
	TrackingTarget = nullptr;

	Gcalc6Dof = new SIMVRMover_Status6DOF();

	updateTime = 0.5f;
	wscale = FVector(1.0f, 1.0f, 1.0f);
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
	currTime = updateTime;
	Gcalc6Dof->ZeroResetStatusUpdate();
}

void USIMVRMover::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}

// Called every frame
void USIMVRMover::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (Controller == nullptr)
		return;

	if (TrackingTarget != nullptr)
	{
		FVector vec = (TrackingTarget->GetActorLocation() - previousPos) / DeltaTime;
		FVector vecChange = vec - previousVec;
		previousPos = TrackingTarget->GetActorLocation();
		previousVec = vec;

		vecChange.X /= wscale.X;
		vecChange.Y /= wscale.Y;
		vecChange.Z /= wscale.Z;

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
		float dataSurge = ToRoundDown((surge.X + surge.Y + surge.Z), 2);
		float dataSway = ToRoundDown((sway.X + sway.Y + sway.Z), 2);
		float dataHeave = ToRoundDown((heave.X + heave.Y + heave.Z), 2);

		//YAW_G calc
		float yaws = FMath::FindDeltaAngleDegrees(-TrackingTarget->GetActorRotation().Euler().Z, previousYaw);	//+-10“x = +-1.0
		previousYaw = -TrackingTarget->GetActorRotation().Euler().Z;

		//Roll, Pitch
		float rolls = FMath::FindDeltaAngleDegrees(-TrackingTarget->GetActorRotation().Euler().X, 0.0f) / 10.0f;	//+-10“x
		float pitchs = FMath::FindDeltaAngleDegrees(-TrackingTarget->GetActorRotation().Euler().Y, 0.0f) / 10.0f;	//+-10“x

		Gcalc6Dof->MaxStatusUpdate(ToRoundDown(rolls, 2), ToRoundDown(pitchs, 2), ToRoundDown(yaws, 2),
			dataHeave, dataSway, dataHeave);

		//Update
		currTime -= DeltaTime;
		if (currTime <= 0.0f)
		{
			saveRoll = ToRoundDown(rolls, 2);
			savePitch = ToRoundDown(pitchs, 2);
			saveYaw = Gcalc6Dof->Yaw();
			saveHeave = Gcalc6Dof->Heave();
			saveSway = Gcalc6Dof->Sway();
			saveSurge = Gcalc6Dof->Surge();

			Gcalc6Dof->ZeroResetStatusUpdate();
			currTime = updateTime;
		}

		Controller->Roll = saveRoll;
		Controller->Pitch = savePitch;
		Controller->Yaw = saveYaw;
		Controller->Heave = saveHeave;
		Controller->Sway = saveSway;
		Controller->Surge = saveSurge;

		//UE_LOG(LogTemp, Log, TEXT("pos %f,%f,%f"), saveSurge, saveSway, saveHeave);
	}

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