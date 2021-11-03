#include "pch.h"
#include "Wheel.h"


void Wheel::InitializeWheel(DirectX::SimpleMath::Vector3 aLocalPos, DirectX::SimpleMath::Vector3 aWorldPos, bool aIsSteerable, float aRadius, float aWidth, float aMass)
{
    SetLocalPos(aLocalPos);
    SetMass(aMass);
    SetRadius(aRadius);
    SetWidth(aWidth);
    SetWorldPos(aWorldPos);
}