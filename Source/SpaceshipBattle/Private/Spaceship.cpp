// Fill out your copyright notice in the Description page of Project Settings.

#include "Spaceship.h"

//����������ͷ�ļ�����������Spaceship.h��ʹ��ǰ���������Ե�����غ��������ԣ�������ͷ�ļ��еĶ�����뿪��
//#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"

//������ѧ��������ѧ����
#include "Kismet/KismetMathLibrary.h"
//����ʵ�����ӵ��ȹ���
#include "Engine/World.h"
//�����ʱ�������ӵ���������
#include "TimerManager.h"

//�����ӵ�ͷ�ļ�
#include "Bullet.h"

ASpaceship::ASpaceship()
{
 	//ʹ�ø�Pawn��ÿ֡����Tick()����
	PrimaryActorTick.bCanEverTick = true;

	#pragma region Components
	//ʵ����StaticMeshҲ�ɽ�����ײ��⣬�����ǲ���Ҫ��ô��ϸ����ײ������㣬����ʹ��������ײ���
	//��Ҫ�ǵ��ڸ�����������ͼ���н�StaticMesh��CollisionPresets����ΪNoCollision
	spaceshipCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SpaceshipCollision"));
	//��������Ϊ�������ÿ����Ϸ������Ҫ��Ψһ�������������ʽָ����Ĭ�Ͻ���һ�������������Ϊ�������
	RootComponent = spaceshipCollision;

	//������ά�������������󶨵��������
	spaceshipStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpaceshipStaticMesh"));
	spaceshipStaticMesh->SetupAttachment(RootComponent);

	//�������������ĵ��ɱ����������󶨵�������ϣ��������ʹ����Ϊ�ɴ�������������������ɴ���ת
	spaceshipSpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpaceshipSpringArm"));
	spaceshipSpringArm->SetupAttachment(RootComponent);

	//����Ĭ����������������󶨵��������
	spaceshipCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("SpaceshipCamera"));
	//��������󶨵����ɱ�����϶�����ֱ�Ӱ󵽸����
	spaceshipCamera->SetupAttachment(spaceshipSpringArm);

	bulletSpawnPoint = CreateDefaultSubobject<USceneComponent>(TEXT("BulletSpawnPoint"));
	bulletSpawnPoint->SetupAttachment(RootComponent);
	#pragma endregion
}

void ASpaceship::BeginPlay()
{
	Super::BeginPlay();
	
	#pragma region Components
	//��ȡ��ҿ�������ǿתΪAPlayerController����
	//д��BeginPlay()�����ǹ��캯���У���Ϊ��������ڸö�����ù��캯��ʱ���ܻ�δ�������
	spaceshipPlayerController = Cast<APlayerController>(GetController());
	//��ʾ�����
	spaceshipPlayerController->bShowMouseCursor = true;
	#pragma endregion
}

void ASpaceship::LookAtCursor()
{
	//��ȡ����꣨2D��Ļ���꣩������ռ��ж�Ӧ��λ�ã�3D�������꣩�ͷ��򣨴������ָ����λ�õ��������˴����ã�
	FVector _cursorLocation, _cursorDirection;
	spaceshipPlayerController->DeprojectMousePositionToWorld(_cursorLocation, _cursorDirection);
	//���ڱ���Ϸ��2D���ӽǣ���������Z����ת������ԭ������
	FVector _targetLocation2D = FVector(_cursorLocation.X, _cursorLocation.Y, GetActorLocation().Z);

	//����ӷɴ���ǰλ�ã������䵱ǰ���򣩿�����Ŀ��λ���������ת�Ƕ�
	FRotator _rotationAngle = UKismetMathLibrary::FindLookAtRotation(
		GetActorLocation(), //��ʼλ�ã��ɴ�λ�ã�
		_targetLocation2D   //Ŀ��λ�ã����λ�ã�
	);

	//ʵ��Ӧ����ת�Ƕȵ��ɴ��ϣ�ʹ�䳯�������λ��
	SetActorRotation(_rotationAngle);
}

