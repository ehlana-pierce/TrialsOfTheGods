// Fill out your copyright notice in the Description page of Project Settings.

//#pragma once
//
//#include "CoreMinimal.h"
//#include "GameFramework/Actor.h"
//#include "SplineConstrainedLightning.generated.h"
//
//UCLASS()
//class FMP1_API ASplineConstrainedLightning : public AActor
//{
//	GENERATED_BODY()
//	
//public:	
//	// Sets default values for this actor's properties
//	ASplineConstrainedLightning();
//
//protected:
//	// Called when the game starts or when spawned
//	virtual void BeginPlay() override;
//
//public:	
//	// Called every frame
//	virtual void Tick(float DeltaTime) override;
//
//};




#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SplineComponent.h"
#include "Components/StaticMeshComponent.h"
#include "SplineConstrainedLightning.generated.h"

UCLASS()
class FMP1_API ASplineConstrainedLightning : public AActor
{
    GENERATED_BODY()

public:
    ASplineConstrainedLightning();

protected:
    virtual void BeginPlay() override;

public:
    virtual void Tick(float DeltaTime) override;

private:
    /** Reference to the spline actor */
    UPROPERTY(EditAnywhere, Category = "Spline Movement")
    AActor* SplineActor;

    /** Cached spline component */
    USplineComponent* SplineComponent;

    /** Static Mesh to represent the lightning object */
    UPROPERTY(VisibleAnywhere, Category = "Mesh")
    UStaticMeshComponent* LightningMesh;

    /** Finds the closest point on the spline */
    FVector GetClosestPointOnSpline();
};
