// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/ActorComponent.h"
#include "Grabber.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class BUILDINGESCAPEPROJ_API UGrabber : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	UGrabber();

	// Called when the game starts
	virtual void BeginPlay() override;
	
	// Called every frame
	virtual void TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction ) override;

private:
    float Reach = 100.f;
    
    UPhysicsHandleComponent* PhysicsHandle = nullptr;
    UInputComponent* InputComponent = nullptr;
    FVector LineTraceEnd;
    FVector PlayerViewPointLocation;
    FRotator PlayerViewPointRotation;
    
    //Ray-cast and grab what's in reach
    void Grab();
    void Release();
    void FindPhysicsComponent();
    
    //Setup attached (assumed) input component
    void SetupInputComponent();
    
    const FHitResult GetFirstPhysicsBodyInReach();
    
    ///Get line trace start and end based on player location
    FVector GetLineTraceEnd();
    FVector GetLineTraceStart();
};
