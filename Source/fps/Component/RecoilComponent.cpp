// Fill out your copyright notice in the Description page of Project Settings.

#include "RecoilComponent.h"

#include "../Actors/FpsCharacter.h"
#include "../Weapons/Hands.h"

// Sets default values for this component's properties
URecoilComponent::URecoilComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;
}

// Called when the game starts
void URecoilComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}

void URecoilComponent::Initialize(AFpsCharacter* FpsCharacter)
{
	/*
	���� : ������ �������� ĳ������ ���� �̺�Ʈ�� ���޹޾� �̺�Ʈ�� �´� ���� �ؾ���
	�� ��
	1. �̺�Ʈ ���޹޾Ҵٰ� �����ϰ� � ������ ������� �ۼ�
	2. �ۼ��� ������ �������� �̺�Ʈ �����ϴ� �κ��� ����
	*/

	FpsCharacter->AddObserver(this);
}

// Called every frame
void URecoilComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void URecoilComponent::OnEquipHands(AHands* Hands)
{
	Hands->AddObserver(this);

}

void URecoilComponent::OnUnequipHands(AHands* Hands)
{
	Hands->RemoveObserver(this);
}