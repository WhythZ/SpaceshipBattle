// Fill out your copyright notice in the Description page of Project Settings.

#include "EnemyshipSpawner.h"

#include "Components/BoxComponent.h"
#include "Enemyship.h"

AEnemyshipSpawner::AEnemyshipSpawner()
{
	PrimaryActorTick.bCanEverTick = true;

	spawnArea = CreateDefaultSubobject<UBoxComponent>(TEXT("SpawnArea"));
	RootComponent = spawnArea;
}

void AEnemyshipSpawner::BeginPlay()
{
	Super::BeginPlay();
}

void AEnemyshipSpawner::Tick(float _delta)
{
	Super::Tick(_delta);
}