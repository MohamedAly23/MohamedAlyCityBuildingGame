#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "CameraPawn.h"
#include "Engine/Engine.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/SpringArmComponent.h"
#include "PlacementUIWidget.h"
#include "Kismet/GameplayStatics.h"

ACameraPawn::ACameraPawn()
{
    // Set this pawn to call Tick() every frame. You can turn this off to improve performance if you don't need it.
    PrimaryActorTick.bCanEverTick = true;

    SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
    SpringArm->SetupAttachment(RootComponent);
    SpringArm->TargetArmLength = SpringArmLength;
    SpringArm->bEnableCameraLag = true;
    SpringArm->CameraLagSpeed = CameraLagSpeed;
    SpringArm->SetRelativeRotation(RelativeRotation);

    Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
    Camera->SetupAttachment(SpringArm, USpringArmComponent::SocketName);
}

// Called when the game starts or when spawned
void ACameraPawn::BeginPlay()
{
    Super::BeginPlay();

    ensure(GetPlayerController());
    ensure(PlacementWidgetClass);

    PlacementWidget = CreateWidget<UPlacementUIWidget>(GetPlayerController(), PlacementWidgetClass);
    ensure(PlacementWidget);
    PlacementWidget->AddToViewport();

    if (DesertOfEgyptSound)
    {
        UGameplayStatics::PlaySound2D(GetWorld(), DesertOfEgyptSound);
    }
}

const void ACameraPawn::Zoom(float Value)
{
    if (!PlacementWidget || !PlacementWidget->IsPlacementWidgetVisible())
    {
        float DesiredArmLength = FMath::Clamp(SpringArm->TargetArmLength + Value, MinZoom, MaxZoom);

        // Smoothly interpolate to the desired arm length
        ensure(SpringArm);
        SpringArm->TargetArmLength = FMath::FInterpTo(SpringArm->TargetArmLength, DesiredArmLength, GetWorld()->GetDeltaSeconds(), InterpSpeed);
    }
}

void ACameraPawn::OnClickWheelButton()
{
    ensure(GetPlayerController());

    float MouseX, MouseY;
    GetPlayerController()->GetMousePosition(MouseX, MouseY);
    PreviousMousePosition = FVector2D(MouseX, MouseY);
}

APlayerController* ACameraPawn::GetPlayerController() const
{
    return Cast<APlayerController>(GetController());
}

const void ACameraPawn::Rotate(float Value, bool Action, float DeltaSpeed)
{
    if (Action)
    {
        ensure(GetPlayerController());

        float MouseX, MouseY;
        GetPlayerController()->GetMousePosition(MouseX, MouseY);
        CurrentMousePosition = FVector2D(MouseX, MouseY);

        FVector2D DeltaPosition = CurrentMousePosition - PreviousMousePosition;
        float Speed = DeltaPosition.X / DeltaSpeed;

        FRotator CurrentRotation = GetActorRotation();
        FRotator NewRotation = FRotator(CurrentRotation.Pitch, CurrentRotation.Yaw += (Value * Speed), CurrentRotation.Roll);
        SetActorRotation(NewRotation);

        PreviousMousePosition = CurrentMousePosition;
    }
}

const void ACameraPawn::Move(FVector Direction, float Speed, bool Forward)
{
    FVector NewDirection;
    FVector ForwardDirection = GetActorForwardVector() * Direction;
    FVector RightDirection = GetActorRightVector() * Direction;

    Speed = FMath::Clamp(Speed, LowSpeed, MaxSpeed);

    if (!Direction.IsZero() && Forward)
    {
        NewDirection = ForwardDirection;
    }
    else if (!Direction.IsZero() && !Forward)
    {
        NewDirection = RightDirection;
    }

    FVector NewLocation = GetActorLocation() + (NewDirection * DirectionMultiplier) * Speed;
    SetActorLocation(NewLocation);
}