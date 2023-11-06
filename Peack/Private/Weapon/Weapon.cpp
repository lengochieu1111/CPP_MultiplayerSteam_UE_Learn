

#include "Weapon/Weapon.h"

AWeapon::AWeapon()
{
	PrimaryActorTick.bCanEverTick = false;
	PrimaryActorTick.bStartWithTickEnabled = false;
	bReplicates = true;

	/* Skeletal Mesh Component */
	this->MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh Component"));
	this->MeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);

}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

void AWeapon::WeaponFire()
{
	if (this->MeshComponent)
	{
		this->MeshComponent->PlayAnimation(this->AA_Fire, false);
	}
}


