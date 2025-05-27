// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "EnemyshipSpawner.generated.h"

class AEnemyship;
class UBoxComponent;

UCLASS()
class SPACESHIPBATTLE_API AEnemyshipSpawner : public AActor
{
	GENERATED_BODY()
	
protected:
	UPROPERTY(EditAnywhere, Category = "Component")
	TSubclassOf<AEnemyship> enemyshipBlueprint;                //接收敌人预制体，用于实例化敌人

	UPROPERTY(VisibleAnywhere, Category = "Enemyship")
	UBoxComponent* spawnArea;                                  //敌人生成区域，使用盒形碰撞组件作为生成区域的参考

public:	
	AEnemyshipSpawner();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float) override;
};
