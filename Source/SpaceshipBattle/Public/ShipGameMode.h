// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ShipGameMode.generated.h"


//����BP_GameMode��������ProjectSettings��ָ������Ϸģʽ���н��丸���Ϊ��ǰ��ShipGameMode����ʹ�÷����ȹ���
UCLASS()
class SPACESHIPBATTLE_API AShipGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:
	//ʹ����ͼ�пɷ���
	UPROPERTY(BlueprintReadOnly)
	int score = 0;                //��Ϸ����������ɱ���˵�����

public:
	void IncreaseScore();
};