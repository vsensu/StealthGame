// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSObjectiveActor.h"

#include "FPSCharacter.h"
#include "Components/StaticMeshComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFPSObjectiveActor::AFPSObjectiveActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = MeshComp;

	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SphereComp->SetCollisionResponseToChannel(ECC_Pawn, ECollisionResponse::ECR_Overlap);
	SphereComp->SetupAttachment(MeshComp);

}

// Called when the game starts or when spawned
void AFPSObjectiveActor::BeginPlay()
{
	Super::BeginPlay();
	
	SphereComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSObjectiveActor::OnCollision);
}

void AFPSObjectiveActor::OnCollision(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult & SweepResult)
{
	auto *Char = Cast<AFPSCharacter>(OtherActor);
	if(Char)
	{
		Char->bIsCarryingObjective = true;
		PlayEffects();
		Destroy();
	}
}

void AFPSObjectiveActor::PlayEffects() const
{
	UGameplayStatics::SpawnEmitterAtLocation(this, PickupFX, GetActorLocation());
}

