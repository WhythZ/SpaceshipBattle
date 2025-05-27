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

	//������͸����ͬ��������󶨵�RootComponent
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
	//��������д�����ȵ��ø��෽��
	Super::NotifyActorBeginOverlap(_otherActor);

	//���ӵ�����˷�����ײʱ�����ٵ������ӵ�
	AEnemyship* _enemyship = Cast<AEnemyship>(_otherActor);
	//��_enemyship��Ϊ�գ�˵��_otherActor�ǵ��˷ɴ����͵Ķ���
	if (_enemyship)
	{
		_enemyship->Destroy();
		Destroy();
	}
	//����ǽ���赲ʱ�����ӵ�
	if (Cast<ABlockingVolume>(_otherActor))
		Destroy();
}
