
// Fill out your copyright notice in the Description page of Project Settings.


#include "Prop/PCBomb.h"
#include "Components/SphereComponent.h"
#include "Field/FieldSystemComponent.h"
#include "Field/FieldSystemNodes.h"
#include "Field/FieldSystemTypes.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystem.h"
#include "Sound/SoundBase.h"
#include "Engine/DamageEvents.h"
#include "Game/PCSpawnManager.h"
#include "Physics/PCCollision.h"

APCBomb::APCBomb()
{
    GasTankMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GasTankMesh"));
    GasTankMesh->SetCollisionProfileName(TEXT("PhysicsActor"));
    GasTankMesh->SetCollisionResponseToChannel(ECC_Pawn, ECR_Block);
    RootComponent = GasTankMesh;

    static ConstructorHelpers::FObjectFinder<UStaticMesh> GasTankMeshRef(TEXT("/Script/Engine.StaticMesh'/Game/IndustryPropsPack6/Meshes/SM_Barrel01.SM_Barrel01'"));
    if (GasTankMeshRef.Object)
    {
        GasTankMesh->SetStaticMesh(GasTankMeshRef.Object);
    }

    static ConstructorHelpers::FObjectFinder<USoundBase> ExplosionSoundRef(TEXT("/Script/Engine.SoundCue'/Game/PandoraCube/Sound/Cue/Explosion_Cue.Explosion_Cue'"));
    if (ExplosionSoundRef.Object)
    {
        ExplosionSound = ExplosionSoundRef.Object;
    }

    static ConstructorHelpers::FObjectFinder<UParticleSystem> ExplosionEffectRef(TEXT("/Script/Engine.ParticleSystem'/Game/Realistic_Starter_VFX_Pack_Vol2/Particles/Explosion/P_Explosion_Big_A.P_Explosion_Big_A'"));
    if (nullptr != ExplosionEffectRef.Object)
    {
        ExplosionEffect = ExplosionEffectRef.Object;
    }
    
    SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp"));
    SphereComp->SetupAttachment(GasTankMesh);
	SphereComp->InitSphereRadius(500.f);
    
    FieldSystemComp = GetFieldSystemComponent();
    if (FieldSystemComp)
    {
        FieldSystemComp->SetupAttachment(GasTankMesh);
    }

    Tags.Add(FName("Bomb"));
    Tags.Add(FName("Metal"));
    Health = 200.0f;
}

void APCBomb::BeginPlay()
{
    Super::BeginPlay();

    if (!RadialFalloffNode)
    {
        RadialFalloffNode = NewObject<URadialFalloff>(this);
        RadialFalloffNode->Magnitude = 5000000.0f;
        RadialFalloffNode->MinRange = 0.0f;
        RadialFalloffNode->MaxRange = 1.0f;
        RadialFalloffNode->Default = 0.0f;
        RadialFalloffNode->Radius = SphereComp->GetScaledSphereRadius();
        RadialFalloffNode->Falloff = EFieldFalloffType::Field_Falloff_Linear;
        RadialFalloffNode->Position = SphereComp->GetComponentLocation();
    }
}

void APCBomb::PostInitializeComponents()
{
    Super::PostInitializeComponents();

    if (GasTankMesh)
    {
        GasTankMesh->SetSimulatePhysics(true);

        GasTankMesh->BodyInstance.bLockXTranslation = true;
        GasTankMesh->BodyInstance.bLockYTranslation = true;
        GasTankMesh->BodyInstance.bLockZTranslation = true;

        GasTankMesh->BodyInstance.bLockXRotation = true;
        GasTankMesh->BodyInstance.bLockYRotation = true;
        GasTankMesh->BodyInstance.bLockZRotation = true;

        GasTankMesh->BodyInstance.UpdateMassProperties();
        GasTankMesh->RecreatePhysicsState();
    }
}

float APCBomb::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
    Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
    
    Health -= DamageAmount;
    if (Health <= 0)
    {
        Explode();
    }

    return DamageAmount;
}

void APCBomb::Explode()
{
    if (ExplosionEffect)
    {
        FVector SpawnLocation = GetActorLocation(); 
        FRotator SpawnRotation = GetActorRotation(); 
        FVector SpawnScale = FVector(3.0f, 3.0f, 3.0f);

        UGameplayStatics::SpawnEmitterAtLocation(
            GetWorld(),
            ExplosionEffect,
            SpawnLocation,
            SpawnRotation,
            SpawnScale,
            true
        );
    }

    if (ExplosionSound)
    {
        FVector SoundLocation = GetActorLocation();

        UGameplayStatics::PlaySoundAtLocation(
            this,            
            ExplosionSound,  
            SoundLocation    
        );
    }

    if (FieldSystemComp && RadialFalloffNode)
    {
        FieldSystemComp->ApplyPhysicsField(
            true,                                 
            EFieldPhysicsType::Field_ExternalClusterStrain,  
            nullptr,                              
            RadialFalloffNode
        );
    }

    FVector ExplosionCenter = GetActorLocation();

    TArray<FOverlapResult> Overlaps;
    FCollisionQueryParams QueryParams(SCENE_QUERY_STAT(ExplodeOverlap), false, this);
    FCollisionObjectQueryParams ObjectQueryParams;
    ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

    bool bHit = GetWorld()->OverlapMultiByObjectType(
        Overlaps,
        ExplosionCenter,
        FQuat::Identity,
        ObjectQueryParams,
        FCollisionShape::MakeSphere(ExplosionRadius),
        QueryParams
    );

    if (bHit)
    {
        for (auto& Overlap : Overlaps)
        {
            AActor* OtherActor = Overlap.GetActor();
            if (OtherActor && OtherActor != this)
            {
                FDamageEvent DamageEvent;

                OtherActor->TakeDamage(
                    ExplosionDamage,
                    DamageEvent,
                    GetInstigatorController(),
                    this
                );
            }
        }
    }

    if (!SpawnManager)
    {
        SpawnManager = APCSpawnManager::GetInstance(GetWorld());
    }

    if (SpawnManager)
    {
        UE_LOG(LogTemp, Warning, TEXT("BOOM"));
        SpawnManager->SpawnZombiesInWave();
    }

    Destroy();
}
