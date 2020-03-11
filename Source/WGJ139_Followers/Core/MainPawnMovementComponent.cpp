// © Master Kenth 2020


#include "MainPawnMovementComponent.h"

DEFINE_LOG_CATEGORY(LogMainPawnMovement);

UMainPawnMovementComponent::UMainPawnMovementComponent()
{
  BaseMoveSpeed = 100.0f;

  SetPlaneConstraintEnabled(true);
  bSnapToPlaneAtStart = true;
  SetPlaneConstraintOrigin(FVector::ZeroVector);
  SetPlaneConstraintNormal(FVector::UpVector);
}

void UMainPawnMovementComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
  Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

  FVector input = ConsumeInputVector();
  if(!input.IsNearlyZero())
  {
    FVector accel = BaseMoveSpeed * input * DeltaTime;
    FHitResult hit;
    MoveUpdatedComponent(accel, UpdatedComponent->RelativeRotation, true, &hit);
    UpdateComponentVelocity();
  }
}

void UMainPawnMovementComponent::AddInputVector(FVector WorldVector, bool bForce)
{
  Super::AddInputVector(WorldVector, bForce);
}