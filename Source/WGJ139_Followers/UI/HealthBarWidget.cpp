// © Master Kenth 2020


#include "HealthBarWidget.h"
#include "Components/ProgressBar.h"


void UHealthBarWidget::SetHealthPercent(float Percent)
{
  if(Bar != nullptr)
  {
    Bar->SetPercent(Percent);
  }
}