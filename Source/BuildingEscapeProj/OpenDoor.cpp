// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscapeProj.h"
#include "OpenDoor.h"
#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{

	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;
}

void UOpenDoor::BeginPlay() {
	Super::BeginPlay();
    //ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
    Owner = GetOwner();
    
}

void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
    if(GetTotalMassOfActorsOnPlate() > 50.f){ //TODO make into a parameter
        OpenDoor();
        LastDoorOpenTime = GetWorld()->GetTimeSeconds();
    }
    
    if(GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay){
        CloseDoor();
    }

}

///helper methods
void UOpenDoor::OpenDoor(){
    //Owner->SetActorRotation(FRotator(0.0F, OpenAngle, 0.0F));
    OnOpenRequest.Broadcast();
}
void UOpenDoor::CloseDoor(){
    Owner->SetActorRotation(FRotator(0.0F, 90.0F, 0.0F));
}
float UOpenDoor::GetTotalMassOfActorsOnPlate(){
    float TotalMass = 0.0f;
    
    TArray<AActor*> OverlappingActors;
    if(!PressurePlate){
        UE_LOG(LogTemp, Error, TEXT("No pressure plate defined"));
        return TotalMass;
    }
    PressurePlate->GetOverlappingActors(OUT OverlappingActors);
    
    for (auto& Actor : OverlappingActors){
        UE_LOG(LogTemp, Warning, TEXT("Found overlapping actor %s"), *Actor->GetName());
        
        TotalMass += Actor->FindComponentByClass<UPrimitiveComponent>()->GetMass();
    }
    //UE_LOG(LogTemp, Warning, TEXT("Total mass of %f"), TotalMass);
    return TotalMass;
}