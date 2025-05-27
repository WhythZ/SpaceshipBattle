// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyshipSpawner.generated.h"

class AEnemyship;
class ASpaceship;
class UBoxComponent;

UCLASS()
class SPACESHIPBATTLE_API AEnemyshipSpawner : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "Component")
	TSubclassOf<AEnemyship> enemyshipBlueprint;                //���յ���Ԥ���壬����ʵ��������

	//TArray<AEnemyship*> enemieships;                           //�洢�������ɵĵ��˷ɴ�������

	#pragma region SpawnEnemyship
	UPROPERTY(VisibleAnywhere, Category = "SpawnEnemyship")
	UBoxComponent* spawnArea;                                  //������������ʹ�ú�����ײ�����Ϊ��������Ĳο�

	UPROPERTY(EditAnywhere, Category = "SpawnEnemyship")
	float minimalDistanceToPlayer = 1000.0f;                   //�������ɵ������֮�����С����

	ASpaceship* playerSpaceship;                               //��ҷɴ�������

	FTimerHandle spawnTimerHandle;                             //���ڼ�Ъ���ɵ��˷ɴ��ļ�ʱ�����

	UPROPERTY(EditAnywhere, Category = "SpawnEnemyship")
	float spawnInterval = 2.0f;                                //���˷ɴ����ɵ�ʱ��������λΪ��
	#pragma endregion

public:	
	AEnemyshipSpawner();

protected:
	virtual void BeginPlay() override;

	#pragma region SpawnEnemyship
	FVector FindSpawnPoint();                                  //�ҵ�һ�����ʵ����ɵ�λ��
	void SpawnEnemyship();                                     //��ָ��λ�����ɵ��˷ɴ�
	#pragma endregion

public:	
	virtual void Tick(float) override;

	//void ClearAllEnemyships();
};
