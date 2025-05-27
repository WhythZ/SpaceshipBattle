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
	TSubclassOf<AEnemyship> enemyshipBlueprint;                //���յ���Ԥ���壬����ʵ��������

	UPROPERTY(VisibleAnywhere, Category = "Enemyship")
	UBoxComponent* spawnArea;                                  //������������ʹ�ú�����ײ�����Ϊ��������Ĳο�

public:	
	AEnemyshipSpawner();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float) override;
};
