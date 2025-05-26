// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Spaceship.generated.h"

//此外也可引入头文件，但此处前向声明能提升编译速度
class USphereComponent;
class UCameraComponent;

UCLASS()
class SPACESHIPBATTLE_API ASpaceship : public APawn
{
	GENERATED_BODY()

protected:
	//用VisibleAnywhere使该组件在UE蓝图编辑器中可见（对组件来说，也可以更改），并用Category将其分类
	//但对于内置属性如int等，VisibleAnywhere能使其可见但不可更改，除非用EditableAnywhere
	UPROPERTY(VisibleAnywhere, Category="Component")
	USphereComponent* SpaceshipCollision;                    //用于处理飞船碰撞的球形碰撞组件
	
	UPROPERTY(VisibleAnywhere, Category = "Component")
	UStaticMeshComponent* SpaceshipStaticMesh;               //用于显示飞船的静态网格组件

	UPROPERTY(VisibleAnywhere, Category = "Component")
	UCameraComponent* SpaceshipCamera;

public:
	ASpaceship();                                            //构造函数，用于设置默认属性

protected:
	virtual void BeginPlay() override;                       //游戏开始或生成时调用

public:
	virtual void Tick(float DeltaTime) override;             //逐帧调用

	//调用来绑定输入功能
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
