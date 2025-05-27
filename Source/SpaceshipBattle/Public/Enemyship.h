// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Enemyship.generated.h"

class USphereComponent;
class ASpaceship;
class AEnemyshipSpawner;

UCLASS()
class SPACESHIPBATTLE_API AEnemyship : public APawn
{
	GENERATED_BODY()

protected:
	#pragma region Components
	UPROPERTY(VisibleAnywhere, Category = "Component")
	USphereComponent* enemyshipCollision;                    //用于处理敌人飞船碰撞的球形碰撞组件

	//BlueprintReadWrite使得该组件能在蓝图编辑器中查看和修改，若想只读则可设置BlueprintReadOnly
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	UStaticMeshComponent* enemyshipStaticMesh;               //用于显示敌人飞船的静态网格组件
	#pragma endregion

	#pragma region Properties
	UPROPERTY(EditAnywhere, Category = "Movement")
	float moveSpeed = 800.0f;                                //敌人飞船移动速度
	#pragma endregion

	ASpaceship* playerSpaceship;                             //玩家飞船的引用，用于获取玩家位置
	//AEnemyshipSpawner* enemyshipSpawner;                     //敌人飞船生成器的引用，用于调用其功能

public:
	AEnemyship();

protected:
	virtual void BeginPlay() override;

	void MoveTowardsPlayer(float);

	//使得该函数能在蓝图编辑器中进行实现，而不是在C++中实现
	UFUNCTION(BlueprintImplementableEvent)
	void SetRandomColor();                                   //敌人飞船在生成时设置随机的机身机翼颜色

public:	
	virtual void Tick(float) override;

	virtual void SetupPlayerInputComponent(class UInputComponent*) override;

	void OnDeath();
};
