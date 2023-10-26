
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PeackCharacter.generated.h"

class USpringArmComponent;
class UCameraComponent;

class UInputMappingContext;
class UInputAction;
struct FInputActionValue;

class UWidgetComponent;

class AWeapon;

UCLASS()
class PEACK_API APeackCharacter : public ACharacter
{
	GENERATED_BODY()

/* FUNCTION */
public:
	APeackCharacter();
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

private:
	void SetupMappingContext();
	void Look(const FInputActionValue& Value);
	void Move(const FInputActionValue& Value);

	void ShowLocalRole();
	void SpawnWeapon();


/* PROPERTY */
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UWidgetComponent> WidgetComponent;

	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	TSubclassOf<AWeapon> WeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = Weapon)
	FName RifleSocketName;
	
	/*
	* Input
	*/
	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> IMC_Peack;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Look;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Move;



};