void ASpaceship::Move(float _delta)
{
	//�����������ķ��������ƶ��ɴ����ڶ�����Ϊtrue��ʾ�ᱻ������ײ���赲
	AddActorWorldOffset(ConsumeMovementInputVector() * moveSpeed * _delta, true);
	//AddActorWorldOffset(ConsumeMovementInputVector() * moveSpeed * FApp::GetDeltaTime(), true);
}

void ASpaceship::HandleVerticalMoveInput(float _value)
{
	//_valueΪ1��ʾ���ϣ�-1��ʾ���£�0��ʾ���ƶ�
	//ע��UpVector����(0,0,1)��Z���ϵķ��򣬶�ForwardVector����(1,0,0)��X���ϵķ���
	AddMovementInput(FVector::ForwardVector, _value);
	//AddMovementInput(FVector(1, 0, 0), _value);
}

void ASpaceship::HandleHorizontalMoveInput(float _value)
{
	//_valueΪ1��ʾ���ң�-1��ʾ����0��ʾ���ƶ�
	//עRightVector����(0,1,0)��Y���ϵķ���
	AddMovementInput(FVector::RightVector, _value);
	//AddMovementInput(FVector(0, 1, 0), _value);
}

void ASpaceship::FireBullet()
{
	//��ֹ�ӵ�Ԥ����δ�����ã����µ��ô˺������±���
	if (bulletBlueprint)
	{
		//����bulletSpawnPoint��λ�ú���ת����bulletBlueprint���͵��ӵ�
		FActorSpawnParameters _spawnParams;
		GetWorld()->SpawnActor<ABullet>(bulletBlueprint,
			bulletSpawnPoint->GetComponentLocation(),
			bulletSpawnPoint->GetComponentRotation(),_spawnParams);
	}
}

void ASpaceship::StartFireBullet()
{
	//����fireBulletTimerHandle��ʱ��
	GetWorldTimerManager().SetTimer(
		fireBulletTimerHandle,   //��ʱ�����
		this,                    //���õĶ���
		&ASpaceship::FireBullet, //���õĺ���
		fireBulletInterval,      //��ʱ�����ʱ��
		true,                    //�Ƿ�ѭ������
		0.0f                     //��ʼ�ӳ�ʱ�䣬0.0f��ʾ������ʼ
	);
}

void ASpaceship::EndFireBullet()
{
	//�ر�fireBulletTimerHandle��ʱ��
	GetWorldTimerManager().ClearTimer(fireBulletTimerHandle);
}

void ASpaceship::Tick(float _delta)
{
	Super::Tick(_delta);

	#pragma region Control
	//ÿ֡����LookAtCursor������ʹ�ɴ��������������λ��
	LookAtCursor();
	//ÿ֡����Move�����������������ʵ���ƶ��ɴ�
	Move(_delta);
	#pragma endregion
}

void ASpaceship::SetupPlayerInputComponent(UInputComponent* _playerInputComponent)
{
	Super::SetupPlayerInputComponent(_playerInputComponent);

	#pragma region Movement
	//�˴�������󶨣��������������룩��������Ϊ��BindAction�������ε����룬����������Ծ���룩
	//�󶨴�ֱ��ˮƽ������ƶ����룬�˴�����ĺ���ǩ���������һ��float����
	//�˴�������ַ�����Ҫ����Ŀ���õ����벿���ж��壨Edit-> ProjectSettings->Input��
	_playerInputComponent->BindAxis("MoveVertical", this, &ASpaceship::HandleVerticalMoveInput);
	_playerInputComponent->BindAxis("MoveHorizontal", this, &ASpaceship::HandleHorizontalMoveInput);
	#pragma endregion

	#pragma region Attack
	//�˴�����Ϊ�󶨣�IE_Pressed��ʾ���¶����ǰ�ס��IE_Released��ʾ�ɿ�������ʱ����ô���Ķ�Ӧ����
	//_playerInputComponent->BindAction("FireBullet", IE_Pressed, this, &ASpaceship::FireBullet);
	_playerInputComponent->BindAction("StartFireBullet", IE_Pressed, this, &ASpaceship::StartFireBullet);
	_playerInputComponent->BindAction("EndFireBullet", IE_Released, this, &ASpaceship::EndFireBullet);
	#pragma endregion
}