// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyshipSpawner.h"

#include "Components/BoxComponent.h"

#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "TimerManager.h"

#include "Enemyship.h"
#include "Spaceship.h"

AEnemyshipSpawner::AEnemyshipSpawner()
{
	PrimaryActorTick.bCanEverTick = true;

	spawnArea = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnArea"));
	RootComponent = spawnArea;
}

void AEnemyshipSpawner::BeginPlay()
{
	Super::BeginPlay();

	#pragma region ContinuousSpawn
	//获取玩家飞船的引用，此处GetPlayerPawn获取0号的APawn类型玩家，强转为ASpaceship类型
	//注意是在BeginPlay()中获取，因为在当前类的构造函数调用时，玩家可能还未创建完成
	playerSpaceship = Cast<ASpaceship>(UGameplayStatics::GetPlayerPawn(this, 0));

	//开启spawnTimerHandle计时器
	GetWorldTimerManager().SetTimer(
		spawnTimerHandle,                   //计时器句柄
		this,                               //调用的对象
		&AEnemyshipSpawner::SpawnEnemyship, //调用的函数
		spawnInterval,                      //计时器间隔时间
		true,                               //是否循环调用
		0.0f                                //初始延迟时间
	);
	#pragma endregion
}

FVector AEnemyshipSpawner::FindSpawnPoint()
{
	FVector _randomPoint;

	do
	{
		//在指定的包围盒（Bounding Box）内随机生成一个3D坐标点
		_randomPoint = UKismetMathLibrary::RandomPointInBoundingBox(
			spawnArea->Bounds.Origin,    //包围盒的中心点（FVector）
			spawnArea->Bounds.BoxExtent  //从包围盒中心点到其各轴边缘的距离（FVector）
		);
	}
	//限定生成点在以玩家为中心的圆域之外
	while ((playerSpaceship->GetActorLocation() - _randomPoint).Size() <= minimalDistanceToPlayer);

	return _randomPoint;
}

void AEnemyshipSpawner::SpawnEnemyship()
{
	//防止预制体未被设置，导致调用此函数导致崩溃
	if (enemyshipBlueprint)
	{
		//根据随机位置和零旋转生成enemyshipBlueprint类型的敌人飞船实例
		FActorSpawnParameters _spawnParams;
		AEnemyship* _newEnemyship = GetWorld()->SpawnActor<AEnemyship>(enemyshipBlueprint,
			FindSpawnPoint(), FRotator::ZeroRotator, _spawnParams);
		////将新生成的敌人飞船添加到数组中
		//if (_newEnemyship) enemieships.Add(_newEnemyship);
	}
}

void AEnemyshipSpawner::Tick(float _delta)
{
	Super::Tick(_delta);
}
