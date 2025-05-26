// Fill out your copyright notice in the Description page of Project Settings.

#include "Bullet.h"

ABullet::ABullet()
{
	PrimaryActorTick.bCanEverTick = true;

	#pragma region Components
	bulletRootEmpty = CreateDefaultSubobject<USceneComponent>(TEXT("BulletRootEmpty"));
	RootComponent = bulletRootEmpty;

	bulletStaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BulletStaticMesh"));
	bulletStaticMesh->SetupAttachment(RootComponent);
	#pragma endregion
}

void ABullet::BeginPlay()
{
	Super::BeginPlay();	
}

void ABullet::Tick(float _delta)
{
	Super::Tick(_delta);
}