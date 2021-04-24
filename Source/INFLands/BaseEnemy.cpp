// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseEnemy.h"
#include "BaseCharacter.h"


#include "GameFramework/Pawn.h"


#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"

#include "Net/UnrealNetwork.h"


#include "Math/RotationMatrix.h"

#include "Blueprint/AIBlueprintHelperLibrary.h" 

//added lastly
#include "Runtime/Engine/Public/TimerManager.h"


int32 DebugPawnSensingDrawing = 0;
FAutoConsoleVariableRef   CVARDebugPawnDrawing(
	TEXT("INF.DebugPawnSensing"),
	DebugPawnSensingDrawing,
	TEXT("Draw Dbug Lines when pawns detect to hero"),
	ECVF_Cheat);

// Sets default values
ABaseEnemy::ABaseEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//// Create a Base enemy mesh component for the owning player.
	//BaseEnemyMesh = GetMesh();//CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BaseEnemyMesh"));



	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));

	PawnSensingComp->OnSeePawn.AddDynamic(this, &ABaseEnemy::OnPawnSeen);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &ABaseEnemy::OnNoiseHeard);

	GuardState = EAIState::Idle;


}

// Called when the game starts or when spawned
void ABaseEnemy::BeginPlay()
{
	Super::BeginPlay();

	OriginalRotation = GetActorRotation();

	 

}



void ABaseEnemy::CalculateDead()
{
	if ((Health <= 0) || (Corrupt >= 100))
		isDead = true;
	else
		isDead = false;

}



//To increase Health or to Damage if delta is negative
void ABaseEnemy::CalculateHealth(float delta)
{
	Health += delta;
	
	CalculateDead();

}


void ABaseEnemy::CalculateCorrupt(float delta)
{
	Corrupt += delta;

	Corrupt = FMath::Clamp(Corrupt, 0.0f, 100.0f);
	CalculateDead();
}


void ABaseEnemy::OnPawnSeen(APawn * SeenPawn)
{
	if (SeenPawn == nullptr) {
		return;
	}

	if (DebugPawnSensingDrawing > 0)
	{
		DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32.0f, 12, FColor::Red, false, 10.0f);
	}

	

	/*APlayerBTGameMode* GM = Cast< APlayerBTGameMode>(GetWorld()->GetAuthGameMode());
	if (GM)
	{
		GM->CompleteMission(SeenPawn, false);
	}*/

	SetGuardState(EAIState::Alerted);

	//3 segundos si hacer nada vuelve a estado sospechoso
	GetWorldTimerManager().ClearTimer(TimerHandle_ResetAlerted);
	GetWorldTimerManager().SetTimer(TimerHandle_ResetAlerted, this, &ABaseEnemy::ResetAlerted, 3.0f);
	

	AController* AIC = GetController();
	if (AIC)
	{
		
		UAIBlueprintHelperLibrary::SimpleMoveToLocation(AIC, SeenPawn->GetActorLocation());
		//UNavigationSystem::SimpleMoveToLocation(AIC, SeenPawn->GetActorLocation()); Deprecated
		//AIC->StopMovement();
	}

	
}

void ABaseEnemy::OnNoiseHeard(APawn * NoiseInstigator, const FVector & Location, float Volume)
{
	if (GuardState == EAIState::Alerted) //in Tl because is Game over in Game mode
	{
		return;

	}


	if (NoiseInstigator == nullptr) {
		return;
	}

	
	if (DebugPawnSensingDrawing > 0)
	{

		DrawDebugSphere(GetWorld(), Location, 32.0f, 12, FColor::Green, false, 10.0f);

	}

	//look at noise origin

	FVector Direction = Location - GetActorLocation();
	Direction.Normalize();

	FRotator NewLookAt = FRotationMatrix::MakeFromX(Direction).Rotator();
	NewLookAt.Pitch = 0.0f;
	NewLookAt.Roll = 0.0f;

	SetActorRotation(NewLookAt);
	
	////Lo ponemo aqu� para que gire 
	//if (GuardState == EAIState::Alerted)
	//{
	//	return;

	//}


	SetGuardState(EAIState::Suspicious);

	GetWorldTimerManager().ClearTimer(TimerHandle_ResetOrientation);
	GetWorldTimerManager().SetTimer(TimerHandle_ResetOrientation, this, &ABaseEnemy::ResetOrientation, 3.0f);

	LastLookatKnowed = NewLookAt;
	 //SimpleMoveToActor(GetController(), CurrentPatrolPoint);

	/*AController* AIC = GetController();
	if (AIC)
	{	
	    AIC->StopMovement();
	}*/

}


