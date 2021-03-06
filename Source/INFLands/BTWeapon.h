// Este obra está bajo una licencia de Creative Commons Reconocimiento 4.0 Internacional

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BTWeapon.generated.h"

UCLASS()
class INFLANDS_API ABTWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABTWeapon();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	//CameraArmComponent
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	class USkeletalMeshComponent* Meshcomp;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName MuzzleSocketName;

	UPROPERTY(VisibleDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	FName TracerTargetName;

	/* EFuego en la bocacha*/
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	class UParticleSystem* MuzzleEffect;

	/**F* For Non Critical hit */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	class UParticleSystem* DefaultImpactEffect;


	/**For For Critical hit */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	class UParticleSystem* CriticalImpactEffect;//FleshImpactEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	class UParticleSystem* TracerEffect;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<class UCameraShake> FireCamShake;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<class AActor> BladeClass;

	UFUNCTION()
	void LaunchingBlade(FVector MuzzleLocation, FRotator MuzzleRotator);
	
	/**To setup normal damage or with poisson e.g.
	 En este proyecto se hace en BP medisante interfases */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	TSubclassOf<class UDamageType> FireDamage;


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly,Category = "Weapon")
	float BaseDamage;
	
	UFUNCTION()
	void Fire();

	//UFUNCTION(Server, Reliable, WithValidation)
	//void ServerFire();	

	//Bullets per minuts
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	float RateOfFire;

	float LastFireTime;

	float TimeBetweenShots;

	FTimerHandle TH_TimeBetweenShots;


	/**Fire Sound when shots  */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		USoundBase* FireSound;
	/**Weapon Sound whe Clip is empty  */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		USoundBase* NoBulletSound;
	/**Weapon Sound when is reloading  */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
		USoundBase* ReloadSound;

	//Challegue Dispersion

	/**Un valor rntre 0 y 90 grados--- a value from 0 to 90 degrees  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	float DispersionDegree;

	/**Un valor rntre 0 y 1--- a value from 0 to 1 1  */
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	float DispersionFactor;

	float Dispersion;

	//Challegue Recoil
	/** Play a FX Recoil when Fire the weapon */
	UFUNCTION()
	virtual void Recoil();

	//Play FX Fire Effect 
	UFUNCTION()
	void PlayFireEffects(FVector TracerEnd);

	//Play FX Fire Effect 
	//UFUNCTION()
	//void PlayImpactsEffects(EPhysicalSurface SurfaceType, FVector ImpactPoint);//, FRotator ImpactNormal);


	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Weapon")
	int Clip;



public:	
	/* Called every frame*/
	//virtual void Tick(float DeltaTime) override;


	//Ammo consumption
	UFUNCTION(BlueprintPure, Category = "Components")
		int GetClip();

	UFUNCTION(BlueprintCallable, Category = "Components")
		virtual void Reload(int MaxAmmoLoadinClip);
	//Handled the fire Weapon
	UFUNCTION(BlueprintCallable, Category = "Components")
		virtual void StartFire();

	UFUNCTION(BlueprintCallable, Category = "Components")
		virtual void StopFire();
	


	/** Return the mesh for the pickup */
	FORCEINLINE class USkeletalMeshComponent* GetMesh() const { return Meshcomp; }

};
