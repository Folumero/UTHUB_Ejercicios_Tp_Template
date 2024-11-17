#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "C_Teleport.generated.h"

class UStaticMeshComponent;
class UBoxComponent;
class UParticleSystem;
class USoundBase;

UCLASS()
class UTHUB_EJERCICIOSC_API AC_Teleport : public AActor
{
	GENERATED_BODY()

public:
	AC_Teleport();
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditInstanceOnly, Category = "Teleport")
	AC_Teleport* LinkedTeleport;

	UPROPERTY(VisibleAnywhere, Category = "Teleport")
	bool bIsTeleportActive = false;

	UPROPERTY(VisibleAnywhere, Category = "Teleport")
	AActor* LastTeleportedActor;
	
	UPROPERTY(EditAnywhere, Category = "Teleport | Effects")
	UParticleSystem* TeleportEffect;
	
	UPROPERTY(EditAnywhere, Category = "Teleport | Effects")
	USoundBase* TeleportSound;

	FVector GetTeleportLocation() const;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void HandleOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void HandleOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	USceneComponent* DestinationPoint;

	UPROPERTY(EditAnywhere, Category = "Teleport")
	UStaticMeshComponent* TeleportMesh;

	UPROPERTY(EditDefaultsOnly, Category = "Teleport")
	UBoxComponent* CollisionVolume;

	UPROPERTY(EditAnywhere, Category = "Teleport")
	TSubclassOf<AActor> AllowedActorClass;

private:
	void PerformTeleport(AActor* ActorToTeleport);
	void ResetTeleportState();
	void DisableCollisionForTeleport();
	void SpawnTeleportEffect(FVector Location);
	void PlayTeleportSound(FVector Location);
	void AssignLinkedTeleport(AC_Teleport* OtherTeleport);
};
