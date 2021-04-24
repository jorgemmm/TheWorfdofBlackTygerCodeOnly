// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

//#include "CoreMinimal.h"
#include "INFLands.h"
#include "GameFramework/Actor.h"
#include "FloatingActor.generated.h"

UCLASS()
class INFLANDS_API AFloatingActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AFloatingActor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Items")
	float RunningTime;


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Items")
	int Cant=1;
	
	
};
