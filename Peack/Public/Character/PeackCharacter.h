
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

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// The function is called when control of the character is obtained
	virtual void PossessedBy(AController* NewController) override;


protected:
	virtual void BeginPlay() override;

private:
	void SetupMappingContext();
	void Look(const FInputActionValue& Value);
	void Move(const FInputActionValue& Value);
	void FireButtonPressed();

	void ShowLocalRole();
	void SpawnWeapon();

	UFUNCTION()
	void OnRep_CurrentWeapon();

	void Fire();

	void FiringDelay();

	void FireDelayFinished();

	UFUNCTION(Server, Reliable)
	void Server_Fire();

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_Fire();

	void LineTraceFromCamera();

	void ApplyDamageToPeackCharacter(const FHitResult& HitResult, const FVector& HitDirection);

	UFUNCTION(Client, Reliable)
	void Client_PlayerControllerReady();

	UFUNCTION(NetMulticast, Reliable)
	void Mutilcast_SpawnHitEffect(const FVector& HitLocation);

	UFUNCTION()
	void HandleTakePointDamage(AActor* DamagedActor, float Damage, class AController* InstigatedBy,
		FVector HitLocation, class UPrimitiveComponent* FHitComponent, FName BoneName, 
		FVector ShotFromDirection, const class UDamageType* DamageType, AActor* DamageCauser);

	UFUNCTION(NetMulticast, Reliable)
	void Multicast_PlayHitReactMontage(const FVector& HitDirection);

	UAnimMontage* GetCorrectHitReactMontage(const FVector& HitDirection);

/* PROPERTY */
private:
	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USpringArmComponent> SpringArmComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UCameraComponent> CameraComponent;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UWidgetComponent> WidgetComponent;

	UPROPERTY(ReplicatedUsing = OnRep_CurrentWeapon)
	TObjectPtr<AWeapon> CurrentWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
	TSubclassOf<AWeapon> WeaponClass;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon")
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

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputAction> IA_Fire;

/*
* Fire 
*/
	bool bIsFiring = false;

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	TObjectPtr<UAnimMontage> FireMontage_Rifle;

	UPROPERTY()
	FTimerHandle FireDelayTimer;

	UPROPERTY(EditDefaultsOnly, Category = "Fire")
	float FireRate = 2.0f;

/*
* Health
*/
	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float Health = 100.0f;

	UPROPERTY(EditDefaultsOnly, Category = "Health")
	float MaxHealth = 100.0f;

/*
* Trace Hit
*/
	UPROPERTY(EditDefaultsOnly, Category = "Trace Hit")
	TArray<TEnumAsByte<EObjectTypeQuery>> TraceObjectTypes;

	UPROPERTY(EditDefaultsOnly, Category = "Trace Hit")
	float TraceHitOffset = 50.f;

/*
* Hit Impact
*/
	UPROPERTY(EditDefaultsOnly, Category = "Sound")
	TObjectPtr<USoundBase> HitSound;

	UPROPERTY(EditDefaultsOnly, Category = "Effect")
	TObjectPtr<UParticleSystem> HitEffect;

/*
* Hit React
*/
	UPROPERTY(EditDefaultsOnly, Category = "Hit React")
	TObjectPtr<UAnimMontage> HitReactMontage_Front;

	UPROPERTY(EditDefaultsOnly, Category = "Hit React")
	TObjectPtr<UAnimMontage> HitReactMontage_Back;	
	
	UPROPERTY(EditDefaultsOnly, Category = "Hit React")
	TObjectPtr<UAnimMontage> HitReactMontage_Left;	

	UPROPERTY(EditDefaultsOnly, Category = "Hit React")
	TObjectPtr<UAnimMontage> HitReactMontage_Right;


};
