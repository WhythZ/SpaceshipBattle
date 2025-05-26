// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

UCLASS()
class SPACESHIPBATTLE_API ABullet : public AActor
{
	GENERATED_BODY()
	
protected:
	#pragma region Components
	UPROPERTY(VisibleAnywhere, Category = "Component")
	USceneComponent* bulletRootEmpty;                     //�������Ϊ�������ʹ�������StaticMesh�ɵ�����ת�Ƕ�

	UPROPERTY(VisibleAnywhere, Category = "Component")
	UStaticMeshComponent* bulletStaticMesh;               //��̬����������˴������ײ������ã�����USphereComponent��
	#pragma endregion

public:
	ABullet();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float) override;
};
