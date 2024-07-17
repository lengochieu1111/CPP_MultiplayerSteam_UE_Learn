
#pragma once

#include "CoreMinimal.h"
#include "Widget/CountdownWidget.h"
#include "PlayerStateWidget.generated.h"

class UTextBlock;

UCLASS()
class PEACK_API UPlayerStateWidget : public UCountdownWidget
{
	GENERATED_BODY()

public:
	void UpdateText_Score(float GivenScore);
	void UpdateText_Death(float GivenDeath);
	void ToggleText_MVP(const bool bVisibility);
	
private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_Score;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_Death;

	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_MVP;

};
