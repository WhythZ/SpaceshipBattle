// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ShipGameMode.generated.h"


//需在BP_GameMode（我们在ProjectSettings中指定的游戏模式）中将其父类改为当前的ShipGameMode，以使用分数等功能
UCLASS()
class SPACESHIPBATTLE_API AShipGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	//使得蓝图中可访问
	UPROPERTY(BlueprintReadOnly)
	int score = 0;                //游戏分数，即击杀敌人的数量

public:
	void IncreaseScore();
};