

#include "Controller/PeackPlayerController.h"

#include "Widget/CharacterWidget.h"

void APeackPlayerController::CreateWidget_Character()
{
	if (this->Widget_Character) return;

	this->Widget_Character = CreateWidget< UCharacterWidget>(this, this->WidgetClass_Character);

	if (this->Widget_Character)
	{
		this->Widget_Character->AddToViewport();
	}

}

void APeackPlayerController::UpdateBar_Health(float Health, float MaxHealth)
{
	if (this->Widget_Character)
	{
		this->Widget_Character->UpdateBar_Health(Health, MaxHealth);
	}
}
