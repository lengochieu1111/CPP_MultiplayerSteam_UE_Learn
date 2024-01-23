

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

void UPlayerStateWidget::UpdateText_Countdown(int TimeLeft)
{
	FString NewString = FString::Printf(TEXT("%d"), TimeLeft);
	if (this->Text_Countdown)
	{
		this->Text_Countdown->SetText(FText::FromString(NewString));
	}
}
