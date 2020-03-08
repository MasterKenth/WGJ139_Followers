// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthBarWidget.h"
#include "Components/ProgressBar.h"


void UHealthBarWidget::SetHealthPercent(float Percent)
{
  if(Bar != nullptr)
  {
    Bar->SetPercent(Percent);
  }
}