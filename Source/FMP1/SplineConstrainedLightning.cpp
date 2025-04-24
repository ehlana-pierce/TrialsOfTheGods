#include "SplineConstrainedLightning.h"
#include "Components/SplineComponent.h"
#include "Components/StaticMeshComponent.h"
#include "UObject/ConstructorHelpers.h"

ASplineConstrainedLightning::ASplineConstrainedLightning()
{
    PrimaryActorTick.bCanEverTick = true;

    // Create the Static Mesh Component
    LightningMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LightningMesh"));
    RootComponent = LightningMesh; // Make it the root of the actor

    // Load a default mesh (Optional - replace with your custom asset)
    static ConstructorHelpers::FObjectFinder<UStaticMesh> MeshAsset(TEXT("/Engine/BasicShapes/Sphere.Sphere"));
    if (MeshAsset.Succeeded())
    {
        LightningMesh->SetStaticMesh(MeshAsset.Object);
    }
}

void ASplineConstrainedLightning::BeginPlay()
{
    Super::BeginPlay();

    // Get the Spline Component from the assigned SplineActor
    if (SplineActor)
    {
        SplineComponent = Cast<USplineComponent>(SplineActor->GetComponentByClass(USplineComponent::StaticClass()));
    }
}

void ASplineConstrainedLightning::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (SplineComponent)
    {
        // Find closest point on the spline (without forcing movement)
        FVector ActorLocation = GetActorLocation();
        float Distance = SplineComponent->FindInputKeyClosestToWorldLocation(ActorLocation);
        
        // Get the correct X/Y position from the spline but keep the actor's own Z
        FVector SplinePosition = SplineComponent->GetLocationAtSplineInputKey(Distance, ESplineCoordinateSpace::World);
        FVector CorrectedPosition = FVector(SplinePosition.X, SplinePosition.Y, ActorLocation.Z);

        // Gently nudge towards the spline position (without overriding player input)
        SetActorLocation(FMath::VInterpTo(ActorLocation, CorrectedPosition, DeltaTime, 1.0f));

        // Get forward direction along the spline
        FVector Tangent = SplineComponent->GetTangentAtSplineInputKey(Distance, ESplineCoordinateSpace::World);
        FRotator TargetRotation = FRotationMatrix::MakeFromXZ(Tangent, FVector::UpVector).Rotator();

        // Smoothly rotate towards the correct direction while keeping control
        FRotator SmoothedRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, 1.0f);
        SetActorRotation(SmoothedRotation);
    }
}

FVector ASplineConstrainedLightning::GetClosestPointOnSpline()
{
    if (!SplineComponent) return GetActorLocation();

    // Find the closest location on the spline
    FVector ActorLocation = GetActorLocation();
    float Distance = SplineComponent->FindInputKeyClosestToWorldLocation(ActorLocation);
    return SplineComponent->GetLocationAtSplineInputKey(Distance, ESplineCoordinateSpace::World);
}
