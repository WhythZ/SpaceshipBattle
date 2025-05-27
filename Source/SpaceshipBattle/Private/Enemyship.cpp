// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemyship.h"

//组件头文件
#include "Components/SphereComponent.h"

//用于数学运算
#include "Kismet/KismetMathLibrary.h"
//用于获取玩家飞船的引用，并调用其功能
#include "Kismet/GameplayStatics.h"
#include "Spaceship.h"

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
}

void AEnemyship::MoveTowardsPlayer(float _delta)
{
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

