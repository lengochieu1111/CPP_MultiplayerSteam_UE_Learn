
#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "PlayerStateWidget.generated.h"

class UTextBlock;

UCLASS()
class PEACK_API UPlayerStateWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	void UpdateText_Score(float GivenScore);
	void UpdateText_Death(float GivenDeath);
	void UpdateText_Countdown(int TimeLeft);
	
private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_Score;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_Death;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_Countdown;

};
