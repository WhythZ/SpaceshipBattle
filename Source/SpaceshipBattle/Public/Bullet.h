// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Bullet.generated.h"

class UProjectileMovementComponent;

UCLASS()
class SPACESHIPBATTLE_API ABullet : public AActor
{
	GENERATED_BODY()
	
protected:
	#pragma region Components
	UPROPERTY(VisibleAnywhere, Category = "Component")
	USceneComponent* bulletRootEmpty;                     //空组件作为根组件，使得子组件StaticMesh可调整旋转角度

	UPROPERTY(VisibleAnywhere, Category = "Component")
	UStaticMeshComponent* bulletStaticMesh;               //静态网格组件，此处兼具碰撞体的作用，无需USphereComponent等

	UPROPERTY(VisibleAnywhere, Category = "Component")
	UProjectileMovementComponent* bulletProjMovement;     //子弹投射物运动组件用于控制移动，需将其重力归零防止Z轴坠落
	#pragma endregion

public:
	ABullet();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float) override;
};
