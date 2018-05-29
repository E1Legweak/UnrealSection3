// Fill out your copyright notice in the Description page of Project Settings.

#include "Grabber.h"

#define OUT

// Sets default values for this component's properties
UGrabber::UGrabber()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}


// Called when the game starts
void UGrabber::BeginPlay()
{
	Super::BeginPlay();
	AOwner = GetOwner();
	FindPhysicsHandleComponent();
	SetupInputComponent();
	
}

//Look for attached Physics Handle
void UGrabber::FindPhysicsHandleComponent()
{
	if (!AOwner) { return; }
	PhysicsHandle = AOwner->FindComponentByClass<UPhysicsHandleComponent>();
	if (PhysicsHandle == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Physics Handle component not found"), *AOwner->GetName());
	}
}

//Look for attached Input Component (only appears at run time)
void UGrabber::SetupInputComponent()
{
	if (!AOwner) { return; }
	InputComp = AOwner->FindComponentByClass<UInputComponent>();
	if (InputComp)
	{
		InputComp->BindAction("Grab", IE_Pressed, this, &UGrabber::Grab);
		InputComp->BindAction("Grab", IE_Released, this, &UGrabber::Release);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("%s: Input component NOT found"), *AOwner->GetName());
	}
}


FVector UGrabber::GetReachLineEnd()
{
	FVector PLocation;
	FRotator PRot;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PLocation, OUT PRot);
	FVector LineTraceEnd = PLocation + (PRot.Vector() * Reach);
	return LineTraceEnd;
}

FVector UGrabber::GetReachLineStart()
{
	FVector PLocation;
	FRotator PRot;
	GetWorld()->GetFirstPlayerController()->GetPlayerViewPoint(OUT PLocation, OUT PRot);
	return PLocation;
}



void UGrabber::Grab() 
{
	UE_LOG(LogTemp, Warning, TEXT("Grabber is grabbing"));

	///Line trace and see if we reach any actors with physics body collision channel set
	auto HitResult = GetFirstPhysicsBodyInReach();
	auto ComponentToGrab = HitResult.GetComponent(); //gets mesh in this case
	auto ActorHit = HitResult.GetActor();
	///If we hit something then attach a physics handle
	if(ActorHit)
	{ 
		if (!PhysicsHandle) { return; }
		//Attach physics handle
		PhysicsHandle->GrabComponent(ComponentToGrab, NAME_None, ComponentToGrab->GetOwner()->GetActorLocation(), true);
	}
}

//Release physics handle
void UGrabber::Release()
{
	UE_LOG(LogTemp, Warning, TEXT("Grabber is dropping"));
	if (!PhysicsHandle) { return; }
	PhysicsHandle->ReleaseComponent();
}

// Called every frame
void UGrabber::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	
	if (!PhysicsHandle) { return; }
	//if physics handle is attached 
	if (PhysicsHandle->GrabbedComponent) 
	{
		//move the object that we're holding
		PhysicsHandle->SetTargetLocation(GetReachLineEnd());
	}
}

const FHitResult UGrabber::GetFirstPhysicsBodyInReach()
{
	///Line Trace (Raycast) out to reach distance
	FHitResult Hit;
	FCollisionQueryParams TraceParam(FName(TEXT("")), false, GetOwner());
	GetWorld()->LineTraceSingleByObjectType(OUT Hit, GetReachLineStart(), GetReachLineEnd(), FCollisionObjectQueryParams(ECollisionChannel::ECC_PhysicsBody), TraceParam);
	return Hit;
}