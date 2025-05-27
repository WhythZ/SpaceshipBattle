// Fill out your copyright notice in the Description page of Project Settings.

#include "Enemyship.h"

//���ͷ�ļ�
#include "Components/SphereComponent.h"

//������ѧ����
#include "Kismet/KismetMathLibrary.h"
//���ڻ�ȡ��ҷɴ������ã��������书��
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

	//��ȡ��ҷɴ������ã��˴�GetPlayerPawn��ȡ0�ŵ�APawn������ң�ǿתΪASpaceship����
	playerSpaceship = Cast<ASpaceship>(UGameplayStatics::GetPlayerPawn(this, 0));

	////��ȡ������������
	//TArray<AActor*> _enemyshipSpawners;
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyship::StaticClass(), _enemyshipSpawners);
	//enemyshipSpawner = Cast<AEnemyshipSpawner>(_enemyshipSpawners[0]);

	//���õ��˷ɴ��������ɫ��������ִ��һ������ʱ��Scale���Ÿ�ԭ�Ӿ�Ч�����ú�������ͼ��ʵ��
	SetRandomColor();
}

void AEnemyship::MoveTowardsPlayer(float _delta)
{
	//����ҷɴ������ڻ�����������ִ���ƶ��߼�
	if (!playerSpaceship->IsAlive()) return;

	FVector _playerLocation = playerSpaceship->GetActorLocation();
	FVector _enemyLocation = GetActorLocation();

	#pragma region Velocity
	//����ӵ��˵���ҷɴ��ķ�������������ȫ��һ��
	FVector _targetDirection = (_playerLocation - _enemyLocation).GetSafeNormal();

	//����������ƶ����ǵó���֡�����true��ʾ�ᱻ��ײ�赲
	AddActorWorldOffset(_targetDirection * moveSpeed * _delta, true);
	//AddActorWorldOffset(_targetDirection * moveSpeed * FApp::GetDeltaTime(), true);
	#pragma endregion

	#pragma region Towards
	//����ӵ��˷ɴ���ǰλ�ã������䵱ǰ���򣩿���Ŀ�����λ���������ת�Ƕ�
	FRotator _rotationAngle = UKismetMathLibrary::FindLookAtRotation(
		_enemyLocation,  //��ʼλ�ã�����λ�ã�
		_playerLocation  //Ŀ��λ�ã����λ�ã�
	);

	//ʵ��Ӧ����ת�Ƕȵ��ɴ��ϣ�ʹ���˳������
	SetActorRotation(_rotationAngle);
	#pragma endregion
}

void AEnemyship::Tick(float _delta)
{
	Super::Tick(_delta);

	//������ҷɴ��ƶ�
	MoveTowardsPlayer(_delta);
}

void AEnemyship::SetupPlayerInputComponent(UInputComponent* _playerInputComponent)
{
	Super::SetupPlayerInputComponent(_playerInputComponent);
}

void AEnemyship::OnDeath()
{
	//����ʱ�ı�ը����Ч��
	if (explosionParticleSystem)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			this,
			explosionParticleSystem,  //��ըЧ������ϵͳ
			GetActorLocation(),       //�ڷɴ�λ����������Ч��
			FRotator::ZeroRotator,    //��ת�Ƕ�
			FVector(1.0f),            //���ű���
			true                      //�Ƿ��Զ�����
		);
	}

	Cast<AShipGameMode>(UGameplayStatics::GetGameMode(this))->IncreaseScore();
	Destroy();
}
