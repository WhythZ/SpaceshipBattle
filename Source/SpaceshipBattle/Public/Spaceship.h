// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Spaceship.generated.h"

//ǰ����������������ͷ�ļ����ͱ����ٶ�
class USphereComponent;
class USpringArmComponent;
class UCameraComponent;
class USoundCue;
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

	#pragma region Movement
	//ע��˴���EditAnywhere������VisibleAnywhere��ʹ�ÿ�����ͼ�༭�����޸ĸ���ֵ����
	UPROPERTY(EditAnywhere, Category = "Movement")
	float moveSpeed = 2000.0f;                               //�ɴ��ƶ��ٶ�
	#pragma endregion

	#pragma region Attack
	//�Զ�������Ҳ��Ҫʹ��EditAnywhere
	UPROPERTY(EditAnywhere, Category = "Attack")
	//TSubclassOf���Ƹñ������ܽ����ӵ���������ĸ�ֵ
	TSubclassOf<ABullet> bulletBlueprint;                    //�����ӵ�Ԥ���壬����ʵ�����ӵ�

	UPROPERTY(VisibleAnywhere, Category = "Attack")
	USceneComponent* bulletSpawnPoint;                       //�ӵ�����㣬�������Ϊ�ӵ�����λ�õĲο���

	FTimerHandle fireBulletTimerHandle;                      //��ʱ����������ڿ����ӵ����������Ƶ��

	UPROPERTY(EditAnywhere, Category = "Attack")
	double fireBulletInterval = 0.5f;                        //�ӵ���������ļ��ʱ�䣬��λΪ��
	#pragma endregion

	#pragma region Restart
	FTimerHandle restartTimerHandle;                         //���ڶ�ʱ�����ļ�ʱ�����
	
	UPROPERTY(EditAnywhere, Category = "Restart")
	double restartCooldown = 2.0f;                           //�����󾭹����������Ϸ
	#pragma endregion

	#pragma region Sounds
	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundCue* fireBulletSFX;
	UPROPERTY(EditAnywhere, Category = "Sounds")
	USoundCue* gameOverSFX;
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
	void FireBullet();								         //���η����ӵ�
	void StartFireBullet();							         //��ʼ�����ӵ�����ס��������
	void EndFireBullet();							         //���������ӵ����ɿ���������
	#pragma endregion

	#pragma region Restart
	void OnDeath();                                          //�������ʱ���õĺ��������ڴ��������߼�
	void RestartLevel();                                     //���������������������Ϸ
	#pragma endregion

	bool isAlive = true;                                     //����Ƿ���ı�־λ

public:
	virtual void Tick(float) override;                       //��֡���õĸ��º���

	//�����������빦��
	virtual void SetupPlayerInputComponent(class UInputComponent*) override;

	virtual void NotifyActorBeginOverlap(AActor*) override;  //��д��ײ��⺯��

	bool IsAlive() const;
};
