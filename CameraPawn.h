#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "CameraPawn.generated.h"

UCLASS()
class CITYBUILDING_API ACameraPawn : public APawn
{
    GENERATED_BODY()

public:
    ACameraPawn();

protected:
    // Called when the game starts or when spawned
    virtual void BeginPlay() override;

public:

    UFUNCTION(BlueprintCallable, Category = "Movement")
    const void Move(FVector Value, float Speed, bool Forward);

    UFUNCTION(BlueprintCallable, Category = "Movement")
    const void Zoom(float Value);

    UFUNCTION(BlueprintCallable, Category = "Movement")
    const void Rotate(float Value, bool Action, float DeltaSpeed);

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    class USpringArmComponent* SpringArm;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    class UCameraComponent* Camera;

    UFUNCTION(BlueprintCallable, Category = "Movement")
    void OnClickWheelButton();

    UPROPERTY(EditAnywhere)
    TSubclassOf<class UPlacementUIWidget> PlacementWidgetClass;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    float SpringArmLength = 600.f;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    float CameraLagSpeed = 3.0f;
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
    FRotator RelativeRotation = FRotator(-50.f, 180.f, 0.f);

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    float MaxSpeed = 2.5f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    float LowSpeed = 1.0f;

    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Movement")
    float DirectionMultiplier = 10.0f;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zoom")
    float MinZoom = 200;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zoom")
    float MaxZoom = 800;

    UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Zoom")
    float InterpSpeed = 10.f;

    UPROPERTY(EditAnywhere)
    USoundBase* DesertOfEgyptSound;
private:
    FVector2D PreviousMousePosition;

    FVector2D CurrentMousePosition;

    APlayerController* GetPlayerController() const;

    UPROPERTY()
    UPlacementUIWidget* PlacementWidget;
};
