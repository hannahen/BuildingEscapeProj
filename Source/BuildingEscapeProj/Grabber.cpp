// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscapeProj.h"
#include "Grabber.h"
#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	/// Set this component to be initialized when the game starts, and to be ticked every frame.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
    FindPhysicsComponent();
    SetupInputComponent();
}

void UGrabber::FindPhysicsComponent(){
    PhysicsHandle = GetOwner()->FindComponentByClass<UPhysicsHandleComponent>();
    
    if(PhysicsHandle){
        UE_LOG(LogTemp, Warning, TEXT("Physics Handle found for %s"), *GetOwner()->GetName());
    } else {
        UE_LOG(LogTemp, Error, TEXT("No physics handle is found for %s"), *GetOwner()->GetName());
    }
}

void UGrabber::SetupInputComponent(){
    InputComponent = GetOwner()->FindComponentByClass<UInputComponent>();
    if(InputComponent){
        UE_LOG(LogTemp, Warning, TEXT("Input Component found for %s, bound keyboard actions"), *GetOwner()->GetName());
        InputComponent->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
        InputComponent->BindAction("Grab", IE_Released, this, &UGrabber::Release);
    } else {
        UE_LOG(LogTemp, Error, TEXT("No input component is found for %s"), *GetOwner()->GetName());
    }
}

void UGrabber::Grab(){
    UE_LOG(LogTemp, Warning, TEXT("Grab key pressed"));
    auto HitResult = GetFirstPhysicsBodyInReach();
    auto ComponentToGrab = HitResult.GetComponent();
    auto ActorHit = HitResult.GetActor();
    
    if(ActorHit){
        PhysicsHandle->GrabComponent(
                    ComponentToGrab,
                    NAME_None,
                    ComponentToGrab->GetOwner()->GetActorLocation(),
                    true // allow rotation
                    );
    }
}

void UGrabber::Release(){
    UE_LOG(LogTemp, Warning, TEXT("Grab key released"));
    PhysicsHandle->ReleaseComponent();
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach(){
    FVector PlayerViewPointLocation;
    FRotator PlayerViewPointRotation;
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
                                                               OUT PlayerViewPointLocation,
                                                               OUT PlayerViewPointRotation
                                                               );
    
    FString OutputLocation = PlayerViewPointLocation.ToString();
    FString OutputRotation =  PlayerViewPointRotation.ToString();
    
    //get line trace end
    FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
    
    // ray-cast out to reach
    FHitResult Hit;
    FCollisionQueryParams TraceParameters(FName(TEXT("")), false, GetOwner());
    GetWorld()->LineTraceSingleByObjectType(OUT Hit, PlayerViewPointLocation, LineTraceEnd, FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParameters);
    
    AActor* HitObject = Hit.GetActor();
    if (HitObject){
        UE_LOG(LogTemp, Warning, TEXT("We hit a ... %s"), *(HitObject->GetName()));
    }
    return Hit;
}

// Called every frame
void UGrabber::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
    
    //TODO: refactor this
    FVector PlayerViewPointLocation;
    FRotator PlayerViewPointRotation;
    GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(
                                                               OUT PlayerViewPointLocation,
                                                               OUT PlayerViewPointRotation
                                                               );
    
    FString OutputLocation = PlayerViewPointLocation.ToString();
    FString OutputRotation =  PlayerViewPointRotation.ToString();
    
    //get line trace end
    FVector LineTraceEnd = PlayerViewPointLocation + PlayerViewPointRotation.Vector() * Reach;
    
    if(PhysicsHandle->GrabbedComponent){
        PhysicsHandle->SetTargetLocation(LineTraceEnd);
    }
}

