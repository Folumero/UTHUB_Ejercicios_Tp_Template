#include "C_Teleport.h"
#include "Components/BoxComponent.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundBase.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "TimerManager.h"

AC_Teleport::AC_Teleport()
{
	PrimaryActorTick.bCanEverTick = true;

	TeleportMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TeleportMesh"));
	RootComponent = TeleportMesh;

	CollisionVolume = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionVolume"));
	CollisionVolume->SetupAttachment(RootComponent);

	DestinationPoint = CreateDefaultSubobject<USceneComponent>(TEXT("DestinationPoint"));
	DestinationPoint->SetupAttachment(RootComponent);

	LastTeleportedActor = nullptr;
}

void AC_Teleport::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FVector AC_Teleport::GetTeleportLocation() const
{
	return DestinationPoint->GetComponentLocation();
}

void AC_Teleport::BeginPlay()
{
	Super::BeginPlay();

	CollisionVolume->OnComponentBeginOverlap.AddDynamic(this, &AC_Teleport::HandleOverlapBegin);
	CollisionVolume->OnComponentEndOverlap.AddDynamic(this, &AC_Teleport::HandleOverlapEnd);
	
	if (LinkedTeleport)
	{
		AssignLinkedTeleport(LinkedTeleport);
	}
}

void AC_Teleport::HandleOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!LinkedTeleport || bIsTeleportActive || OtherActor == nullptr)
		return;

	if (!OtherActor->IsA(AllowedActorClass))
		return;

	if (OtherActor == LastTeleportedActor)
		return;

	PerformTeleport(OtherActor);
}

void AC_Teleport::HandleOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
	UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OtherActor == LastTeleportedActor)
		LastTeleportedActor = nullptr;

	if (LinkedTeleport)
		LinkedTeleport->bIsTeleportActive = false;
}

void AC_Teleport::PerformTeleport(AActor* ActorToTeleport)
{
	if (!LinkedTeleport || !ActorToTeleport)
		return;

	LastTeleportedActor = ActorToTeleport;
	LinkedTeleport->DisableCollisionForTeleport();

	SpawnTeleportEffect(GetActorLocation());
	PlayTeleportSound(GetActorLocation());

	bIsTeleportActive = true;
	LinkedTeleport->bIsTeleportActive = true;
	ActorToTeleport->SetActorLocation(LinkedTeleport->GetTeleportLocation());

	LinkedTeleport->SpawnTeleportEffect(LinkedTeleport->GetTeleportLocation());
	LinkedTeleport->PlayTeleportSound(LinkedTeleport->GetTeleportLocation());

	GetWorld()->GetTimerManager().SetTimerForNextTick(this, &AC_Teleport::ResetTeleportState);
}

void AC_Teleport::SpawnTeleportEffect(FVector Location)
{
	if (TeleportEffect)
	{
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TeleportEffect, Location);
	}
}

void AC_Teleport::PlayTeleportSound(FVector Location)
{
	if (TeleportSound)
	{
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), TeleportSound, Location);
	}
}

void AC_Teleport::DisableCollisionForTeleport()
{
	CollisionVolume->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [this]()
	{
		CollisionVolume->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}, 0.5f, false);
}

void AC_Teleport::ResetTeleportState()
{
	bIsTeleportActive = false;

	if (LinkedTeleport)
		LinkedTeleport->bIsTeleportActive = false;
}

void AC_Teleport::AssignLinkedTeleport(AC_Teleport* OtherTeleport)
{
	if (OtherTeleport && OtherTeleport->LinkedTeleport == nullptr)
	{
		OtherTeleport->LinkedTeleport = this;
		
	}
}
