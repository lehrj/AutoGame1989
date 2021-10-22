#include "pch.h"
#include "Wheel.h"


void Wheel::InitializeWheel(DirectX::SimpleMath::Vector3 aLocalPos, DirectX::SimpleMath::Vector3 aWorldPos, bool aIsStearable, float aRadius, float aWidth, float aMass)
{
    SetLocalPos(aLocalPos);
    SetRadius(aRadius);
    SetWidth(aWidth);
}