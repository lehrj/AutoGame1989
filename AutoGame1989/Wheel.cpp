#include "pch.h"
#include "Wheel.h"


void Wheel::InitializeWheel(DirectX::SimpleMath::Vector3 aLocalPos, DirectX::SimpleMath::Vector3 aWorldPos, bool aIsSteerable, float aRadius, float aWidth, float aMass)
{  
    SetLocalPos(aLocalPos);
    SetMass(aMass);
    SetRadius(aRadius);
    SetSteerable(aIsSteerable);
    SetWidth(aWidth);
    SetWorldPos(aWorldPos);

    // hacking in data whlie hooking everything up for testing, undo this once equations are sorted out and what data is needed
    SetAngularVelocity(0.0f);   
    SetForward(DirectX::SimpleMath::Vector3::UnitX);
    SetTerrainNormal(DirectX::SimpleMath::Vector3::UnitY);
    SetUp(DirectX::SimpleMath::Vector3::UnitY);
    SetWidth(0.235f);
}