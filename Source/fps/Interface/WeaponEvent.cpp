// Fill out your copyright notice in the Description page of Project Settings.


#include "WeaponEvent.h"
#include "../Weapons/WeaponBase.h"

// Add default functionality here for any IWeaponEvent functions that are not pure virtual.
void IWeaponEvent::OnActionEvent(AWeaponBase* WeaponBase)
{
}

void IWeaponEvent::OnSubactionEvent(AWeaponBase* WeaponBase)
{
}

void IWeaponEvent::OnReloadEvent(AWeaponBase* WeaponBase)
{
}