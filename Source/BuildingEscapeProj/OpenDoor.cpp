// Fill out your copyright notice in the Description page of Project Settings.

#include "BuildingEscapeProj.h"
#include "OpenDoor.h"


// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = true;
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UOpenDoor::BeginPlay() {
	Super::BeginPlay();
    ActorThatOpens = GetWorld()->GetFirstPlayerController()->GetPawn();
    Owner = GetOwner();
    
}

void UOpenDoor::OpenDoor(){
    Owner->SetActorRotation(FRotator(0.0F, OpenAngle, 0.0F));
}
void UOpenDoor::CloseDoor(){
    Owner->SetActorRotation(FRotator(0.0F, 90.0F, 0.0F));
}

// Called every frame
void UOpenDoor::TickComponent( float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction )
{
	Super::TickComponent( DeltaTime, TickType, ThisTickFunction );
    if(PressurePlate->IsOverlappingActor(ActorThatOpens)){
        OpenDoor();
        LastDoorOpenTime = GetWorld()->GetTimeSeconds();
    }
    
    if(GetWorld()->GetTimeSeconds() - LastDoorOpenTime > DoorCloseDelay){
        CloseDoor();
    }

}

