// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseCharacter.h"
#include "BTWeapon.h"

#include "Engine/World.h"
#include "Engine.h"
#include "Engine/EngineTypes.h"

#include "Components/SkeletalMeshComponent.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "Engine/World.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"



// Sets default values
ABaseCharacter::ABaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;


	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 540.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);// AttachTo(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

	//  AttachTo(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm


	NoiseEmitterComponent = CreateDefaultSubobject<UPawnNoiseEmitterComponent>(TEXT("NoiseEmitter"));

	DisableInputs = false;


	WeaponSocketName ="GunSocket";
		

	GunSocketName = "hand_rSocket";

	
	
	UnequipWeaponSocketName = "unequip_so_axe";

	UnequipGunSocketName = "unequip_so";


	   	 

	Blades = 10;
	UE_LOG(LogTemp, Warning, TEXT("Hero Initial Ammo Load Up %i"), Blades );

	//Zoom
	ZoomedFOV = 65.0f;

	InterpSpeed = 20.0f;

	bWantsToChangeGun = false;

	
}

// Called when the game starts or when spawned
void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();

	isDead = false;
	bIsGunEquiped = true;
	bIsChainEquiped = false;

	

	DefaultFOV = FollowCamera->FieldOfView;

	//Código de seguridad
	//bWantsToChangeGun = bIsGunEquiped;
	//bIsChainEquiped = !bIsGunEquiped;


	/*if(GunWeaponClass)
	{
		//Spwaneamos solo si podemos destruir el anterior
		GunWeapon = GetWorld()->SpawnActor<ABTWeapon>(GunWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

		if (GunWeapon) 
		{
			//Adjuntamos la ballesta
			GunWeapon->SetOwner(this);

			GunWeapon->AttachToComponent(GetMesh(),
				FAttachmentTransformRules::SnapToTargetNotIncludingScale,
				GunSocketName);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("No se pudo adjuntar actor: %S"), *GunWeapon->GetName());
		}
	}
	 //or
	 //ChangeWeapon(bIsGunEquiped);
	
	*/
	
	

	

	
}



// Called every frame
void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	bIsZoomed = bWantsToZoom;

	float TargetFOV = bWantsToZoom ? ZoomedFOV : DefaultFOV;

	float NewFOV = FMath::FInterpTo(FollowCamera->FieldOfView, TargetFOV, DeltaTime, InterpSpeed);

	FollowCamera->SetFieldOfView(NewFOV);

	

	//bWantsToChangeGun = bIsGunEquiped;
	


	

}

void ABaseCharacter::StartZoom()
{
	bWantsToZoom = true;
}

void ABaseCharacter::StopZoom()
{
	bWantsToZoom = false;
}



FVector ABaseCharacter::GetPawnViewLocation() const
{

	if (FollowCamera)
	{
		return FollowCamera->GetComponentLocation();
	}

	return Super::GetPawnViewLocation();

}

/** In BP*/
void ABaseCharacter::Equip()
{
	//ChangeWeapon(bWantsToChangeGun);
	//
}
/** In BP*/
void ABaseCharacter::UnEquip()
{
	//Invertimos
	/*if (bWantsToChangeGun) bWantsToChangeGun = false;  
	else bWantsToChangeGun = true;

	if (bIsGunEquiped) bIsGunEquiped = false;
	else bIsGunEquiped = true;
	
	UE_LOG(LogTemp, Warning, TEXT("Valor de bWantsToChangeGun: %s"), bWantsToChangeGun ? TEXT("True") : TEXT("False"));*/

	
}

