// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Spaceship.generated.h"

//ǰ����������������ͷ�ļ����ͱ����ٶ�
class USphereComponent;
class USpringArmComponent;
class UCameraComponent;
class ABullet;

UCLASS()
class SPACESHIPBATTLE_API ASpaceship : public APawn
{
	GENERATED_BODY()

protected:
	#pragma region Components
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
	#pragma endregion

	#pragma region Properties
	//ע��˴���EditAnywhere������VisibleAnywhere��ʹ�ÿ�����ͼ�༭�����޸ĸ���ֵ����
	UPROPERTY(EditAnywhere, Category = "Movement")
	float moveSpeed = 2000.0f;                               //�ɴ��ƶ��ٶ�
	#pragma endregion

	#pragma region Attack
	UPROPERTY(EditAnywhere, Category = "Attack")
	//TSubclassOf���Ƹñ������ܽ����ӵ���������ĸ�ֵ
	TSubclassOf<ABullet> bulletBlueprint;                    //�����ӵ�Ԥ���壬����ʵ�����ӵ�

	UPROPERTY(VisibleAnywhere, Category = "Attack")
	USceneComponent* bulletSpawnPoint;                       //�ӵ�����㣬�������Ϊ�ӵ�����λ�õĲο���
	#pragma endregion

public:
	ASpaceship();                                            //���캯������������Ĭ������

protected:
	virtual void BeginPlay() override;                       //��Ϸ��ʼ������ʱ����

	#pragma region Control
	void LookAtCursor();                                     //ʹ�ɴ����������λ��

	void Move(float);                                        //��������ʵ��ִ���ƶ�
	void HandleVerticalMoveInput(float);                     //����ֱ�����ƶ�����
	void HandleHorizontalMoveInput(float);                   //����ˮƽ�����ƶ�����
	#pragma endregion

	#pragma region Attack
	void FireBullet();								         //�����ӵ�
	#pragma endregion

public:
	virtual void Tick(float) override;                       //��֡���õĸ��º���

	//�����������빦��
	virtual void SetupPlayerInputComponent(class UInputComponent*) override;
};
