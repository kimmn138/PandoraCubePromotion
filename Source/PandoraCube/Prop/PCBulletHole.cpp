// Fill out your copyright notice in the Description page of Project Settings.


#include "Prop/PCBulletHole.h"
#include "Components/DecalComponent.h"
#include "Components/TimelineComponent.h"
#include "Materials/Material.h"

// Sets default values
APCBulletHole::APCBulletHole()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BulletHole = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
	RootComponent = BulletHole;
	BulletHole->SetWorldScale3D(FVector(0.2f, 0.01f, 0.01f));
	BulletHole->SetWorldRotation(FRotator(0.0f, -180.0f, 0.0f));

	BulletHoleTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("BulletHoleTimeline"));
	BulletHoleInterpFunction.BindUFunction(this, FName("BulletHoleTimelineProgress"));

	static ConstructorHelpers::FObjectFinder<UMaterial> BulletHoleMaterialRef(TEXT("/Game/Megascans/Decals/Concrete_Patch_Oil_sdpqjfi/M_DynamicDecal.M_DynamicDecal"));
	if (nullptr != BulletHoleMaterialRef.Object)
	{
		BulletHole->SetDecalMaterial(BulletHoleMaterialRef.Object);
	}
	static ConstructorHelpers::FObjectFinder<UCurveFloat> BulletHoleCurveRef(TEXT("/Script/Engine.CurveFloat'/Game/PandoraCube/Curve/BulletHoleCurveFloat.BulletHoleCurveFloat'"));
	if (nullptr != BulletHoleCurveRef.Object)
	{
		BulletHoleCurve = BulletHoleCurveRef.Object;
	}

}

// Called when the game starts or when spawned
void APCBulletHole::BeginPlay()
{
	Super::BeginPlay();

	BulletHole->SetLifeSpan(10.0f);

	if (BulletHoleCurve)
	{
		BulletHoleTimeline->AddInterpFloat(BulletHoleCurve, BulletHoleInterpFunction);
		BulletHoleTimeline->SetTimelineLength(0.15f);
		BulletHoleTimeline->SetLooping(false);
	}
	
	DynamicMaterialInstance = BulletHole->CreateDynamicMaterialInstance();
	if (DynamicMaterialInstance)
	{
		if (BulletHoleCurve && !BulletHoleTimeline->IsPlaying())
		{
			BulletHoleTimeline->PlayFromStart();
		}
	}
}

void APCBulletHole::BulletHoleTimelineProgress(float Value)
{
	DynamicMaterialInstance->SetScalarParameterValue("emissive_boost", Value);
}

