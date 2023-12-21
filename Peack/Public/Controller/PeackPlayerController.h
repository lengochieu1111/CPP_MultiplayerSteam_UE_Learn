
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PeackPlayerController.generated.h"

class UCharacterWidget;
class UPlayerStateWidget;
class APeackPlayerState;

UCLASS()
class PEACK_API APeackPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void CreateWidget_Character();
	void CreateWidget_PlayerState();
	void ReadyPlayerState(APeackPlayerState* GivenPlayerState);

	void UpdateBar_Health(float Health, float MaxHealth);
	void UpdateText_Score(float GivenScore);
	void UpdateText_Death(float GivenDeath);

private:
/*
* Character Widget
*/
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UUserWidget> WidgetClass_Character;

	UPROPERTY()
	TObjectPtr<UCharacterWidget> Widget_Character;

/*
* PlayerState Widget
*/
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UUserWidget> WidgetClass_PlayerState;

	UPROPERTY()
	TObjectPtr<UPlayerStateWidget> Widget_PlayerState;
	
};
