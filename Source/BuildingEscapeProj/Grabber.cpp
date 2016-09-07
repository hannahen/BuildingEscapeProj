// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscapeProj.h"
#include "Grabber.h"
#define OUT

UGrabber::UGrabber()
{
	/// Set this component to be initialized when the game starts, and to be ticked every frame.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}

void UGrabber::BeginPlay()
{
	Super::BeginPlay();
    FindPhysicsComponent();
    SetupInputComponent();
    
}

void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
    Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
    if(!PhysicsHandle) { return; }
    if(PhysicsHandle->GrabbedComponent){
        PhysicsHandle->SetTargetLocation(GetLineTraceEnd());
    }
}

void UGrabber::FindPhysicsComponent(){
    PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
    
    if(!PhysicsHandle){
        UE_LOG(LogTemp, Error, TEXT("No physics handle is found for %s"), *GetOwner()->GetName());
    }
}

void UGrabber::SetupInputComponent(){
    InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
    if(InputComponent){
        InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
        InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
    } else {
        UE_LOG(LogTemp, Error, TEXT("No input component is found for %s"), *GetOwner()->GetName());
    }
}

void UGrabber::Grab(){
    auto HitResult = GetFirstPhysicsBodyInReach();
    auto ComponentToGrab = HitResult.GetComponent();
    auto ActorHit = HitResult.GetActor();
    
    if(ActorHit){
        if(!PhysicsHandle) { return; }
        PhysicsHandle->GrabComponent(
                    ComponentToGrab,
                    NAME_None,
                    ComponentToGrab->GetOwner()->GetActorLocation(),
                    true // allow rotation
                    );
    }
}

void UGrabber::Release(){
    if(!PhysicsHandle) { return; }
    PhysicsHandle->ReleaseComponent();
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach(){
    // ray-cast out to reach
    FHitResult Hit;
    FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
    GetWorld()->LineTraceSingleByObjectType(OUT Hit, GetLineTraceStart(), GetLineTraceEnd(), FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParameters);
    
    AActor* HitObject = Hit.GetActor();
    return Hit;
}

FVector UGrabber::GetLineTraceEnd(){
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
                                                               OUT PlayerViewPointLocation,
                                                               OUT PlayerViewPointRotation
                                                               );
    FString OutputLocation = PlayerViewPointLocation.ToString();
    FString OutputRotation =  PlayerViewPointRotation.ToString();
    return PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
}
FVector UGrabber::GetLineTraceStart(){
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
                                                               OUT PlayerViewPointLocation,
                                                               OUT PlayerViewPointRotation
                                                               );
    return PlayerViewPointLocation;
}

