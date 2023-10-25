// Fill out your copyright notice in the Description page of Project Settings.


#include "Widget/LocalRoleWidget.h"
#include "Components/TextBlock.h"

void ULocalRoleWidget::UpdateText_Role(FText NewText)
{
	if (this->Text_Role)
		this->Text_Role->SetText(NewText);
}
