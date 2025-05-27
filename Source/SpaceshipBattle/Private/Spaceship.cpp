// Fill out your copyright notice in the Description page of Project Settings.

#include "Spaceship.h"

//引入组件相关头文件，而不是像Spaceship.h中使用前向声明，以调用相关函数和属性，并避免头文件中的额外编译开销
//#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"

//引入数学库用于数学运算
#include "Kismet/KismetMathLibrary.h"
//用于实例化子弹等功能
#include "Engine/World.h"
//引入计时器用于子弹连续发射
#include "TimerManager.h"

//引入子弹头文件
#include "Bullet.h"

ASpaceship::ASpaceship()
{
 	//使得该Pawn在每帧调用Tick()函数
	PrimaryActorTick.bCanEverTick = true;

	#pragma region Components
	//实际上StaticMesh也可进行碰撞检测，但我们不需要那么精细的碰撞检测运算，所以使用球形碰撞组件
	//但要记得在该类衍生的蓝图类中将StaticMesh的CollisionPresets设置为NoCollision
	spaceshipCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SpaceshipCollision"));
	//主动设置为根组件（每个游戏对象都需要有唯一根组件，若不显式指定则默认将第一个创建的组件作为根组件）
	RootComponent = spaceshipCollision;

	//创建三维网格组件并将其绑定到根组件上
	spaceshipStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpaceshipStaticMesh"));
	spaceshipStaticMesh->SetupAttachment(RootComponent);

	//创建摄像机所需的弹簧臂组件并将其绑定到根组件上，该组件可使得作为飞船子组件的摄像机不跟随飞船旋转
	spaceshipSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpaceshipSpringArm"));
	spaceshipSpringArm->SetupAttachment(RootComponent);

	//创建默认摄像机组件并将其绑定到根组件上
	spaceshipCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("SpaceshipCamera"));
	//将摄像机绑定到弹簧臂组件上而不是直接绑到根组件
	spaceshipCamera->SetupAttachment(spaceshipSpringArm);

	bulletSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("BulletSpawnPoint"));
	bulletSpawnPoint->SetupAttachment(RootComponent);
	#pragma endregion
}

void ASpaceship::BeginPlay()
{
	Super::BeginPlay();
	
	#pragma region Components
	//获取玩家控制器，强转为APlayerController类型
	//写在BeginPlay()而不是构造函数中，因为其它组件在该对象调用构造函数时可能还未创建完成
	spaceshipPlayerController = Cast<APlayerController>(GetController());
	//显示鼠标光标
	spaceshipPlayerController->bShowMouseCursor = true;
	#pragma endregion
}

void ASpaceship::LookAtCursor()
{
	//获取鼠标光标（2D屏幕坐标）在世界空间中对应的位置（3D世界坐标）和方向（从摄像机指向光标位置的向量，此处无用）
	FVector _cursorLocation, _cursorDirection;
	spaceshipPlayerController->DeprojectMousePositionToWorld(_cursorLocation, _cursorDirection);
	//由于本游戏是2D的视角，所以无需Z轴旋转，保持原样即可
	FVector _targetLocation2D = FVector(_cursorLocation.X, _cursorLocation.Y, GetActorLocation().Z);

	//计算从飞船当前位置（根据其当前朝向）看向光标目标位置所需的旋转角度
	FRotator _rotationAngle = UKismetMathLibrary::FindLookAtRotation(
		GetActorLocation(), //起始位置（飞船位置）
		_targetLocation2D   //目标位置（光标位置）
	);

	//实际应用旋转角度到飞船上，使其朝向鼠标光标位置
	SetActorRotation(_rotationAngle);
}

void ASpaceship::Move(float _delta)
{
	//根据玩家输入的方向向量移动飞船，第二参数为true表示会被其它碰撞体阻挡
	AddActorWorldOffset(ConsumeMovementInputVector() * moveSpeed * _delta, true);
	//AddActorWorldOffset(ConsumeMovementInputVector() * moveSpeed * FApp::GetDeltaTime(), true);
}

void ASpaceship::HandleVerticalMoveInput(float _value)
{
	//_value为1表示向上，-1表示向下，0表示不移动
	//注意UpVector代表(0,0,1)是Z轴上的方向，而ForwardVector代表(1,0,0)是X轴上的方向
	AddMovementInput(FVector::ForwardVector, _value);
	//AddMovementInput(FVector(1, 0, 0), _value);
}

void ASpaceship::HandleHorizontalMoveInput(float _value)
{
	//_value为1表示向右，-1表示向左，0表示不移动
	//注RightVector代表(0,1,0)是Y轴上的方向
	AddMovementInput(FVector::RightVector, _value);
	//AddMovementInput(FVector(0, 1, 0), _value);
}

void ASpaceship::FireBullet()
{
	//防止子弹预制体未被设置，导致调用此函数导致崩溃
	if (bulletBlueprint)
	{
		//根据bulletSpawnPoint的位置和旋转生成bulletBlueprint类型的子弹
		FActorSpawnParameters _spawnParams;
		GetWorld()->SpawnActor<ABullet>(bulletBlueprint,
			bulletSpawnPoint->GetComponentLocation(),
			bulletSpawnPoint->GetComponentRotation(),_spawnParams);
	}
}

void ASpaceship::StartFireBullet()
{
	//开启fireBulletTimerHandle计时器
	GetWorldTimerManager().SetTimer(
		fireBulletTimerHandle,   //计时器句柄
		this,                    //调用的对象
		&ASpaceship::FireBullet, //调用的函数
		fireBulletInterval,      //计时器间隔时间
		true,                    //是否循环调用
		0.0f                     //初始延迟时间，0.0f表示立即开始
	);
}

void ASpaceship::EndFireBullet()
{
	//关闭fireBulletTimerHandle计时器
	GetWorldTimerManager().ClearTimer(fireBulletTimerHandle);
}

void ASpaceship::Tick(float _delta)
{
	Super::Tick(_delta);

	#pragma region Control
	//每帧调用LookAtCursor函数，使飞船持续朝向鼠标光标位置
	LookAtCursor();
	//每帧调用Move函数，根据玩家输入实际移动飞船
	Move(_delta);
	#pragma endregion
}

void ASpaceship::SetupPlayerInputComponent(UInputComponent* _playerInputComponent)
{
	Super::SetupPlayerInputComponent(_playerInputComponent);

	#pragma region Movement
	//此处采用轴绑定（处理连续的输入）而不是行为绑定BindAction（处理单次的输入，例如用于跳跃输入）
	//绑定垂直和水平方向的移动输入，此处传入的函数签名必须接收一个float参数
	//此处传入的字符串需要在项目设置的输入部分中定义（Edit-> ProjectSettings->Input）
	_playerInputComponent->BindAxis("MoveVertical", this, &ASpaceship::HandleVerticalMoveInput);
	_playerInputComponent->BindAxis("MoveHorizontal", this, &ASpaceship::HandleHorizontalMoveInput);
	#pragma endregion

	#pragma region Attack
	//此处是行为绑定，IE_Pressed表示按下而不是按住，IE_Released表示松开，触发时会调用传入的对应函数
	//_playerInputComponent->BindAction("FireBullet", IE_Pressed, this, &ASpaceship::FireBullet);
	_playerInputComponent->BindAction("StartFireBullet", IE_Pressed, this, &ASpaceship::StartFireBullet);
	_playerInputComponent->BindAction("EndFireBullet", IE_Released, this, &ASpaceship::EndFireBullet);
	#pragma endregion
}