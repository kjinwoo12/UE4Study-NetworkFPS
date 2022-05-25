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
	목적 : 옵저버 패턴으로 캐릭터의 각종 이벤트를 전달받아 이벤트에 맞는 일을 해야함
	할 일
	1. 이벤트 전달받았다고 가정하고 어떤 식으로 사용할지 작성
	2. 작성된 내용을 바탕으로 이벤트 전달하는 부분을 구현
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