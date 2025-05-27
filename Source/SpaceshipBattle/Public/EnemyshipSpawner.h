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
	TSubclassOf<AEnemyship> enemyshipBlueprint;                //接收敌人预制体，用于实例化敌人

	//TArray<AEnemyship*> enemieships;                           //存储所有生成的敌人飞船的数组

	#pragma region SpawnEnemyship
	UPROPERTY(VisibleAnywhere, Category = "SpawnEnemyship")
	UBoxComponent* spawnArea;                                  //敌人生成区域，使用盒形碰撞组件作为生成区域的参考

	UPROPERTY(EditAnywhere, Category = "SpawnEnemyship")
	float minimalDistanceToPlayer = 1000.0f;                   //敌人生成点与玩家之间的最小距离

	ASpaceship* playerSpaceship;                               //玩家飞船的引用

	FTimerHandle spawnTimerHandle;                             //用于间歇生成敌人飞船的计时器句柄

	UPROPERTY(EditAnywhere, Category = "SpawnEnemyship")
	float spawnInterval = 2.0f;                                //敌人飞船生成的时间间隔，单位为秒
	#pragma endregion

public:	
	AEnemyshipSpawner();

protected:
	virtual void BeginPlay() override;

	#pragma region SpawnEnemyship
	FVector FindSpawnPoint();                                  //找到一个合适的生成点位置
	void SpawnEnemyship();                                     //在指定位置生成敌人飞船
	#pragma endregion

public:	
	virtual void Tick(float) override;

	//void ClearAllEnemyships();
};
