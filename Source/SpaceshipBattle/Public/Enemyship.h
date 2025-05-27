// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Enemyship.generated.h"

class USphereComponent;
class ASpaceship;

UCLASS()
class SPACESHIPBATTLE_API AEnemyship : public APawn
{
	GENERATED_BODY()

protected:
	#pragma region Components
	UPROPERTY(VisibleAnywhere, Category = "Component")
	USphereComponent* enemyshipCollision;                    //用于处理敌人飞船碰撞的球形碰撞组件

	UPROPERTY(VisibleAnywhere, Category = "Component")
	UStaticMeshComponent* enemyshipStaticMesh;               //用于显示敌人飞船的静态网格组件
	#pragma endregion

	#pragma region Properties
	UPROPERTY(EditAnywhere, Category = "Movement")
	float moveSpeed = 800.0f;                                //敌人飞船移动速度
	#pragma endregion

	ASpaceship* playerSpaceship;                             //玩家飞船的引用，用于获取玩家位置

public:
	AEnemyship();

protected:
	virtual void BeginPlay() override;

	void MoveTowardsPlayer(float);

public:	
	virtual void Tick(float) override;

	virtual void SetupPlayerInputComponent(class UInputComponent*) override;
};