void ABaseEnemy::ResetOrientation()
{
	if (GuardState == EAIState::Alerted)
	{
		 
		return;
	}

	SetActorRotation(OriginalRotation);

	SetGuardState(EAIState::Idle);

	// Stopped investigating...if we are a patrolling pawn, pick a new patrol point to move to
	/*if (bPatrol)
	{
		MoveToNextPatrolPoint();
	}*/
}


void ABaseEnemy::ResetAlerted()
{
	//S�lo se puede entrar en estado alerta
	if (GuardState != EAIState::Alerted)
	{

		return;
	}
	SetGuardState(EAIState::Suspicious);

	SetActorRotation(LastLookatKnowed);
}


void ABaseEnemy::SetGuardState(EAIState NewState)
{
	if (GuardState == NewState)
	{
		return;
	}

	GuardState = NewState;

	OnStateChanged(GuardState);

}





// Called every frame
void ABaseEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

   FVector StartTrace =	GetMesh()->GetForwardVector();



}




//
//void ABaseEnemy::CheckDamage(FName SocketInicio, FName SocketFinal, bool Reset)
//{
//	if (Reset) {
//
//	
//	//BaseEnemyMesh->GetActorLocation();
//	FVector inicio = BaseEnemyMesh->GetSocketLocation(SocketInicio);
//	FVector final  = BaseEnemyMesh->GetSocketLocation(SocketInicio);
//	ECollisionChannel inTraceChanel  = ECollisionChannel::ECC_Camera;
//	
//	//True to simplex collision
//	
//	bool binTraceComplexIN = false;
//	FCollisionQueryParams  F_CQParam = FCollisionQueryParams(binTraceComplexIN); // Hay m�s sobrecargas como actores a ignorar
//
//
//	
//	ECollisionResponse   E_CR = ECollisionResponse::ECR_Ignore;
//	//Structure that defines response container for the query
//	FCollisionResponseParams F_CRParam = FCollisionResponseParams(E_CR);//::DefaultResponseParam;
//	//Enum indicating how each type should respond
//	UWorld* World = GetWorld();
//	
//	GetWorld()->LineTraceSingleByChannel(Hit, inicio, final, inTraceChanel,F_CQParam, F_CRParam); 
//	if(World){
//		
//		if (Hit.bBlockingHit) {
//			// Cast the actor to BaseCharacter
//			ABaseCharacter* const HitBaseCharacter = Cast<ABaseCharacter>(Hit.Actor);
//			// If the cast is successful and the pickup is valid
//			if (HitBaseCharacter && !HitBaseCharacter->IsPendingKill()) {
//				
//				//Damage Base
//				float damage = BaseDamage;
//				//Get the Hero Armour
//				float armour = HitBaseCharacter->Armour;
//				//Get the Total Damage
//				if (armour > BaseDamage) damage = -1;
//				else damage = damage + armour; 
//				
//				//Handle Damage
//				HitBaseCharacter->CalculateHealth(damage);
//
//				//Only one Hit
//				Reset = false;
//
//				//The reset must be true with another start (or end�?) animations
//			}
//
//		}
//		
//
//	}
//
//	}
//}
