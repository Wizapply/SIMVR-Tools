// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "SIMVRComponent.generated.h"

UCLASS( ClassGroup=(SIMVR), meta=(BlueprintSpawnableComponent) )
class SIMVRSDK_UE4_API USIMVRComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USIMVRComponent();

	// Called when the game starts
	virtual void BeginPlay() override;
	// Called when the game ends
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

	//Status
	UPROPERTY(Category = Status, VisibleAnywhere)
		bool isOpened;

	//SerialNumber
	UPROPERTY(Category = SerialNumber, VisibleAnywhere)
		FString SerialNumber;

	///Rotation
	UPROPERTY(Category = Rotation, EditAnywhere, meta = (ClampMin = "-1.0", ClampMax = "1.0", UIMin = "-1.0", UIMax = "1.0"))
		float Roll;
	UPROPERTY(Category = Rotation, EditAnywhere, meta = (ClampMin = "-1.0", ClampMax = "1.0", UIMin = "-1.0", UIMax = "1.0"))
		float Pitch;
	UPROPERTY(Category = Rotation, EditAnywhere, meta = (ClampMin = "-1.0", ClampMax = "1.0", UIMin = "-1.0", UIMax = "1.0"))
		float Yaw;
	///G
	UPROPERTY(Category = G, EditAnywhere, meta = (ClampMin = "-1.0", ClampMax = "1.0", UIMin = "-1.0", UIMax = "1.0"))
		float Heave;
	UPROPERTY(Category = G, EditAnywhere, meta = (ClampMin = "-1.0", ClampMax = "1.0", UIMin = "-1.0", UIMax = "1.0"))
		float Sway;
	UPROPERTY(Category = G, EditAnywhere, meta = (ClampMin = "-1.0", ClampMax = "1.0", UIMin = "-1.0", UIMax = "1.0"))
		float Surge;
	///Configures
	UPROPERTY(Category = Configures, EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
		float Speed;
	UPROPERTY(Category = Configures, EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
		float Acceleration;
	UPROPERTY(Category = Configures, EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
		float SpeedYaw;
	UPROPERTY(Category = Configures, EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
		float AccelerationYaw;

	UPROPERTY(Category = Configures, EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
		float RotationMotionRatio;
	UPROPERTY(Category = Configures, EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
		float GravityMotionRatio;

	UPROPERTY(Category = Configures, EditAnywhere)
		bool isOrigined;

	UFUNCTION(BlueprintCallable, Category = "SIMVRController")
		void OpenSIMVR();
	UFUNCTION(BlueprintCallable, Category = "SIMVRController")
		void CloseSIMVR();
};
