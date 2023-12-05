

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
