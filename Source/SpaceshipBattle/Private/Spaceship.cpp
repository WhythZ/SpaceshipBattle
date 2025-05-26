// Fill out your copyright notice in the Description page of Project Settings.

#include "Spaceship.h"

//������ѧ��������ѧ����
#include "Kismet/KismetMathLibrary.h"

//����������ͷ�ļ���������ǰ���������Ե�����غ���������
//#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/PlayerController.h"

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

void ASpaceship::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	//ÿ֡����LookAtCursor������ʹ�ɴ��������������λ��
	LookAtCursor();
}

void ASpaceship::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}