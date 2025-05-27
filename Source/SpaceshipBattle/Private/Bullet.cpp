// Fill out your copyright notice in the Description page of Project Settings.

#include "Bullet.h"

#include "GameFramework/ProjectileMovementComponent.h"

#include "Engine/BlockingVolume.h"

#include "Enemyship.h"

ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = true;

	#pragma region Components
	bulletRootEmpty = CreateDefaultSubobject<USceneComponent>(TEXT("BulletRootEmpty"));
	RootComponent = bulletRootEmpty;

	bulletStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletStaticMesh"));
	bulletStaticMesh->SetupAttachment(RootComponent);

	//该组件和根组件同级，无需绑定到RootComponent
	bulletProjMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("BulletProjMovement"));
	#pragma endregion
}

void ABullet::BeginPlay()
{
	Super::BeginPlay();	
}

void ABullet::Tick(float _delta)
{
	Super::Tick(_delta);
}

void ABullet::NotifyActorBeginOverlap(AActor* _otherActor)
{
	//由于是重写，故先调用父类方法
	Super::NotifyActorBeginOverlap(_otherActor);

	//当子弹与敌人发送碰撞时，销毁敌人与子弹
	AEnemyship* _enemyship = Cast<AEnemyship>(_otherActor);
	//若_enemyship不为空，说明_otherActor是敌人飞船类型的对象
	if (_enemyship)
	{
		_enemyship->Destroy();
		Destroy();
	}
	//碰到墙的阻挡时销毁子弹
	if (Cast<ABlockingVolume>(_otherActor))
		Destroy();
}