/** In BP*/
void ABaseCharacter::ChangeWeapon(bool WantToChange)
{
	//A nivel de clase
	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	//SpawnParams.Owner = this;

	//Spwaneamos solo si podemos destruir el anterior


	
	
	
	//Una vez cargados las armas	
	if(WantToChange)
	{
		if (GunWeaponClass)
		{
			GunWeapon = GetWorld()->SpawnActor<ABTWeapon>(GunWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
			//Adjuntamos la ballesta
			GunWeapon->SetOwner(this);

			GunWeapon->AttachToComponent(GetMesh(),
				FAttachmentTransformRules::SnapToTargetNotIncludingScale,
				GunSocketName
			);

			UE_LOG(LogTemp, Warning, TEXT("Arma Addjuntada %s"), *GunWeapon->GetName());
		}else UE_LOG(LogTemp, Error, TEXT("No cargó  GunWeaponClass"));

			
		
		if (AxeWeaponClass) {

			AxeWeapon = GetWorld()->SpawnActor<ABTWeapon>(AxeWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);
			
			AxeWeapon->SetOwner(this);
			AxeWeapon->AttachToComponent(GetMesh(),
				FAttachmentTransformRules::SnapToTargetNotIncludingScale,
				UnequipWeaponSocketName
			);


			//UE_LOG(LogTemp, Error, TEXT("Arma Desquipada %s"), *GunWeapon->GetName());
		}
		else UE_LOG(LogTemp, Error, TEXT("No cargó  AxeWeaponClass"));

		  
		
	
	           

	

	}
	else
	{
		//Hay que crear clase primero  de Arma CC
		if (AxeWeaponClass) 
		{

			AxeWeapon = GetWorld()->SpawnActor<ABTWeapon>(AxeWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

			//Adjuntamos el hacha
			AxeWeapon->SetOwner(this);
			AxeWeapon->AttachToComponent(GetMesh(),
				FAttachmentTransformRules::SnapToTargetNotIncludingScale,
				WeaponSocketName
			);
		}
		else UE_LOG(LogTemp, Error, TEXT("No cargó  AxeWeaponClass"));

		if (GunWeaponClass)
		{
			GunWeapon = GetWorld()->SpawnActor<ABTWeapon>(GunWeaponClass, FVector::ZeroVector, FRotator::ZeroRotator, SpawnParams);

			//Desequipamos la ballesta
			GunWeapon->SetOwner(this);
			GunWeapon->AttachToComponent(GetMesh(),
				FAttachmentTransformRules::SnapToTargetNotIncludingScale,
				UnequipGunSocketName
			);
		}

	}

}


void ABaseCharacter::StartFire()
{


	if (GunWeapon != nullptr)
	{
		/*const float ForceAmount = 20000.0f;
		GetMesh()->AddForce(FVector(0.0f, 0.0f, ForceAmount));*/

		GunWeapon->StartFire();

	}

}

void ABaseCharacter::StopFire()
{
	if (GunWeapon != nullptr)
	{
		GunWeapon->StopFire();
	}
}

// Called to bind functionality to input
void ABaseCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	//Super::SetupPlayerInputComponent(PlayerInputComponent);

	check(InputComponent);
	InputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	InputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);



	InputComponent->BindAxis("MoveForward", this, &ABaseCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &ABaseCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	
	
	//
	InputComponent->BindAxis("TurnRate", this, &ABaseCharacter::TurnAtRate);
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);

	InputComponent->BindAxis("LookUpRate", this, &ABaseCharacter::LookUpAtRate);	
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	
	//Zoom
	InputComponent->BindAction("Zoom", IE_Pressed, this, &ABaseCharacter::StartZoom);
	InputComponent->BindAction("Zoom", IE_Released , this, &ABaseCharacter::StopZoom);	

	// handle touch devices
	InputComponent->BindTouch(IE_Pressed, this, &ABaseCharacter::TouchStarted);
	InputComponent->BindTouch(IE_Released, this, &ABaseCharacter::TouchStopped);

	/**In BP*/
	//Change Weapon
	/*InputComponent->BindAction("EquipWeapon", IE_Pressed, this, &ABaseCharacter::Equip);
	InputComponent->BindAction("EquipWeapon", IE_Released, this, &ABaseCharacter::UnEquip);

	//Fire
	InputComponent->BindAction("Fire", IE_Pressed, this, &ABaseCharacter::StartFire);
	InputComponent->BindAction("Fire", IE_Released, this, &ABaseCharacter::StopFire);*/

}



void ABaseCharacter::CalculateDead()
{
	if ((Health <= 0)||(Corrupt>=100))
		isDead = true;
	else
		isDead = false;

}



