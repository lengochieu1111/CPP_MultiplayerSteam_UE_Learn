

#include "Widget/CharacterWidget.h"
#include "Components/ProgressBar.h"

void UCharacterWidget::UpdateBar_Health(float Health, float MaxHealth)
{
	if(this->Bar_Health)
		this->Bar_Health->SetPercent(Health / MaxHealth);
}
