// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "SIMVRComponent.h"
#include "SIMVRMover.generated.h"

UCLASS( ClassGroup=(SIMVRMover), meta=(BlueprintSpawnableComponent) )
class USIMVRMover : public UActorComponent
{
	GENERATED_BODY()

private:
	USIMVRComponent* Controller;
	FTimerHandle TimerHandle;

	FVector previousPos;
	FVector previousVec;
	float previousYaw;

	float saveRoll;
	float savePitch;
	float saveYaw;
	float saveHeave;
	float saveSway;
	float saveSurge;

	//private method
	float ToRoundDown(float dValue, int iDigits);

public:	
	// Sets default values for this component's properties
	USIMVRMover();

	// Called when the game starts
	virtual void BeginPlay() override;
	// Called when the game ends
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Called every frame
	//virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;
	UFUNCTION()
	void UpdateComponentSIMVR();

	UPROPERTY(BlueprintReadWrite, Category = "Target Setting", EditAnywhere)
		AActor* TrackingTarget;
	UPROPERTY(BlueprintReadWrite, Category = "Processing Setting", EditAnywhere, meta = (ClampMin = "0.0"))
		float updateTime;
	UPROPERTY(BlueprintReadWrite, Category = "Processing Setting", EditAnywhere)
		FVector4 wscale;

	//Rotation
	UPROPERTY(BlueprintReadWrite, Category = "Force Rotation and G", EditAnywhere, meta = (ClampMin = "-1.0", ClampMax = "1.0", UIMin = "-1.0", UIMax = "1.0"))
		float RollForce;
	UPROPERTY(BlueprintReadWrite, Category = "Force Rotation and G", EditAnywhere, meta = (ClampMin = "-1.0", ClampMax = "1.0", UIMin = "-1.0", UIMax = "1.0"))
		float PitchForce;
	UPROPERTY(BlueprintReadWrite, Category = "Force Rotation and G", EditAnywhere, meta = (ClampMin = "-1.0", ClampMax = "1.0", UIMin = "-1.0", UIMax = "1.0"))
		float YawForce;
	//G
	UPROPERTY(BlueprintReadWrite, Category = "Force Rotation and G", EditAnywhere, meta = (ClampMin = "-1.0", ClampMax = "1.0", UIMin = "-1.0", UIMax = "1.0"))
		float HeaveForce;
	UPROPERTY(BlueprintReadWrite, Category = "Force Rotation and G", EditAnywhere, meta = (ClampMin = "-1.0", ClampMax = "1.0", UIMin = "-1.0", UIMax = "1.0"))
		float SwayForce;
	UPROPERTY(BlueprintReadWrite, Category = "Force Rotation and G", EditAnywhere, meta = (ClampMin = "-1.0", ClampMax = "1.0", UIMin = "-1.0", UIMax = "1.0"))
		float SurgeForce;

};
