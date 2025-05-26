// Fill out your copyright notice in the Description page of Project Settings.

#include "Spaceship.h"
//引入头文件，而不是前向声明，以调用相关函数和属性
//#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"

ASpaceship::ASpaceship()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	#pragma region Components
	//创建球形碰撞组件并将其设置为根组件，每个游戏对象都需要有唯一根组件，若不显示指定则默认将第一个创建的组件作为根组件
	SpaceshipStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpaceshipStaticMesh"));
	RootComponent = SpaceshipStaticMesh;
	
	//实际上StaticMesh也可进行碰撞检测，但我们不需要那么精细的碰撞检测运算，所以使用球形碰撞组件
	//但要记得在该类衍生的蓝图类中将StaticMesh的CollisionPresets设置为NoCollision
	SpaceshipCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SpaceshipCollision"));
	//将球形碰撞组件绑定到根组件上
	SpaceshipCollision->SetupAttachment(RootComponent);

	//创建默认摄像机组件并将其绑定到根组件上
	SpaceshipCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("SpaceshipCamera"));
	SpaceshipCamera->SetupAttachment(RootComponent);
	#pragma endregion
}

void ASpaceship::BeginPlay()
{
	Super::BeginPlay();
	
}

void ASpaceship::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASpaceship::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

