// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Spaceship.generated.h"

//前向声明，避免引入头文件拉低编译速度
class USphereComponent;
class USpringArmComponent;
class UCameraComponent;
class ABullet;

UCLASS()
class SPACESHIPBATTLE_API ASpaceship : public APawn
{
	GENERATED_BODY()

protected:
	#pragma region Components
	//用VisibleAnywhere使该组件在UE蓝图编辑器中可见（对组件来说，也可以更改），并用Category将其分类
	//但对于内置属性如int等，VisibleAnywhere能使其可见但不可更改，除非用EditableAnywhere
	UPROPERTY(VisibleAnywhere, Category="Component")
	USphereComponent* spaceshipCollision;                    //用于处理飞船碰撞的球形碰撞组件
	
	UPROPERTY(VisibleAnywhere, Category = "Component")
	UStaticMeshComponent* spaceshipStaticMesh;               //用于显示飞船的静态网格组件

	UPROPERTY(VisibleAnywhere, Category = "Component")
	USpringArmComponent* spaceshipSpringArm;                 //弹簧臂组件，用于控制摄像机与飞船间的关系

	UPROPERTY(VisibleAnywhere, Category = "Component")
	UCameraComponent* spaceshipCamera;				         //照向飞船的视角摄像机组件

	APlayerController* spaceshipPlayerController;            //玩家控制器，用于获取玩家输入
	#pragma endregion

	#pragma region Properties
	//注意此处是EditAnywhere而不是VisibleAnywhere，使得可在蓝图编辑器中修改该数值属性
	UPROPERTY(EditAnywhere, Category = "Movement")
	float moveSpeed = 2000.0f;                               //飞船移动速度
	#pragma endregion

	#pragma region Attack
	UPROPERTY(EditAnywhere, Category = "Attack")
	//TSubclassOf限制该变量仅能接收子弹及其子类的赋值
	TSubclassOf<ABullet> bulletBlueprint;                    //接收子弹预制体，用于实例化子弹

	UPROPERTY(VisibleAnywhere, Category = "Attack")
	USceneComponent* bulletSpawnPoint;                       //子弹发射点，空组件作为子弹发射位置的参考点
	#pragma endregion

public:
	ASpaceship();                                            //构造函数，用于设置默认属性

protected:
	virtual void BeginPlay() override;                       //游戏开始或生成时调用

	#pragma region Control
	void LookAtCursor();                                     //使飞船朝向鼠标光标位置

	void Move(float);                                        //根据输入实际执行移动
	void HandleVerticalMoveInput(float);                     //处理垂直方向移动输入
	void HandleHorizontalMoveInput(float);                   //处理水平方向移动输入
	#pragma endregion

	#pragma region Attack
	void FireBullet();								         //发射子弹
	#pragma endregion

public:
	virtual void Tick(float) override;                       //逐帧调用的更新函数

	//调用来绑定输入功能
	virtual void SetupPlayerInputComponent(class UInputComponent*) override;
};
