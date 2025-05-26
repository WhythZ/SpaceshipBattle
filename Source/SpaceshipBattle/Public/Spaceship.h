// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Spaceship.generated.h"

//����Ҳ������ͷ�ļ������˴�ǰ�����������������ٶ�
class USphereComponent;
class UCameraComponent;

UCLASS()
class SPACESHIPBATTLE_API ASpaceship : public APawn
{
	GENERATED_BODY()

protected:
	//��VisibleAnywhereʹ�������UE��ͼ�༭���пɼ����������˵��Ҳ���Ը��ģ�������Category�������
	//����������������int�ȣ�VisibleAnywhere��ʹ��ɼ������ɸ��ģ�������EditableAnywhere
	UPROPERTY(VisibleAnywhere, Category="Component")
	USphereComponent* SpaceshipCollision;                    //���ڴ���ɴ���ײ��������ײ���
	
	UPROPERTY(VisibleAnywhere, Category = "Component")
	UStaticMeshComponent* SpaceshipStaticMesh;               //������ʾ�ɴ��ľ�̬�������

	UPROPERTY(VisibleAnywhere, Category = "Component")
	UCameraComponent* SpaceshipCamera;

public:
	ASpaceship();                                            //���캯������������Ĭ������

protected:
	virtual void BeginPlay() override;                       //��Ϸ��ʼ������ʱ����

public:
	virtual void Tick(float DeltaTime) override;             //��֡����

	//�����������빦��
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
