

#include "Widget/CountdownWidget.h"
#include "Components/TextBlock.h"

void UCountdownWidget::UpdateText_Countdown(int TimeLeft)
{
	FString NewString = FString::Printf(TEXT("%d"), TimeLeft);
	if (this->Text_Countdown)
	{
		this->Text_Countdown->SetText(FText::FromString(NewString));
	}
}

