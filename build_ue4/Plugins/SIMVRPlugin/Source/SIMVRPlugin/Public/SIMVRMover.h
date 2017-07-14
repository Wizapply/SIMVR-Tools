// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "SIMVRMover.generated.h"

UCLASS( ClassGroup=(SIMVRMover), meta=(BlueprintSpawnableComponent) )
class USIMVRMover : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USIMVRMover();

	// Called when the game starts
	virtual void BeginPlay() override;
	// Called when the game ends
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

};
