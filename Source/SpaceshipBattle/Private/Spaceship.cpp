// Fill out your copyright notice in the Description page of Project Settings.

#include "Spaceship.h"
//����ͷ�ļ���������ǰ���������Ե�����غ���������
//#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "Camera/CameraComponent.h"

ASpaceship::ASpaceship()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	#pragma region Components
	//����������ײ�������������Ϊ�������ÿ����Ϸ������Ҫ��Ψһ�������������ʾָ����Ĭ�Ͻ���һ�������������Ϊ�����
	SpaceshipStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SpaceshipStaticMesh"));
	RootComponent = SpaceshipStaticMesh;
	
	//ʵ����StaticMeshҲ�ɽ�����ײ��⣬�����ǲ���Ҫ��ô��ϸ����ײ������㣬����ʹ��������ײ���
	//��Ҫ�ǵ��ڸ�����������ͼ���н�StaticMesh��CollisionPresets����ΪNoCollision
	SpaceshipCollision = CreateDefaultSubobject<USphereComponent>(TEXT("SpaceshipCollision"));
	//��������ײ����󶨵��������
	SpaceshipCollision->SetupAttachment(RootComponent);

	//����Ĭ����������������󶨵��������
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