//To increase Health or to Damage if delta is negative
void ABaseCharacter::CalculateHealth(float delta)
{
	if (Armour <= 0) {
		Health += delta;
	}
	else
	{
		
		Health = Health + delta / 8;
		CalculeArmour(delta/4);
	}

	
	if(Health>=75)Resistance = Health;
	else {
		if (Health >= 50) Resistance = Health / 2;
		else Resistance = Health / 4;
		
	}
	Health = FMath::Clamp(Health, 0.0f, 100.0f);
	Resistance = FMath::Clamp(Resistance, 0.0f, 100.0f);
	CalculateDead();

}


void ABaseCharacter::CalculateCorrupt(float delta)
{
	Corrupt += delta;

	Corrupt = FMath::Clamp(Corrupt, 0.0f, 100.0f);
	CalculateDead();
}



void ABaseCharacter::CalculateMadness(float delta)
{
	Madness += delta;
	Madness = FMath::Clamp(Madness,0.0f, 100.0f);
	CalculateDead();
}



void ABaseCharacter::CalculateFear(float delta)
{
	
	Fear += delta;
	//La Locura disminuye el miedo
	if (Madness > 40) Fear = Fear - Madness / 5;
	/*else
	{
		Fear = Fear - Madness / 100;
	}*/

	Fear = FMath::Clamp(Fear, 0.0f, 100.0f);
	CalculateDead();
}



//Killing Counts handler
void ABaseCharacter::AddDeathCount()
{
	DeathCount+= 1;
}


void ABaseCharacter::ResetKills()
{
	DeathCount = 0;
}



void ABaseCharacter::SetKills(int delta)
{
	DeathCount+= delta;
}



int ABaseCharacter::GetKills()
{
	return DeathCount;
}


bool ABaseCharacter::CalculeCoronas(int incoronas)
{

	//Return true si se ha podido pagar o ingresar
	//False si no hay suficientes monedas para pagar

	

	int CurrenCoronas = Coronas;
	CurrenCoronas += incoronas;
	if (CurrenCoronas < 0) {
		UE_LOG(LogTemp, Warning, TEXT("No hay suficientes coronas"));
		return false;
	}
	else {
		Coronas = CurrenCoronas;
		UE_LOG(LogTemp, Warning, TEXT("Transacción correcta"));
		return true;
	}

	/*
	if (incoronas > 0) 
	{
		//Se trat de un ingreso
		Coronas += incoronas;
		return true;

	}
	else {
		//Se trata de un pago

		if (incoronas <= Coronas) {
			Coronas += incoronas;

			//Por seguridad Mantenemos el valor por encima de 0.
			if (Coronas <= 0) {
				Coronas = 0;
			}
			return true;

		}
		else {
			//printf(TEXT("No hay suficientes coronas"));
			info = TEXT("No hay suficientes coronas");
			return false;
		}
		
		
	}
	*/
	
	
	
}



void ABaseCharacter::CalculeArmour(float inArmour)
{
	Armour += inArmour;
	//Armour = inArmour;
	Armour = FMath::Clamp(Armour, 0.0f, 200.0f);

	//if crash do it like Calculecoronas

	//if work it out do calculecoronas like this
}

float ABaseCharacter::CheckInDamage(float inArmour, float inDamage)
{
	//INdAMAGE DEBE SER NEGATIVO
	if(inDamage >=0)
	{
		inDamage = inDamage * (-1);
	}
	
	if ((inArmour + inDamage) >= 0.0f) {

		CalculeArmour(inDamage / 2);
		return -1.0f;
	}
	else {

		CalculeArmour(inDamage / 2);
		return (inArmour + inDamage);
	}

}



void ABaseCharacter::AddBlade(int inBlades)
{
	Blades += inBlades;

	if (Blades <= 0) {
		Blades = 0;
	}

}



void ABaseCharacter::ReloadClipBlade(int inMaxLoad)
{
	//inBlades es la munción total que queda	
	if(ClipBlades<inMaxLoad)
	{
		if (Blades >= inMaxLoad)
			{
				//InMaxload es la carga máxima de la recámara				
				for (size_t i = ClipBlades; i < inMaxLoad; i++) 
				{
					ClipBlades += 1;
					AddBlade(-1);
				}
			}
		else 
			{
				//Inblades Blades < inMaxLoad				
				for (size_t i = ClipBlades; i < Blades; i++) {
					ClipBlades += 1;
					AddBlade(-1);
				}
				
			}	
	
	}

	if (ClipBlades <= 0) {
		ClipBlades = 0;
	}
}





