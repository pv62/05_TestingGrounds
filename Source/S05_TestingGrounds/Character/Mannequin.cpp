// Fill out your copyright notice in the Description page of Project Settings.

#include "Mannequin.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Weapons/Gun.h"


// Sets default values
AMannequin::AMannequin()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a CameraComponent	
	FP_Camera_Component = CreateDefaultSubobject<UCameraComponent>(TEXT("FP_Camera"));
	FP_Camera_Component->SetupAttachment(GetCapsuleComponent());
	FP_Camera_Component->RelativeLocation = FVector(-39.56f, 1.75f, 64.f); // Position the camera
	FP_Camera_Component->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	FP_Arms_Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FP_Arms"));
	FP_Arms_Mesh->SetOnlyOwnerSee(true);
	FP_Arms_Mesh->SetupAttachment(FP_Camera_Component);
	FP_Arms_Mesh->bCastDynamicShadow = false;
	FP_Arms_Mesh->CastShadow = false;
	FP_Arms_Mesh->RelativeRotation = FRotator(1.9f, -19.19f, 5.2f);
	FP_Arms_Mesh->RelativeLocation = FVector(-0.5f, -4.4f, -155.7f);
}

// Called when the game starts or when spawned
void AMannequin::BeginPlay()
{
	Super::BeginPlay();

	if (GunBlueprint == nullptr) { 
		UE_LOG(LogTemp, Warning, TEXT("Gun blueprint missing.")); 
		return; 
	}

	Gun = GetWorld()->SpawnActor<AGun>(GunBlueprint);

	//Attach gun mesh component to Skeleton, doing it here because the skelton is not yet created in the constructor
	if (IsPlayerControlled())
	{
		Gun->AttachToComponent(FP_Arms_Mesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint")); 
	}
	else
	{
		Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
	}
	Gun->AnimInstance1P = FP_Arms_Mesh->GetAnimInstance();
	Gun->AnimInstance3P = GetMesh()->GetAnimInstance();

	if (InputComponent != nullptr)
	{
		// Bind fire event
		InputComponent->BindAction("Fire", IE_Pressed, this, &AMannequin::PullTrigger);
	}
}

void AMannequin::PullTrigger()
{
	if (Gun)
	{
		Gun->OnFire();
	}
}

// Called every frame
void AMannequin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AMannequin::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AMannequin::UnPossessed()
{
	Super::UnPossessed();
	if (!Gun) { return; }
	Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint"));
}

