// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "SIMVRComponent.generated.h"

UCLASS( ClassGroup=(SIMVR), meta=(BlueprintSpawnableComponent) )
class USIMVRComponent : public UActorComponent
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
	UPROPERTY(BlueprintReadOnly, Category = "Status", VisibleAnywhere)
		bool isOpened;

	//AppCode
	UPROPERTY(BlueprintReadWrite, Category = "AppCode", EditAnywhere)
		FString AppCode;

	//Status
	UPROPERTY(BlueprintReadWrite, Category = "Status", EditAnywhere)
		bool AxisProcessing;

	//Rotation
	UPROPERTY(BlueprintReadWrite, Category = "Rotation and G", EditAnywhere, meta = (ClampMin = "-1.0", ClampMax = "1.0", UIMin = "-1.0", UIMax = "1.0", EditCondition="AxisProcessing"))
		float Roll;
	UPROPERTY(BlueprintReadWrite, Category = "Rotation and G", EditAnywhere, meta = (ClampMin = "-1.0", ClampMax = "1.0", UIMin = "-1.0", UIMax = "1.0", EditCondition="AxisProcessing"))
		float Pitch;
	UPROPERTY(BlueprintReadWrite, Category = "Rotation and G", EditAnywhere, meta = (ClampMin = "-1.0", ClampMax = "1.0", UIMin = "-1.0", UIMax = "1.0", EditCondition="AxisProcessing"))
		float Yaw;
	//G
	UPROPERTY(BlueprintReadWrite, Category = "Rotation and G", EditAnywhere, meta = (ClampMin = "-1.0", ClampMax = "1.0", UIMin = "-1.0", UIMax = "1.0", EditCondition="AxisProcessing"))
		float Heave;
	UPROPERTY(BlueprintReadWrite, Category = "Rotation and G", EditAnywhere, meta = (ClampMin = "-1.0", ClampMax = "1.0", UIMin = "-1.0", UIMax = "1.0", EditCondition="AxisProcessing"))
		float Sway;
	UPROPERTY(BlueprintReadWrite, Category = "Rotation and G", EditAnywhere, meta = (ClampMin = "-1.0", ClampMax = "1.0", UIMin = "-1.0", UIMax = "1.0", EditCondition="AxisProcessing"))
		float Surge;
	//Axis
	UPROPERTY(BlueprintReadWrite, Category = "Direct", EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0", EditCondition = "!AxisProcessing"))
		float Axis1Value;
	UPROPERTY(BlueprintReadWrite, Category = "Direct", EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0", EditCondition = "!AxisProcessing"))
		float Axis2Value;
	UPROPERTY(BlueprintReadWrite, Category = "Direct", EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0", EditCondition = "!AxisProcessing"))
		float Axis3Value;
	UPROPERTY(BlueprintReadWrite, Category = "Direct", EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0", EditCondition = "!AxisProcessing"))
		float Axis4Value;
	UPROPERTY(BlueprintReadWrite, Category = "Direct", EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0", EditCondition = "!AxisProcessing"))
		float Axis5Value;
	UPROPERTY(BlueprintReadWrite, Category = "Direct", EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0", EditCondition = "!AxisProcessing"))
		float Axis6Value;

	//Configures
	UPROPERTY(BlueprintReadWrite, Category = "Configures", EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
		float Speed;
	UPROPERTY(BlueprintReadWrite, Category = "Configures", EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
		float Acceleration;

	UPROPERTY(BlueprintReadWrite, Category = "Configures", EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
		float RotationMotionRatio;
	UPROPERTY(BlueprintReadWrite, Category =" Configures", EditAnywhere, meta = (ClampMin = "0.0", ClampMax = "1.0", UIMin = "0.0", UIMax = "1.0"))
		float GravityMotionRatio;

	UPROPERTY(BlueprintReadWrite, Category = "Configures", EditAnywhere)
		bool isOrigined;

	UFUNCTION(BlueprintCallable, Category = "SIMVRController")
		void OpenSIMVR();
	UFUNCTION(BlueprintCallable, Category = "SIMVRController")
		void CloseSIMVR();
};
