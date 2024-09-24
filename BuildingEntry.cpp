#include "BuildingEntry.h"
#include "Building.h" 
#include "Components/Image.h"
#include "Components/Button.h"
#include "Components/TextBlock.h"
#include "Kismet/GameplayStatics.h"

void UBuildingEntry::NativeConstruct()
{
    Super::NativeConstruct();

    PlaceButton->OnClicked.AddDynamic(this, &UBuildingEntry::OnPlaceButtonClicked);
}

void UBuildingEntry::SetBuildingInfo(const FBuildingInfo BuildingInfo)
{
    Info = BuildingInfo;

    BuildingNameText->SetText(FText::FromString(Info.Name));

    UTexture2D* LoadedIcon = Info.Icon.LoadSynchronous();
    ensure(LoadedIcon);
    BuildingMeshImage->SetBrushFromTexture(LoadedIcon);
}

void UBuildingEntry::OnPlaceButtonClicked()
{
    ensure(Info.Class);
    if (UWorld* World = GetWorld())
    {
        UClass* LoadedClass = Info.Class.LoadSynchronous();
        ensure(LoadedClass);

        AActor* SpawnedActor = World->SpawnActor<ABuilding>(LoadedClass, FVector(0.f), FRotator::ZeroRotator);

        ABuilding* SpawnedBuilding = Cast<ABuilding>(SpawnedActor);

        ensure(SpawnedBuilding);
        if (SpawnedBuilding->CanPlacement())
        {
            SpawnedBuilding->StartPlacement();
            UGameplayStatics::PlaySoundAtLocation(this, AddBuildingClickSound, SpawnedBuilding->GetActorLocation());
        }
        else {
            UGameplayStatics::PlaySoundAtLocation(this, DeclinedAddBuildingSound, SpawnedBuilding->GetActorLocation());
        }
    }
}