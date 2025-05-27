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
	//��ȡ��ҷɴ������ã��˴�GetPlayerPawn��ȡ0�ŵ�APawn������ң�ǿתΪASpaceship����
	//ע������BeginPlay()�л�ȡ����Ϊ�ڵ�ǰ��Ĺ��캯������ʱ����ҿ��ܻ�δ�������
	playerSpaceship = Cast<ASpaceship>(UGameplayStatics::GetPlayerPawn(this, 0));

	//����spawnTimerHandle��ʱ��
	GetWorldTimerManager().SetTimer(
		spawnTimerHandle,                   //��ʱ�����
		this,                               //���õĶ���
		&AEnemyshipSpawner::SpawnEnemyship, //���õĺ���
		spawnInterval,                      //��ʱ�����ʱ��
		true,                               //�Ƿ�ѭ������
		0.0f                                //��ʼ�ӳ�ʱ��
	);
	#pragma endregion
}

FVector AEnemyshipSpawner::FindSpawnPoint()
{
	FVector _randomPoint;

	do
	{
		//��ָ���İ�Χ�У�Bounding Box�����������һ��3D�����
		_randomPoint = UKismetMathLibrary::RandomPointInBoundingBox(
			spawnArea->Bounds.Origin,    //��Χ�е����ĵ㣨FVector��
			spawnArea->Bounds.BoxExtent  //�Ӱ�Χ�����ĵ㵽������Ե�ľ��루FVector��
		);
	}
	//�޶����ɵ��������Ϊ���ĵ�Բ��֮��
	while ((playerSpaceship->GetActorLocation() - _randomPoint).Size() <= minimalDistanceToPlayer);

	return _randomPoint;
}

void AEnemyshipSpawner::SpawnEnemyship()
{
	//��ֹԤ����δ�����ã����µ��ô˺������±���
	if (enemyshipBlueprint)
	{
		//�������λ�ú�����ת����enemyshipBlueprint���͵ĵ��˷ɴ�ʵ��
		FActorSpawnParameters _spawnParams;
		AEnemyship* _newEnemyship = GetWorld()->SpawnActor<AEnemyship>(enemyshipBlueprint,
			FindSpawnPoint(), FRotator::ZeroRotator, _spawnParams);
		////�������ɵĵ��˷ɴ���ӵ�������
		//if (_newEnemyship) enemieships.Add(_newEnemyship);
	}
}

void AEnemyshipSpawner::Tick(float _delta)
{
	Super::Tick(_delta);
}
