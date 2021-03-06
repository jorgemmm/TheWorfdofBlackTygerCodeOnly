// Este obra está bajo una licencia de Creative Commons Reconocimiento 4.0 Internacional

#include "BTWeapon.h"
#include "Camera/CameraShake.h"
#include "Components/SkeletalMeshComponent.h"

#include "Engine.h"
#include "Engine/EngineTypes.h"
#include "Kismet/GameplayStatics.h"

#include "Particles/ParticleSystem.h"
#include "Particles/ParticleSystemComponent.h"
#include "PhysicalMaterials/PhysicalMaterial.h"

#include "GameFramework/DamageType.h"

#include "TimerManager.h"

#include "DrawDebugHelpers.h"



int32 DebugWeaponDrawing = 0;
FAutoConsoleVariableRef   CVARDebugWeaponDrawing(
	TEXT("INF.DebugWeapons"),
	DebugWeaponDrawing,
	TEXT("Draw Dbug Lines for weapons"),
	ECVF_Cheat);



// Sets default values
ABTWeapon::ABTWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	//PrimaryActorTick.bCanEverTick = true;

	Meshcomp = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("MeshComp"));
	RootComponent = Meshcomp;


	MuzzleSocketName = "MuzzleSocket";
	TracerTargetName = "Target";

	BaseDamage = 20.0f;
	RateOfFire = 600.0f;
	DispersionFactor = 0.5;
	Clip = 45;

	UE_LOG(LogTemp, Warning, TEXT("Weapon Initial Clip Load Up 45"));



}

// Called when the game starts or when spawned
void ABTWeapon::BeginPlay()
{
	Super::BeginPlay();

	TimeBetweenShots = (60 / RateOfFire);

	DispersionFactor = FMath::Clamp(DispersionFactor, 0.0f, 1.0f);
	DispersionDegree = FMath::Clamp(DispersionDegree, 0.0f, 90.0f);
	Dispersion = 180.0f - DispersionDegree;

	
	
}



/* Called every frame*/
//void ABTWeapon::Tick(float DeltaTime)
//{
//	Super::Tick(DeltaTime);
//
//}

void ABTWeapon::Fire()
{
	if (Clip <= 0)
	{
		//No hace falta Replicar esto
		//Sonido sin balas
		UGameplayStatics::PlaySound2D(this, NoBulletSound);
		return;
	}

	AActor* MyOwner = GetOwner();

	if (MyOwner != nullptr)
	{
		FVector EyeLocation;
		FRotator EyeRotation;

		MyOwner->GetActorEyesViewPoint(EyeLocation, EyeRotation);

		//FRotator SpawnRotation = EyeRotation;
		DispersionFactor /= 10;

		EyeRotation.Pitch += (FMath::FRandRange(-1, 1) * DispersionFactor) *  Dispersion;
		EyeRotation.Yaw += (FMath::FRandRange(-1, 1) *  DispersionFactor)  *  Dispersion;   //FMath::FRand() * 15.0f;
		EyeRotation.Roll += (FMath::FRandRange(-1, 1) * DispersionFactor)  *  Dispersion;   //FMath::FRand() *15.0f;


		FVector ShotDirection = EyeRotation.Vector();
		FVector TraceEnd = EyeLocation + (ShotDirection * 2000);

		FCollisionQueryParams QueryParams;

		QueryParams.AddIgnoredActor(MyOwner);
		QueryParams.AddIgnoredActor(this);
		QueryParams.bTraceComplex = true;

		QueryParams.bReturnPhysicalMaterial = true;
		EPhysicalSurface SurfaceType = SurfaceType_Default;

		//Particle "Target" Parameter
		FVector TracerEndPoint = TraceEnd;

		FHitResult Hit;
		if (GetWorld()->LineTraceSingleByChannel(Hit, EyeLocation, TraceEnd,
			ECC_Visibility, 
			//ECC_GameTraceChannel1,
			//COLLISION_MYWEAPONCHANNEL,  //Defining en CoopGame.h
			QueryParams))
		{
			AActor* HicActor = Hit.GetActor();
			float ActualDamage = BaseDamage;
			float HitpointHeigh = HicActor->GetActorLocation().Z;
			if (HitpointHeigh > 75.0f && HitpointHeigh < 200.f) ActualDamage *= 1.5;
			/*FName BoneHit = Hit.BoneName;			
			UE_LOG(LogTemp, Warning , TEXT("The bone name hitted is %s"), BoneHit);
			float comparefloat = BoneHit.Compare("M_F_head");*/
			//if ( FName (TEXT("head"), FNAME_Find) != NAME_None)
			/*if (FName(TEXT("head"), FNAME_Find) != BoneHit)
			{
				ActualDamage *= 1.5;
			}*/

			
			/**Aplicando dño en este proyecto mediante interfases 
			if(FireDamage){
				UGameplayStatics::ApplyPointDamage(HicActor, ActualDamage, ShotDirection, Hit,
					MyOwner->GetInstigatorController(),
					MyOwner,//this,
					FireDamage);
			}
			*/
			else UE_LOG(LogTemp, Error, TEXT("FireDamage is not Got it"));	

			

			TracerEndPoint = Hit.ImpactPoint;
			
			FVector MuzzleLoc = Meshcomp->GetSocketLocation(MuzzleSocketName);
			FVector MuzzleOffset = MuzzleLoc * 0.1*(1, 1, 0);
			MuzzleLoc += FTransform(EyeRotation).TransformVector(MuzzleOffset);

			
			// Transform MuzzleOffset from camera space to world space.
			//FVector MuzzleLoc = EyeLocation + FTransform(EyeRotation).TransformVector(MuzzleOffset);


			FRotator MuzzleRot = EyeRotation;
			MuzzleRot.Pitch += 10.0f;
			
			LaunchingBlade(MuzzleLoc, MuzzleRot);

		}		
		else 	UE_LOG(LogTemp, Error, TEXT("Linetrace is not Got it"));

		PlayFireEffects(Hit.ImpactPoint);

		

		Clip -= 1;

		if (DebugWeaponDrawing > 0)		
		{
			DrawDebugLine(GetWorld(), EyeLocation, TraceEnd, FColor::White, false, 1.0f, 0, 1.0f);
		}

		LastFireTime = GetWorld()->GetTimeSeconds();


	}

}

