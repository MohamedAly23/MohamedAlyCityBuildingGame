#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "PlacementUIWidget.generated.h"

/**
 * UPlacementUIWidget class for managing the placement UI and its interactions.
 */
UCLASS()
class CITYBUILDING_API UPlacementUIWidget : public UUserWidget
{
    GENERATED_BODY()
public:
    bool IsPlacementWidgetVisible();

protected:
    virtual void NativeOnInitialized() override;

    UPROPERTY(meta = (BindWidget))
    class UScrollBox* PlacementScrollBox;

    UPROPERTY(meta = (BindWidget))
    UButton* HideButton;

    UPROPERTY(EditAnywhere)
    UDataTable* BuildingInfoDataTable;

    UPROPERTY(EditAnywhere)
    TSubclassOf<class UBuildingEntry> BuildingEntryClass;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    USoundBase* OpenWidgetClickSound;

private:

    UPROPERTY(meta = (BindWidget))
    UButton* ToggleVisibilityButton;

    UFUNCTION()
    void OnHideButtonClicked();

    UFUNCTION()
    void OnToggleVisibilityButtonClicked();

    void InitBuildingEntries();
};
