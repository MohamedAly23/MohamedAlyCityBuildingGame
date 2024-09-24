#include "PlacementUIWidget.h"
#include "Components/Button.h"
#include "Components/ScrollBox.h"
#include "BuildingEntry.h"
#include "Kismet/GameplayStatics.h"

void UPlacementUIWidget::NativeOnInitialized()
{
    Super::NativeOnInitialized();

    ToggleVisibilityButton->OnClicked.AddDynamic(this, &UPlacementUIWidget::OnToggleVisibilityButtonClicked);
    HideButton->OnClicked.AddDynamic(this, &UPlacementUIWidget::OnHideButtonClicked);

    InitBuildingEntries();

    HideButton->SetVisibility(ESlateVisibility::Collapsed);
    PlacementScrollBox->SetVisibility(ESlateVisibility::Collapsed);
}

void UPlacementUIWidget::InitBuildingEntries()
{
    if (BuildingInfoDataTable && BuildingEntryClass && PlacementScrollBox)
    {
        const TMap<FName, uint8*>& RowMap = BuildingInfoDataTable->GetRowMap();
        for (const TPair<FName, uint8*>& RowPair : RowMap)
        {
            const FName& RowName = RowPair.Key;

            // Create and add a new BuildingEntry widget
            UBuildingEntry* BuildingEntry = CreateWidget<UBuildingEntry>(GetWorld(), BuildingEntryClass);
            if (ensure(BuildingEntry))
            {
                FBuildingInfo* RowData = reinterpret_cast<FBuildingInfo*>(RowPair.Value);
                if (ensure(RowData))
                {
                    BuildingEntry->SetBuildingInfo(*RowData);
                    PlacementScrollBox->AddChild(BuildingEntry);
                }
            }
        }
    }
}

bool UPlacementUIWidget::IsPlacementWidgetVisible()
{
    if (PlacementScrollBox)
    {
        return PlacementScrollBox->IsVisible();
    }
    return false;
}

void UPlacementUIWidget::OnToggleVisibilityButtonClicked()
{
    // Toggle the visibility of the scroll box and hide button
    const ESlateVisibility NewVisibility = IsPlacementWidgetVisible() ? ESlateVisibility::Collapsed : ESlateVisibility::Visible;
    PlacementScrollBox->SetVisibility(NewVisibility);
    HideButton->SetVisibility(NewVisibility);

    UGameplayStatics::PlaySound2D(this, OpenWidgetClickSound);

}

void UPlacementUIWidget::OnHideButtonClicked()
{
    PlacementScrollBox->SetVisibility(ESlateVisibility::Collapsed);
    HideButton->SetVisibility(ESlateVisibility::Collapsed);

    UGameplayStatics::PlaySound2D(this, OpenWidgetClickSound);
}