void ABTWeapon::Recoil()
{

}

void ABTWeapon::LaunchingBlade(FVector MuzzleLocation,FRotator MuzzleRotation)
{
	
	
	
	UWorld* World = GetWorld();
	if (World)
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		SpawnParams.Instigator = GetInstigator();
		// Spawn the projectile at the muzzle.
		AActor* Blade = World->SpawnActor<AActor>(BladeClass, MuzzleLocation, MuzzleRotation, SpawnParams);
		if (Blade)
		{
			// Set the projectile's initial trajectory.
			FVector LaunchDirection = MuzzleRotation.Vector();
			//Blade->FireInDirection(LaunchDirection);
		}
	}
}

void ABTWeapon::PlayFireEffects(FVector TracerEnd)
{
	if (FireSound != nullptr)  UGameplayStatics::PlaySound2D(this, FireSound);
	else UE_LOG(LogTemp, Error, TEXT("FireSound is null or none"));



	//Fuego en la bocacha
	if (MuzzleEffect != nullptr)   UGameplayStatics::SpawnEmitterAttached(MuzzleEffect, Meshcomp, MuzzleSocketName);
	else  UE_LOG(LogTemp, Error, TEXT("Muzzle Particle System is null or none"));

	//Trazo del disparo
	if (TracerEffect != nullptr)
	{
		FVector MuzzleLocation = Meshcomp->GetSocketLocation(MuzzleSocketName);
		UParticleSystemComponent* TracerComp = UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), TracerEffect, MuzzleLocation);

		if (TracerComp != nullptr)   TracerComp->SetVectorParameter(TracerTargetName, TracerEnd);
		else   UE_LOG(LogTemp, Error, TEXT("Tracer Particle System Not Spwaned"));
	}
	else  UE_LOG(LogTemp, Error, TEXT("Tracer Particle System is null or none"));


	APawn* MyOwner = Cast<APawn>(GetOwner());
	if (MyOwner != nullptr)
	{
		APlayerController* PC = Cast<APlayerController>(MyOwner->GetController());


		if (PC != nullptr)
		{
			if (FireCamShake) PC->ClientPlayCameraShake(FireCamShake);
			else UE_LOG(LogTemp, Error, TEXT("FireCamShake is null or none"));
		}
		else  UE_LOG(LogTemp, Error, TEXT("Player Controller is null or none"));

	}
	else   UE_LOG(LogTemp, Error, TEXT("Pawn Owner is null or none"));




}

int ABTWeapon::GetClip()
{
	return 0;
}

void ABTWeapon::Reload(int MaxAmmoLoadinClip)
{

}

void ABTWeapon::StartFire()
{
	float FirstDelay = FMath::Max(LastFireTime + TimeBetweenShots - GetWorld()->GetTimeSeconds(), 0.0f);
	//GetWorld()->GetTimeSeconds(), 		

	//in TL Course
	GetWorldTimerManager().SetTimer(TH_TimeBetweenShots, this, &ABTWeapon::Fire, TimeBetweenShots, true, FirstDelay); 

	//GetWorldTimerManager().SetTimer(TH_TimeBetweenShots, this, &ABTWeapon::Fire, TimeBetweenShots, true);

}

void ABTWeapon::StopFire()
{
	GetWorldTimerManager().ClearTimer(TH_TimeBetweenShots);
}

