

#include "Controller/PeackPlayerController.h"

#include "Blueprint/UserWidget.h"

void APeackPlayerController::CreateWidget_Character()
{
	if (this->Widget_Character) return;

	this->Widget_Character = CreateWidget< UUserWidget>(this, this->WidgetClass_Character);

	if (this->Widget_Character)
	{
		this->Widget_Character->AddToViewport();
	}

}
