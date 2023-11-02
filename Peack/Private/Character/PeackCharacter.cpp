

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
	}
}

void APeackCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(APeackCharacter, CurrentWeapon);

}

void APeackCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	ShowLocalRole();

	if (HasAuthority())
		SpawnWeapon();

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

	#pragma endregion


