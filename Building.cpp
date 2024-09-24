#include "Building.h"
#include "GameFramework/PlayerController.h"
#include "Components/StaticMeshComponent.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"

bool ABuilding::bCanPlaceBuildings = true;

ABuilding::ABuilding()
{
    // Enable ticking for this actor
    PrimaryActorTick.bCanEverTick = true;
    SetActorTickEnabled(false);

    MeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComponent"));
    RootComponent = MeshComponent;

    CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
    CollisionComponent->SetupAttachment(RootComponent);

    CollisionComponent->SetGenerateOverlapEvents(true);
    CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
    CollisionComponent->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);

}

UStaticMeshComponent* ABuilding::GetMeshComponent() const
{
    return MeshComponent;
}

UMaterialInterface* ABuilding::GetClickedMaterial() const
{
    return ClickedMaterial;
}

void ABuilding::BeginPlay()
{
    Super::BeginPlay();

    ensure(CollisionComponent);

    CollisionComponent->OnComponentBeginOverlap.AddDynamic(this, &ABuilding::OnOverlapBegin);
    CollisionComponent->OnComponentEndOverlap.AddDynamic(this, &ABuilding::OnOverlapEnd);
    CollisionComponent->OnClicked.AddDynamic(this, &ABuilding::HandleClick);

}

void ABuilding::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    if (!bIsPlaced)
    {
        MoveBuilding();
    }
}

void ABuilding::HandleClick(UPrimitiveComponent* ClickedComponent, FKey ButtonPressed)
{
    if (bCanPlaceBuildings)
    {
        ensure(GetMeshComponent());
        if (bIsPlaced)
        {
            SetActorTickEnabled(true);
            bCanPlaceBuildings = false;
            PreviousMaterial = GetMeshComponent()->GetMaterial(0);

            GetMeshComponent()->SetMaterial(0, GetClickedMaterial());
            bIsPlaced = false;
        }
    }
    else if (!bIsPlaced)
    {
        UMaterialInterface* CurrentMaterial = GetMeshComponent()->GetMaterial(0);

        if (CurrentMaterial == GetClickedMaterial())
        {
            SetActorTickEnabled(false);

            GetMeshComponent()->SetMaterial(0, PreviousMaterial);
            bIsPlaced = true;
            bCanPlaceBuildings = true;
        }
        else if (CurrentMaterial == GetOverlapMaterial)
        {
            UGameplayStatics::PlaySoundAtLocation(this, PlacementDeclinedSound, GetActorLocation());
        }
    }
}

void ABuilding::StartPlacement()
{
    MoveBuilding();
    bIsPlaced = false;
    bCanPlaceBuildings = false;
    GetMeshComponent()->SetMaterial(0, GetClickedMaterial());
}

bool ABuilding::CanPlacement()
{
   return bCanPlaceBuildings;
}

void ABuilding::MoveBuilding()
{
    const APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
    ensure(PlayerController);

    float MouseX, MouseY;
    PlayerController->GetMousePosition(MouseX, MouseY);

    FVector WorldLocation, WorldDirection;
    if (PlayerController->DeprojectScreenPositionToWorld(MouseX, MouseY, WorldLocation, WorldDirection))
    {
        FVector EndPoint = WorldLocation + (WorldDirection * TraceRange);

        FHitResult HitResult;
        FCollisionQueryParams QueryParams;
        // Perform a line trace to find the target location
        GetWorld()->LineTraceSingleByChannel(HitResult, WorldLocation, EndPoint, ECC_Visibility, QueryParams);

        if (HitResult.IsValidBlockingHit())
        {
            // Set the target location based on the hit point
            FVector TargetLocation = HitResult.ImpactPoint;

            // Smoothly move the actor to the target location
            const FVector CurrentLocation = GetActorLocation();
            TargetLocation.Z = CurrentLocation.Z;
            const FVector SmoothedLocation = FMath::Lerp(CurrentLocation, TargetLocation, LerpLocationValue);

            SetActorLocation(SmoothedLocation);
        }
    }

}
int32 OverlapCount = 0;  // Initialize the overlap count

void ABuilding::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
    bool bFromSweep, const FHitResult& SweepResult)
{
    if (OtherActor && OtherActor != this && !bIsPlaced)
    {
        OverlapCount++;
        GetMeshComponent()->SetMaterial(0, GetOverlapMaterial);
    }
}

void ABuilding::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
    UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
    if (OtherActor && OtherActor != this && !bIsPlaced)
    {
        OverlapCount--;
        if (OverlapCount <= 0)
        {
            GetMeshComponent()->SetMaterial(0, GetClickedMaterial());
        }
    }
}
