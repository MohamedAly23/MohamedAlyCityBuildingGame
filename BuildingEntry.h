#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BuildingInfo.h"
#include "BuildingEntry.generated.h"

class UImage;
class UButton;
class UTextBlock;
class UVerticalBox;

/**
 * User widget class for displaying building information and handling build actions.
 */
UCLASS(BlueprintType)
class CITYBUILDING_API UBuildingEntry : public UUserWidget
{
    GENERATED_BODY()

protected:
    virtual void NativeConstruct() override;

public:
    UFUNCTION(BlueprintCallable)
    void SetBuildingInfo(const FBuildingInfo BuildingInfo);

protected:
    UPROPERTY(meta = (BindWidget))
    UVerticalBox* NameBox;

    UPROPERTY(meta = (BindWidget))
    UTextBlock* BuildingNameText;

    UPROPERTY(meta = (BindWidget))
    UImage* BuildingMeshImage;

    UPROPERTY(meta = (BindWidget))
    UButton* PlaceButton;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    USoundBase* AddBuildingClickSound;

    UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sound")
    USoundBase* DeclinedAddBuildingSound;

private:

    FBuildingInfo Info;

    UFUNCTION()
    void OnPlaceButtonClicked();
};
