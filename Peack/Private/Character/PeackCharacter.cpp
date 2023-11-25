

#include "Character/PeackCharacter.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"

#include "Components/CapsuleComponent.h"

#include "Components/WidgetComponent.h"
#include "Widget/LocalRoleWidget.h"

#include "Weapon/Weapon.h"
#include "Net/UnrealNetwork.h"

#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"

#include "Controller/PeackPlayerController.h"

APeackCharacter::APeackCharacter()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;

	// Adjust Mesh Component
	GetMesh()->AddLocalOffset(FVector(0.0, 0.0, -88.0));
	GetMesh()->AddLocalRotation(FRotator(0.0, -90.0, 0.0));

	// Adjust Movement Component
	bUseControllerRotationYaw = true;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	GetCharacterMovement()->MaxWalkSpeed = 400.0f;

	// Spring Arm
	this->SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("Spring Arm Component"));
	this->SpringArmComponent->SetupAttachment(GetRootComponent());
	this->SpringArmComponent->TargetArmLength = 400.0f;
	this->SpringArmComponent->bUsePawnControlRotation = true;

	this->SpringArmComponent->SocketOffset = FVector(0.0, 50.0, 80.0);
	
	// Camera
	this->CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera Component"));
	this->CameraComponent->SetupAttachment(this->SpringArmComponent);

	// Net
	NetUpdateFrequency = 100.0f;
	MinNetUpdateFrequency = 100.0f;

	//
	GetMesh()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECC_Camera, ECR_Ignore);

	// Widget Component
	this->WidgetComponent = CreateDefaultSubobject<UWidgetComponent>(TEXT("Widget Component"));
	this->WidgetComponent->SetupAttachment(GetRootComponent());

	this->WidgetComponent->AddLocalOffset(FVector(0.0, 0.0, 100.0));
	this->WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);
	this->WidgetComponent->SetDrawAtDesiredSize(true);

	// Set Collision Object Type
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_EngineTraceChannel1);

}

void APeackCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	/* Setup Mapping Context */
	SetupMappingContext();

	/* Setup Input Action */
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(this->IA_Look, ETriggerEvent::Triggered, this, &ThisClass::Look);
		EnhancedInputComponent->BindAction(this->IA_Move, ETriggerEvent::Triggered, this, &ThisClass::Move);
		EnhancedInputComponent->BindAction(this->IA_Fire, ETriggerEvent::Triggered, this, &ThisClass::FireButtonPressed);
	}
}

void APeackCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APeackCharacter, CurrentWeapon);

	DOREPLIFETIME_CONDITION(APeackCharacter, Health, COND_OwnerOnly);

}

void APeackCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	Client_PlayerControllerReady();

}

void APeackCharacter::BeginPlay()
{
	Super::BeginPlay();

	ShowLocalRole();

	if (HasAuthority())
	{
		SpawnWeapon();

		OnTakePointDamage.AddDynamic(this, &ThisClass::HandleTakePointDamage);
	}

}

void APeackCharacter::Client_PlayerControllerReady_Implementation()
{
	if (this->PeackPlayerController = Cast<APeackPlayerController>(GetController()))
	{
		this->PeackPlayerController->CreateWidget_Character();
		this->PeackPlayerController->UpdateBar_Health(this->Health, this->MaxHealth);
	}

}

void APeackCharacter::ShowLocalRole()
{
	if (this->WidgetComponent == nullptr) return;
	
	if (ULocalRoleWidget* LocalRoleWidget = Cast<ULocalRoleWidget>(this->WidgetComponent->GetUserWidgetObject()))
	{
		FText RoleText;
		switch (GetLocalRole())
		{
		case ENetRole::ROLE_Authority:
			RoleText = FText::FromString(TEXT("Authority"));
			break;
		case ENetRole::ROLE_AutonomousProxy:
			RoleText = FText::FromString(TEXT("Autonomous Proxy"));
			break;
		case ENetRole::ROLE_SimulatedProxy:
			RoleText = FText::FromString(TEXT("Simulated Proxy"));
			break;
		} 

		LocalRoleWidget->UpdateText_Role(RoleText);
	}

}

void APeackCharacter::SpawnWeapon()
{
	if (IsValid(this->WeaponClass) == false) return;

	UWorld* MyLevel = GetWorld();
	if (IsValid(MyLevel) == false) return;

	// Spawn Weapon
	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = this;
	SpawnParameters.Instigator = this;
	AWeapon* SpawnWeapon = MyLevel->SpawnActor<AWeapon>(this->WeaponClass, SpawnParameters);

	if (SpawnWeapon == nullptr) return;

	// Attach The Weapon To Mesh
	FAttachmentTransformRules AttachmentRules 
		= FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true);
	SpawnWeapon->AttachToComponent(GetMesh(), AttachmentRules, this->RifleSocketName);

	this->CurrentWeapon = SpawnWeapon;
}

void APeackCharacter::OnRep_CurrentWeapon()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(
			-1,
			3.0f,
			FColor::Purple,
			TEXT("OnRep_CurrentWeapon")
		);
}

	#pragma region Enhanced Input

void APeackCharacter::SetupMappingContext()
{
	if (APlayerController* MyPlayerController = Cast<APlayerController>(GetController()))
	{
		if (ULocalPlayer* MyLocalPlayer = MyPlayerController->GetLocalPlayer())
		{
			UEnhancedInputLocalPlayerSubsystem* Subsystem
				= ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(MyLocalPlayer);

			if (Subsystem)
				Subsystem->AddMappingContext(this->IMC_Peack, 0);
		}
	}
}

