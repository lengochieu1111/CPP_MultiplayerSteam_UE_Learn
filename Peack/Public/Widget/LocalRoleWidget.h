// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "LocalRoleWidget.generated.h"

class UTextBlock;

UCLASS()
class PEACK_API ULocalRoleWidget : public UUserWidget
{
	GENERATED_BODY()

/* FUNCTION */
public:
	void UpdateText_Role(FText NewText);


/* PROPERTY */
private:

	UPROPERTY(meta = (BindWidget))
	TObjectPtr<UTextBlock> Text_Role;

};
