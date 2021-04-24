// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
//#include "Engine/Classes/Engine/World.h"
//#include "Engine/Classes/Engine/SkeletalMesh.h"

//#include "Engine/Public/CollisionQueryParams.h"


#include "INFLands.h"
#include "BaseCharacter.h"
#include "BaseEnemy.generated.h"

UENUM(BlueprintType)
enum class EAIState : uint8
{
	Idle,

	Suspicious,

	Alerted
};

class UPawnSensingComponent;

/**
 * 
 */
UCLASS()
class INFLANDS_API ABaseEnemy : public ACharacter
{
	GENERATED_BODY()
	
public:
	
	// Sets default values for this character's properties
	ABaseEnemy();
	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UPawnSensingComponent* PawnSensingComp;

	//// First-person mesh (arms), visible only to the owning player.
	//UPROPERTY(VisibleDefaultsOnly, Category = "BaseEnemy")
	//class USkeletalMeshComponent*  BaseEnemyMesh;

	//Estadísticas:
	//Expose a float property
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BaseCharacter")
		float Health = 100;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "BaseCharacter")
		bool isDead;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BaseCharacter")
		float Corrupt = 10;	
	
	void CalculateDead();

	UFUNCTION(BlueprintCallable, Category = "BaseCharacter")
	void CalculateHealth(float delta);

	UFUNCTION(BlueprintCallable, Category = "BaseCharacter")
	void CalculateCorrupt(float delta);


	UFUNCTION()
	void OnPawnSeen(APawn* SeenPawn);

	UFUNCTION()
	void OnNoiseHeard(APawn* NoiseInstigator, const FVector& Location, float Volume);

	//Handler AI Estate
	UPROPERTY(VisibleAnyWhere, BlueprintReadOnly, Category = "AI")
	EAIState GuardState;

	FRotator OriginalRotation;

	FRotator LastLookatKnowed;

	void SetGuardState(EAIState NewState);

	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
	void OnStateChanged(EAIState NewState);



	FTimerHandle TimerHandle_ResetOrientation;

	FTimerHandle TimerHandle_ResetAlerted;

	UFUNCTION()
	void ResetOrientation();

	UFUNCTION()
	void ResetAlerted();

	

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	


	

	//Damage is Negative Is aplied over the CalculeHealth Method
	UPROPERTY(BlueprintReadWrite, EditAnyWhere, Category = "BaseEnemy")
	float BaseDamage=-8.0f;
	
	/*UFUNCTION(BlueprintCallable, Category="BaseEnemy")
	virtual void CheckDamage(FName SocketInicio, FName SocketFinal, bool Reset) override;
*/
//protected:
	
	UPROPERTY(BlueprintReadOnly, VisibleAnyWhere, Category = "BaseEnemy")
	FHitResult Hit;








	/*UPROPERTY(BlueprintReadOnly, VisibleAnyWhere, Category = "BaseEnemy")
	FCollisionQueryParams  F_CQParam;

	UPROPERTY(BlueprintReadOnly, VisibleAnyWhere, Category = "BaseEnemy")
	FCollisionResponseParams F_CRParam;*/
};
