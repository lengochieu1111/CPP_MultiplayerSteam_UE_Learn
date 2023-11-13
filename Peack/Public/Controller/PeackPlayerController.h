
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PeackPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class PEACK_API APeackPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void CreateWidget_Character();

private:
	UPROPERTY(EditDefaultsOnly, Category = "Widget")
	TSubclassOf<UUserWidget> WidgetClass_Character;

	UPROPERTY()
	TObjectPtr<UUserWidget> Widget_Character;
	
};