void APeackCharacter::Look(const FInputActionValue& Value)
{
	FVector2D Value_Vector2D = Value.Get<FVector2D>();

	if (Value_Vector2D.X != 0)
		AddControllerYawInput(Value_Vector2D.X);

	if (Value_Vector2D.Y != 0)
		AddControllerPitchInput(Value_Vector2D.Y);

}

void APeackCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D Value_Vector2D = Value.Get<FVector2D>();
	const FRotator ControllerRotation = FRotator(0.0, GetControlRotation().Yaw, 0.0);

	if (Value_Vector2D.Y != 0)
	{
		const FVector ForwardDirection = ControllerRotation.RotateVector(FVector::ForwardVector);
		AddMovementInput(ForwardDirection, Value_Vector2D.Y);
	}
	
	if (Value_Vector2D.X != 0)
	{
		const FVector RightDirection = ControllerRotation.RotateVector(FVector::RightVector);
		AddMovementInput(RightDirection, Value_Vector2D.X);
	}
}

void APeackCharacter::FireButtonPressed()
{
	Fire();
}

	#pragma endregion

	#pragma region Fire

void APeackCharacter::Fire()
{
	Server_Fire();
}

// Server
void APeackCharacter::Server_Fire_Implementation()
{
	if (this->bIsFiring) return;

	LineTraceFromCamera();
	Multicast_Fire();
	this->bIsFiring = true;

	FiringDelay();
}

void APeackCharacter::FiringDelay()
{
	FTimerManager& WorldTimerManager = GetWorldTimerManager();

	if (WorldTimerManager.IsTimerActive(this->FireDelayTimer))
		WorldTimerManager.ClearTimer(this->FireDelayTimer);

	WorldTimerManager.SetTimer(
		this->FireDelayTimer,
		this,
		&ThisClass::FireDelayFinished,
		1.0f / this->FireRate
	);

}

void APeackCharacter::FireDelayFinished()
{
	this->bIsFiring = false;
}

// Server, Client
void APeackCharacter::Multicast_Fire_Implementation()
{
	if (this->CurrentWeapon)
	{
		PlayAnimMontage(this->FireMontage_Rifle);
		this->CurrentWeapon->WeaponFire();
	}
}

void APeackCharacter::LineTraceFromCamera()
{
	if (this->CameraComponent == nullptr) return;

	const FVector& CameraLocation = this->CameraComponent->GetComponentLocation();
	const FVector& CameraDirection = this->CameraComponent->GetForwardVector();

	float TraceStartOffset = FVector::Distance(CameraLocation, GetActorLocation()) + this->TraceHitOffset;

	FVector StartLocation = CameraLocation + (CameraDirection * TraceStartOffset);
	FVector EndLocation = StartLocation + (CameraDirection * 10'000'00);

	TArray<AActor*> ActorsToIgnore;
	FHitResult HitResult;

	const bool bDoHit = UKismetSystemLibrary::LineTraceSingleForObjects(
		this,
		StartLocation,
		EndLocation,
		this->TraceObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::ForDuration,
		HitResult,
		true
	);

	if (bDoHit)
	{
		Mutilcast_SpawnHitEffect(HitResult.ImpactPoint);

		ApplyDamageToPeackCharacter(HitResult, CameraDirection);

	}

}

void APeackCharacter::ApplyDamageToPeackCharacter(const FHitResult& HitResult, const FVector& HitDirection)
{
	UGameplayStatics::ApplyPointDamage(
		HitResult.GetActor(),
		20.0f,
		HitDirection,
		HitResult,
		GetController(),
		this->CurrentWeapon,
		UGameplayStatics::StaticClass()
	);
}

void APeackCharacter::HandleTakePointDamage(AActor* DamagedActor, float Damage, AController* InstigatedBy, FVector HitLocation, UPrimitiveComponent* FHitComponent, FName BoneName, FVector ShotFromDirection, const UDamageType* DamageType, AActor* DamageCauser)
{
	this->Health = FMath::Max(this->Health - Damage, 0);

	if (IsLocallyControlled())
		this->PeackPlayerController->UpdateBar_Health(this->Health, this->MaxHealth);

	Multicast_PlayHitReactMontage(ShotFromDirection);

}

// Client who controlled this character
void APeackCharacter::OnRep_Health()
{
	if (GEngine)
		GEngine->AddOnScreenDebugMessage(
			-1,
			1.0f,
			FColor::Blue,
			FString(TEXT("OnRep_Health"))
		);

	this->PeackPlayerController->UpdateBar_Health(this->Health, this->MaxHealth);

}

// Server, Client
void APeackCharacter::Multicast_PlayHitReactMontage_Implementation(const FVector& HitDirection)
{
	PlayAnimMontage(GetCorrectHitReactMontage(HitDirection));
}

UAnimMontage* APeackCharacter::GetCorrectHitReactMontage(const FVector& HitDirection)
{
	const FVector& ForwardDirection = GetActorForwardVector();

	float Dot = FVector::DotProduct(HitDirection, ForwardDirection);
	bool bShouldUseDot = FMath::Abs(Dot) > 0.5;
	if (bShouldUseDot)
	{
		if (Dot > 0.5)
			return this->HitReactMontage_Back;
		else
			return this->HitReactMontage_Front;
	}
	else
	{
		FVector Cross = FVector::CrossProduct(HitDirection, ForwardDirection);
		if (Cross.Z > 0.5)
			return this->HitReactMontage_Right;
		else
			return this->HitReactMontage_Left;
	}

}

// Server, Client
void APeackCharacter::Mutilcast_SpawnHitEffect_Implementation(const FVector& HitLocation)
{
	UGameplayStatics::SpawnEmitterAtLocation(
		GetWorld(),
		this->HitEffect,
		HitLocation
	);

	UGameplayStatics::PlaySoundAtLocation(
		this,
		this->HitSound,
		HitLocation
	);

}
	#pragma endregion



