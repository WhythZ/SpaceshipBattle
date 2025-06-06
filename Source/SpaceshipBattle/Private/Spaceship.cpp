// Fill out your copyright notice in the Description page of Project Settings.

#include "Spaceship.h"

//引入组件相关头文件，而不是像Spaceship.h中使用前向声明，以调用相关函数和属性，并避免头文件中的额外编译开销
//#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"
#include "Particles/ParticleSystemComponent.h"
#include "Particles/ParticleSystem.h"

//引入数学库用于数学运算
#include "Kismet/KismetMathLibrary.h"
//用于实例化子弹等功能
#include "Engine/World.h"
//引入计时器用于子弹连续发射
#include "TimerManager.h"
//用于重启关卡、播放音效等功能
#include "Kismet/GameplayStatics.h"
#include "Sound/SoundCue.h"

//引入子弹头文件
#include "Bullet.h"
#include "Enemyship.h"

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

	thrusterParticleSystemComp = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ThrusterParticleSystemComp"));
	thrusterParticleSystemComp->SetupAttachment(RootComponent);
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
	//死亡时不执行该函数
	if (!bAlive) return;

	if (_value != 0.0f) bMovingVertical = true;
	else bMovingVertical = false;

	//_value为1表示向上，-1表示向下，0表示不移动
	//注意UpVector代表(0,0,1)是Z轴上的方向，而ForwardVector代表(1,0,0)是X轴上的方向
	AddMovementInput(FVector::ForwardVector, _value);
	//AddMovementInput(FVector(1, 0, 0), _value);
}

void ASpaceship::HandleHorizontalMoveInput(float _value)
{
	//死亡时不执行该函数
	if (!bAlive) return;

	if (_value != 0.0f) bMovingHorizontal = true;
	else bMovingHorizontal = false;

	//_value为1表示向右，-1表示向左，0表示不移动
	//注RightVector代表(0,1,0)是Y轴上的方向
	AddMovementInput(FVector::RightVector, _value);
	//AddMovementInput(FVector(0, 1, 0), _value);
}

void ASpaceship::FireBullet()
{
	//死亡时不执行该函数
	if (!bAlive) return;

	//防止子弹预制体未被设置，导致调用此函数导致崩溃
	if (bulletBlueprint)
	{
		//根据bulletSpawnPoint的位置和旋转生成bulletBlueprint类型的子弹
		FActorSpawnParameters _spawnParams;
		GetWorld()->SpawnActor<ABullet>(bulletBlueprint,
			bulletSpawnPoint->GetComponentLocation(),
			bulletSpawnPoint->GetComponentRotation(),_spawnParams);
	}

	//播放音效
	if (fireBulletSFX)
		UGameplayStatics::PlaySoundAtLocation(this, fireBulletSFX, GetActorLocation());
}

void ASpaceship::StartFireBullet()
{
	//死亡时不执行该函数
	if (!bAlive) return;

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

void ASpaceship::OnDeath()
{
	//UE_LOG(LogTemp, Warning, TEXT("ASpaceship::OnDeath()"));

	//不能直接销毁玩家，会导致崩溃
	//Destroy();
	//将StaticMesh的可见性设置为不可见，隐藏飞船
	spaceshipStaticMesh->SetVisibility(false);
	//将玩家存活状态设置为false
	bAlive = false;

	//开启计时器
	GetWorldTimerManager().SetTimer(
		restartTimerHandle,         //计时器句柄
		this,                       //调用的对象
		&ASpaceship::RestartLevel,  //调用的函数
		restartCooldown,            //计时器间隔时间
		false                       //此处无需循环调用
	);

	//播放音效
	if (gameOverSFX)
		UGameplayStatics::PlaySoundAtLocation(this, gameOverSFX, GetActorLocation());

	//死亡时的爆炸粒子效果
	if (explosionParticleSystem)
	{
		UGameplayStatics::SpawnEmitterAtLocation(
			this,
			explosionParticleSystem,  //爆炸效果粒子系统
			GetActorLocation(),       //在飞船位置生成粒子效果
			FRotator::ZeroRotator,    //旋转角度
			FVector(1.0f),            //缩放比例
			true                      //是否自动销毁
		);
	}
}

void ASpaceship::RestartLevel()
{
	//#pragma region ResetPlayer
	////重置玩家存活状态
	//bAlive = true;
	////恢复可见性
	//spaceshipStaticMesh->SetVisibility(true);
	////重置玩家位置与旋转角度
	//SetActorLocation(FVector(0, 0, 100));
	//SetActorRotation(FRotator::ZeroRotator);
	//#pragma endregion

	//#pragma region ResetEnemy
	////清空场上敌人
	//TArray<AActor*> _enemyships;
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemyship::StaticClass(), _enemyships);
	//for (AActor* _enemyship : _enemyships)
	//	_enemyship->Destroy();
	//#pragma endregion

	//重启当前关卡，不知为何一旦使用OpenLevel函数，虽能编译成功，但一启动游戏就会阻塞、玩家无法移动、敌人不生成
	//因为本函数名为Restart（后面才改为RestartLevel），可能与某个系统函数冲突了，导致每次新场景加载时就会调用一次本函数
	//而调用该函数又会导致场景重新加载，继而无限重新加载，导致死循环
	UGameplayStatics::OpenLevel(this, "MainMap");
	//UGameplayStatics::OpenLevel(this, FName(*GetWorld()->GetName()));
}

void ASpaceship::UpdateParticleSystem()
{
	if (!bAlive)
	{
		thrusterParticleSystemComp->Deactivate();
		return;
	}

	if (bMovingVertical || bMovingHorizontal)
		thrusterParticleSystemComp->Activate();
	else
		thrusterParticleSystemComp->Deactivate();
}

void ASpaceship::Tick(float _delta)
{
	Super::Tick(_delta);

	//若玩家已死亡则不执行
	if (bAlive)
	{
		#pragma region Control
		//每帧调用LookAtCursor函数，使飞船持续朝向鼠标光标位置
		LookAtCursor();
		//每帧调用Move函数，根据玩家输入实际移动飞船
		Move(_delta);
		#pragma endregion

		//粒子系统更新
		UpdateParticleSystem();
	}
}

void ASpaceship::SetupPlayerInputComponent(UInputComponent* _playerInputComponent)
{
	Super::SetupPlayerInputComponent(_playerInputComponent);
	
	//这些输入检测并不是动态更新的，而是在开始时进行绑定的，所以在此处执行下述语句是无意义的
	//if (!bAlive) return;

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

void ASpaceship::NotifyActorBeginOverlap(AActor* _otherActor)
{
	//由于是重写，故先调用父类方法
	Super::NotifyActorBeginOverlap(_otherActor);

	//玩家碰到敌人就死掉
	AEnemyship* _enemyship = Cast<AEnemyship>(_otherActor);
	if (_enemyship)
	{
		//销毁敌人飞船
		_enemyship->Destroy();

		OnDeath();
	}
}

bool ASpaceship::IsAlive() const
{
	return bAlive;
}
