#include "pch.h"
#include "Wheel.h"


void Wheel::InitializeWheel(DirectX::SimpleMath::Vector3 aLocalPos, bool aIsStearable, double aRadius, double aWidth, double aMass)
{
    SetLocalPos(aLocalPos);
    SetRadius(aRadius);
    SetWidth(aWidth);
}