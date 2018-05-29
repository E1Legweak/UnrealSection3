// Fill out your copyright notice in the Description page of Project Settings.

#include "OpenDoor.h"
#include "GameFramework/Actor.h"

#define OUT

// Sets default values for this component's properties
UOpenDoor::UOpenDoor()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

}


// Called when the game starts
void UOpenDoor::BeginPlay()
{
	Super::BeginPlay();

	AOwner = GetOwner();

	if (PressurePlate == nullptr) 
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Pressure Plate not Assigned"), *AOwner->GetName());
	}
}


// Called every frame
void UOpenDoor::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	// Poll trigger volume every frame
	if (GetMassTotal()>TriggerMass)
	{
		OnOpen.Broadcast();
	}
	//Check if it is time to close the door
	
	else 
	{		
		OnClose.Broadcast();
	}
}

float UOpenDoor::GetMassTotal()
{
	float TotalMass = 0.f;

	//Find all overlapping actors
	TArray<AActor*> OverlappingActors;
	if (!PressurePlate) { return TotalMass; }
	PressurePlate->GetOverlappingActors(OUT OverlappingActors);

	//Iterate through them adding their masses

	for (const auto* Act : OverlappingActors)
	{
		TotalMass += Act->FindComponentByClass<UPrimitiveComponent>()->GetMass();
		//UE_LOG(LogTemp, Warning, TEXT("%s is on plate"), *Act->GetName());
	}
	return TotalMass;
}

