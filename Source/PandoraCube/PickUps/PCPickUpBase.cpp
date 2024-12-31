// Fill out your copyright notice in the Description page of Project Settings.


#include "PickUps/PCPickUpBase.h"
#include "Components/BoxComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "Character/PCPlayerCharacter.h"
#include "Engine/DataTable.h"

// Sets default values
APCPickUpBase::APCPickUpBase()
{
	BoxCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision"));
	BoxCollision->SetBoxExtent(FVector(42.95897f, 17.13143f, 3.400801f));
	BoxCollision->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	BoxCollision->SetCollisionProfileName(TEXT("PhysicsActor"));
	BoxCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Ignore);
	RootComponent = BoxCollision;

	BoxCollision2 = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxCollision2"));
	BoxCollision2->SetRelativeLocation(FVector(0.0f, 0.0f, 13.94f));
	BoxCollision2->SetCollisionProfileName(TEXT("OverlapAll"));
	BoxCollision2->SetGenerateOverlapEvents(true);
	BoxCollision2->SetupAttachment(BoxCollision);

	GunMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("GunMesh"));
	GunMesh->SetRelativeRotation(FRotator(90.f, -90.f, 0.f));
	GunMesh->SetupAttachment(BoxCollision);

	static ConstructorHelpers::FObjectFinder<UDataTable> ItemDataTableRef(TEXT("/Script/Engine.DataTable'/Game/PandoraCube/ItemData/ItemDataTable.ItemDataTable'"));
	if (nullptr != ItemDataTableRef.Object)
	{
		ItemDataTable = ItemDataTableRef.Object;
	}
}

// Called when the game starts or when spawned
void APCPickUpBase::BeginPlay()
{
	Super::BeginPlay();
	
	BoxCollision2->OnComponentBeginOverlap.AddDynamic(this, &APCPickUpBase::OnOverlapBegin);
}

void APCPickUpBase::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (BoxCollision)
	{
		BoxCollision->SetSimulatePhysics(true);
		BoxCollision->SetMassOverrideInKg(NAME_None, 100.0f, true);
		BoxCollision->SetLinearDamping(5.0f);
		BoxCollision->SetAngularDamping(5.0f);
	}
}

void APCPickUpBase::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (OtherActor->ActorHasTag(TEXT("Player")))
	{
		if (OtherActor->GetClass()->ImplementsInterface(UPCPlayerInterface::StaticClass()))
		{
			IPCPlayerInterface::Execute_AddItemToInventory(OtherActor, this, Item);
		}
	}
}
