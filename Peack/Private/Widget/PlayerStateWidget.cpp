

#include "Widget/PlayerStateWidget.h"
#include "Components/TextBlock.h"

void UPlayerStateWidget::UpdateText_Score(float GivenScore)
{
	FString NewString = FString::Printf(TEXT("%d"), FMath::FloorToInt(GivenScore));
	if (this->Text_Score)
	{
		this->Text_Score->SetText(FText::FromString(NewString));
	}

}

void UPlayerStateWidget::UpdateText_Death(float GivenDeath)
{
	FString NewString = FString::Printf(TEXT("%d"), FMath::FloorToInt(GivenDeath));
	if (this->Text_Death)
	{
		this->Text_Death->SetText(FText::FromString(NewString));
	}
}

void UPlayerStateWidget::ToggleText_MVP(const bool bVisibility)
{
	if (this->Text_MVP)
	{
		if (bVisibility)
		{
			this->Text_MVP->SetVisibility(ESlateVisibility::Visible);
		}
		else
		{
			this->Text_MVP->SetVisibility(ESlateVisibility::Hidden);
		}
	}
}

