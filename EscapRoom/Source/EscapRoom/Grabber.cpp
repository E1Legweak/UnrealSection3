// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();

	// ...
	UE_LOG(LogTemp, Warning, TEXT("Grabber reporting for shits"));

	AOwner = GetOwner();
}


// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	/// Get the player view point
	FVector PLocation;
	FRotator PRot;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PLocation, OUT PRot);
	//UE_LOG(LogTemp, Warning, TEXT("%s also %s"), *PLocation.ToString(), *PRot.ToString());

	//Draw a red line in the world to visulise grab
	FVector LineTraceEnd = PLocation + (PRot.Vector() * Reach);
	DrawDebugLine(GetWorld(), PLocation, LineTraceEnd, FColor(255,0,0), false, 0.f, 0.f, 10.f);

	///Line Trace (Raycast) out to reach distance
	FHitResult Hit;
	FCollisionQueryParams TraceParam(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(OUT Hit, PLocation, LineTraceEnd, FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParam);
	

	///See what we hit
	AActor* ActorHit = Hit.GetActor();
	if (ActorHit)
	{
		UE_LOG(LogTemp, Warning, TEXT("%s"), *ActorHit->GetName())
	}
}

