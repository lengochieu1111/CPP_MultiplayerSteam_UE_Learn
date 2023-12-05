
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
	
private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Text_Score;

};
