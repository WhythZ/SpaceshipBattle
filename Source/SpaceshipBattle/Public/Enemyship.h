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
	USphereComponent* enemyshipCollision;                    //���ڴ�����˷ɴ���ײ��������ײ���

	//BlueprintReadWriteʹ�ø����������ͼ�༭���в鿴���޸ģ�����ֻ���������BlueprintReadOnly
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Component")
	UStaticMeshComponent* enemyshipStaticMesh;               //������ʾ���˷ɴ��ľ�̬�������
	#pragma endregion

	#pragma region Properties
	UPROPERTY(EditAnywhere, Category = "Movement")
	float moveSpeed = 800.0f;                                //���˷ɴ��ƶ��ٶ�
	#pragma endregion

	ASpaceship* playerSpaceship;                             //��ҷɴ������ã����ڻ�ȡ���λ��
	//AEnemyshipSpawner* enemyshipSpawner;                     //���˷ɴ������������ã����ڵ����书��

public:
	AEnemyship();

protected:
	virtual void BeginPlay() override;

	void MoveTowardsPlayer(float);

	//ʹ�øú���������ͼ�༭���н���ʵ�֣���������C++��ʵ��
	UFUNCTION(BlueprintImplementableEvent)
	void SetRandomColor();                                   //���˷ɴ�������ʱ��������Ļ��������ɫ

public:	
	virtual void Tick(float) override;

	virtual void SetupPlayerInputComponent(class UInputComponent*) override;

	void OnDeath();
};
