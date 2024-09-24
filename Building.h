#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Building.generated.h"

class UBoxComponent;
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnBuildingMessage, const FString&, Message, const FLinearColor&, MessageColor);

UCLASS()
class CITYBUILDING_API ABuilding : public AActor
{
    GENERATED_BODY()

public:
    ABuilding();

    UFUNCTION()
    void StartPlacement();

    UFUNCTION()
    bool CanPlacement();

protected:

    UStaticMeshComponent* GetMeshComponent() const;

    UMaterialInterface* GetClickedMaterial() const;

    UPROPERTY(EditAnywhere)
    UMaterialInterface* GetOverlapMaterial;

    UPROPERTY(VisibleAnywhere)
    UBoxComponent* CollisionComponent;

    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    USoundBase* PlacementDeclinedSound;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    float TraceRange = 10000.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    float LerpLocationValue = 0.1f;

public:
    // Called every frame
    virtual void Tick(float DeltaTime) override;

private:
    UPROPERTY()
    UMaterialInterface* PreviousMaterial;

    bool bIsPlaced = true;

    UFUNCTION(BlueprintCallable, Category = "Interaction")
    void HandleClick(UPrimitiveComponent* ClickedComponent, FKey ButtonPressed);

    UFUNCTION()
    void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
        bool bFromSweep, const FHitResult& SweepResult);

    UFUNCTION()
    void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor,
        UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

    UPROPERTY(VisibleAnywhere)
    UStaticMeshComponent* MeshComponent;

    UPROPERTY(EditAnywhere)
    UMaterialInterface* ClickedMaterial;

    UFUNCTION()
    void MoveBuilding();

    static bool bCanPlaceBuildings; // replace click with Placement
};