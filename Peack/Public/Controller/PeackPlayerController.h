
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PeackPlayerController.generated.h"

class UCharacterWidget;

UCLASS()
class PEACK_API APeackPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void CreateWidget_Character();

	void UpdateBar_Health(float Health, float MaxHealth);

private:
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UUserWidget> WidgetClass_Character;

	UPROPERTY()
	TObjectPtr<UCharacterWidget> Widget_Character;
	
};
