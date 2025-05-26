// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Spaceship.generated.h"

//����Ҳ������ͷ�ļ������˴�ǰ�����������������ٶ�
class USphereComponent;
class USpringArmComponent;
class UCameraComponent;

UCLASS()
class SPACESHIPBATTLE_API ASpaceship : public APawn
{
	GENERATED_BODY()

protected:
	//��VisibleAnywhereʹ�������UE��ͼ�༭���пɼ����������˵��Ҳ���Ը��ģ�������Category�������
	//����������������int�ȣ�VisibleAnywhere��ʹ��ɼ������ɸ��ģ�������EditableAnywhere
	UPROPERTY(VisibleAnywhere, Category="Component")
	USphereComponent* spaceshipCollision;                    //���ڴ���ɴ���ײ��������ײ���
	
	UPROPERTY(VisibleAnywhere, Category = "Component")
	UStaticMeshComponent* spaceshipStaticMesh;               //������ʾ�ɴ��ľ�̬�������

	UPROPERTY(VisibleAnywhere, Category = "Component")
	USpringArmComponent* spaceshipSpringArm;                 //���ɱ���������ڿ����������ɴ���Ĺ�ϵ

	UPROPERTY(VisibleAnywhere, Category = "Component")
	UCameraComponent* spaceshipCamera;				         //����ɴ����ӽ���������

	APlayerController* spaceshipPlayerController;            //��ҿ����������ڻ�ȡ�������

public:
	ASpaceship();                                            //���캯������������Ĭ������

protected:
	virtual void BeginPlay() override;                       //��Ϸ��ʼ������ʱ����

	void LookAtCursor();                                     //ʹ�ɴ����������λ��

public:
	virtual void Tick(float DeltaTime) override;             //��֡���õĸ��º���

	//�����������빦��
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
};
