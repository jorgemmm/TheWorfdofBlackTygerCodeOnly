// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "INFLands.h"
#include "GameFramework/Character.h"
#include "BaseCharacter.generated.h"


class UDamageType;

UCLASS(Blueprintable)
class INFLANDS_API ABaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
		class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI")
	UPawnNoiseEmitterComponent* NoiseEmitterComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseCharacter")
	bool bHeroIsFiring;
	
	


	//Change weapon
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	bool bWantsToChangeGun;
	//const bool bEquiped= true;

	//UPROPERTY()
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseCharacter")
	AActor* AxeWeapon;

	//Hay que crear clase primero  de Arma CC
	//
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player")
	TSubclassOf<class AActor> AxeWeaponClass;	
	

	//UPROPERTY()
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "BaseCharacter")		
	class ABTWeapon* GunWeapon;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player")
	TSubclassOf<class ABTWeapon> GunWeaponClass;

	

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BaseCharacter")
	FName WeaponSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BaseCharacter")
	FName GunSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BaseCharacter")
	FName UnequipWeaponSocketName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "BaseCharacter")
	FName UnequipGunSocketName;

	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	bool bWantsToZoom;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player")
	float ZoomedFOV;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Player", meta = (ClampMin = 0.1, ClampMax = 100))
	float InterpSpeed;

	/*Default  FOV set during begin play */
	float DefaultFOV;

	UFUNCTION()
	void StartZoom();

	UFUNCTION()
	void StopZoom();

	UFUNCTION()
	void Equip();

	UFUNCTION()
	void UnEquip();

	UFUNCTION()
	void ChangeWeapon(bool WantToChange);

	UFUNCTION(BlueprintCallable, Category = "Player")
	void StartFire();
	/*UFUNCTION(BlueprintCallable, Category = "Weapon")
	void EndFire();*/

	UFUNCTION(BlueprintCallable, Category = "Player")
	void StopFire();


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	virtual FVector GetPawnViewLocation() const override;
	
public:

	/**To show Snypercrooshair*/
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BaseCharacter")
		bool bIsZoomed;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "BaseCharacter")
    bool DisableInputs;

	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera)
	float BaseLookUpRate;


	//Estadísticas:
	//Expose a float property
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BaseCharacter")
		float Health = 100;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BaseCharacter")
		float Resistance = 100;

	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BaseCharacter")
		float Corrupt = 10;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BaseCharacter")
		float Madness = 10;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BaseCharacter")
		float Fear = 10;
	
	

	//ReadOnly Vbles
	//Expose a boolean property
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "BaseCharacter")
		bool isDead ;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BaseCharacter")
		bool bIsGunEquiped;
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BaseCharacter")
		bool bIsChainEquiped;

	//Expose a boolean property
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "BaseCharacter")
		int DeathCount = 0;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BaseCharacter")
		int  Coronas = 100;

	
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BaseCharacter")
		int  Blades;

	//Armour value
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BaseCharacter")
		float Armour = 0;
	//Valor de la recámara
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BaseCharacter")
		int  ClipBlades = 2;
	//Valor de la recámara
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "BaseCharacter")
		float  TimeReloadClip=0;

	


	//Expose a boolean property; If false Melé Weapon Equiped
	/*UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "BaseCharacter")
		bool isRangeWeaponReady = false;*/
	//Expose a boolean property; If false Melé Weapon Equiped
	/*UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "BaseCharacter")
		bool isMeleWeaponReady = false;*/



	//Step 4: Make a helper function, just because we are lazy
	virtual void CalculateDead();

	//Expose a Health method
	UFUNCTION(BlueprintCallable, Category = "BaseCharacter")
		virtual void CalculateHealth(float delta);
	
	UFUNCTION(BlueprintCallable, Category = "BaseCharacter")
		virtual void CalculateCorrupt(float delta);
	
	UFUNCTION(BlueprintCallable, Category = "BaseCharacter")
		virtual void CalculateMadness(float delta);
	
	UFUNCTION(BlueprintCallable, Category = "BaseCharacter")
		virtual void CalculateFear(float delta);

	
	/** Function to call when the Character is Damaged */
	/*UFUNCTION(BlueprintNativeEvent)
	void WasDamaged(float delta);
	virtual void WasDamaged_Implementation(float delta);*/


	//Expose a  Death Count method	
	UFUNCTION(BlueprintCallable, Category = "BaseCharacter")
	virtual void AddDeathCount();
    
	UFUNCTION(BlueprintCallable, Category = "BaseCharacter")
	virtual void ResetKills();

	UFUNCTION(BlueprintCallable, Category = "BaseCharacter")
	virtual void SetKills(int delta);
	
	UFUNCTION(BlueprintPure, Category = "BaseCharacter")
	virtual int GetKills();

	//Return true si se ha podido pagar o ingresar
	//False si no hay suficientes monedas para pagar
	//UFUNCTION(BlueprintCallable, Category = "BaseCharacter")
	//virtual  bool CalculeCoronas(int incoronas);
	UFUNCTION(BlueprintCallable, Category = "BaseCharacter")
	bool CalculeCoronas(int incoronas);
	
	UFUNCTION(BlueprintCallable, Category = "BaseCharacter")
	virtual void CalculeArmour(float inArmour);

	UFUNCTION(BlueprintCallable, Category = "BaseCharacter")
	virtual float CheckInDamage(float inArmour, float inDamage);


	UFUNCTION(BlueprintCallable, Category = "BaseCharacter")
	virtual void AddBlade(int inBlades);
	
	UFUNCTION(BlueprintCallable, Category = "BaseCharacter")
	virtual void ReloadClipBlade(int inMaxLoad);
	
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "BaseCharacter")
	FString  info = TEXT("");


	UFUNCTION(BlueprintCallable, Category = "BaseEnemy")
	virtual void CheckDamage(FName SocketInicio, FName SocketFinal, float inDamage , bool Reset);



	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="BaseEnemy")
	TSubclassOf<UDamageType> DamageType;


	//Step 6: Editor code to make updating values in the editor cleaner
#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif

	

protected:

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	/** Handler for when a touch input begins. */
	void TouchStarted(ETouchIndex::Type FingerIndex, FVector Location);

	/** Handler for when a touch input stops. */
	void TouchStopped(ETouchIndex::Type FingerIndex, FVector Location);


public:

	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
	

	/** Return whether or not the pickup is active */
	UFUNCTION(BlueprintPure, Category = "BaseCharacter")
		bool IsGunEquiped();

	/** Allows other classes to safely change whether or not pickup is active*/
	UFUNCTION(BlueprintCallable, Category = "BaseCharacter")
	void SetGunEquiped(bool WannaGun);


	


};
