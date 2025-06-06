// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemyship.h"

//组件头文件
#include "Components/SphereComponent.h"

//用于数学运算
#include "Kismet/KismetMathLibrary.h"
//用于获取玩家飞船的引用，并调用其功能
#include "Kismet/GameplayStatics.h"

#include "Spaceship.h"
#include "EnemyshipSpawner.h"
#include "ShipGameMode.h"

AEnemyship::AEnemyship()
{
	PrimaryActorTick.bCanEverTick = true;

	#pragma region Components
	enemyshipCollision = CreateDefaultSubobject<USphereComponent>(TEXT("EnemyshipCollision"));
	RootComponent = enemyshipCollision;

	enemyshipStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("EnemyshipStaticMesh"));
	enemyshipStaticMesh->SetupAttachment(RootComponent);
	#pragma endregion
}

void AEnemyship::BeginPlay()
{
	Super::BeginPlay();

	//获取玩家飞船的引用，此处GetPlayerPawn获取0号的APawn类型玩家，强转为ASpaceship类型
	playerSpaceship = Cast<ASpaceship>(UGameplayStatics::GetPlayerPawn(this, 0));

	////获取生成器的引用
	//TArray<AActor*> _enemyshipSpawners;
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyship::StaticClass(), _enemyshipSpawners);
	//enemyshipSpawner = Cast<AEnemyshipSpawner>(_enemyshipSpawners[0]);

	//设置敌人飞船的随机颜色，并对其执行一个生成时的Scale缩放复原视觉效果，该函数在蓝图中实现
	SetRandomColor();
}

void AEnemyship::MoveTowardsPlayer(float _delta)
{
	//若玩家飞船不存在或已死亡，则不执行移动逻辑
	if (!playerSpaceship->IsAlive()) return;

	FVector _playerLocation = playerSpaceship->GetActorLocation();
	FVector _enemyLocation = GetActorLocation();

	#pragma region Velocity
	//计算从敌人到玩家飞船的方向向量，并安全归一化
	FVector _targetDirection = (_playerLocation - _enemyLocation).GetSafeNormal();

	//敌人向玩家移动，记得乘上帧间隔，true表示会被碰撞阻挡
	AddActorWorldOffset(_targetDirection * moveSpeed * _delta, true);
	//AddActorWorldOffset(_targetDirection * moveSpeed * FApp::GetDeltaTime(), true);
	#pragma endregion

	#pragma region Towards
	//计算从敌人飞船当前位置（根据其当前朝向）看向目标玩家位置所需的旋转角度
	FRotator _rotationAngle = UKismetMathLibrary::FindLookAtRotation(
		_enemyLocation,  //起始位置（敌人位置）
		_playerLocation  //目标位置（玩家位置）
	);

	//实际应用旋转角度到飞船上，使敌人朝向玩家
	SetActorRotation(_rotationAngle);
	#pragma endregion
}

void AEnemyship::Tick(float _delta)
{
	Super::Tick(_delta);

	//朝向玩家飞船移动
	MoveTowardsPlayer(_delta);
}

void AEnemyship::SetupPlayerInputComponent(UInputComponent* _playerInputComponent)
{
	Super::SetupPlayerInputComponent(_playerInputComponent);
}

void AEnemyship::OnDeath()
{
	//死亡时的爆炸粒子效果
	EmitExplosionParticles();

	//增加得分计数
	Cast<AShipGameMode>(UGameplayStatics::GetGameMode(this))->IncreaseScore();
	Destroy();
}