void ABaseCharacter::CheckDamage(FName SocketInicio, FName SocketFinal,float inDamage ,bool Reset)
{
	//if (Reset) {
		//AActor* MyOwner = GetOwner();
		USkeletalMeshComponent*	Mesh =  GetMesh();
		//if (MyOwner) 
		if(Mesh)
		{
			//Get the mesh
			
			FVector inicio = Mesh->GetSocketLocation(SocketInicio);
			FVector final = Mesh->GetSocketLocation(SocketInicio);

			//or ShotDirection
			FVector DamageDirection = final.ForwardVector;

			FCollisionQueryParams QueryParams;
			//QueryParams.AddIgnoredActor(MyOwner);

			QueryParams.AddIgnoredActor(this);
			QueryParams.bTraceComplex = true;

			FHitResult Hit;

			if (GetWorld()->LineTraceSingleByChannel(Hit, inicio, final, ECC_Visibility, QueryParams)) {
				
				float BaseDamage;
				if (inDamage <= 0)  BaseDamage = 20.0;
				else BaseDamage = inDamage;
				//Blocking hit, Proces damage
				//AActor* HitActor = Hit.GetActor();
				//UGameplayStatics::ApplyPointDamage(HitActor, inDamage, DamageDirection, Hit, MyOwner->GetInstigatorController(), this, DamageType);
				//UGameplayStatics::ApplyPointDamage(HitActor, inDamage, DamageDirection, Hit, GetInstigatorController(), this, DamageType);
			    
				// Cast the actor to BaseCharacter
				ABaseCharacter* const HitBaseCharacter = Cast<ABaseCharacter>(Hit.Actor);
				if (HitBaseCharacter && !HitBaseCharacter->IsPendingKill()) {

					//Damage Base
					float damage = BaseDamage;
					//Get the Hero Armour
					float armour = HitBaseCharacter->Armour;
					//Get the Total Damage
					if (armour > BaseDamage) damage = -1;
					else damage = damage + armour;

					//Handle Damage
					HitBaseCharacter->CalculateHealth(damage);

					//Only one Hit
					Reset = false;

					//The reset must be true with another start (or end¿?) animations
				}

			}
			/*else {
				DrawDebugLine(GetWorld(), inicio, final, FColor::Red, false, 1.0f, 1.0f);


			}*/
			DrawDebugLine(GetWorld(), inicio, final, FColor::White, false, 1.0f, 1.0f);

			
		//}
	}
	
	

}




#if WITH_EDITOR
void ABaseCharacter::PostEditChangeProperty(FPropertyChangedEvent & PropertyChangedEvent)
{
	isDead = false;
	Health = 100;
	Super::PostEditChangeProperty(PropertyChangedEvent);
	CalculateDead();
}

#endif

//Movement Functions

void ABaseCharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f)	&&  (!DisableInputs))
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}


void ABaseCharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f)     
		
		&& (!DisableInputs)
		)
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
void ABaseCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
     if (!DisableInputs)  AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}
void ABaseCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	if (!DisableInputs) AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}
void ABaseCharacter::TouchStarted(ETouchIndex::Type FingerIndex, FVector Location)
{
	// jump, but only on the first touch
	if (FingerIndex == ETouchIndex::Touch1)
	{
		bHeroIsFiring = true;
		//Jump();
	}
}
void ABaseCharacter::TouchStopped(ETouchIndex::Type FingerIndex, FVector Location)
{
	if (FingerIndex == ETouchIndex::Touch1)
	{
		bHeroIsFiring = false;
		//StopJumping();
	}
}

bool ABaseCharacter::IsGunEquiped()
{
	return bIsGunEquiped;
}

void ABaseCharacter::SetGunEquiped(bool WannaGun)
{
	//bIsGunEquiped=true;
}



