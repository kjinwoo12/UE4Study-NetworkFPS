// Fill out your copyright notice in the Description page of Project Settings.


#include "LoginWidget.h"

bool ULoginWidget::IsIpExpression(FString str) 
{
	FRegexPattern ipPattern(FString(TEXT("^(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)(\\.(25[0-5]|2[0-4]\\d|1\\d\\d|[1-9]?\\d)){3}$")));
	FRegexMatcher ipRegexMatcher(ipPattern, str);
	if (ipRegexMatcher.FindNext()) return true;
	else return false;
}