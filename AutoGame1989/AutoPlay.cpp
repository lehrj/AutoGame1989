#include "pch.h"
#include "AutoPlay.h"



AutoPlay::AutoPlay()
{
    ResetPlayData();
   
}

void AutoPlay::BuildDebugData()
{
    m_debugData.clear();

}


std::vector<std::string> AutoPlay::GetDebugData()
{
    BuildDebugData();
    return m_debugData;
};

void AutoPlay::ResetPlayData()
{
    Utility::ZeroImpactData(m_impactData);
    m_isSwingPowerSet = false;
    m_isSwingStart = false;
    m_isOnDownSwing = false;
    m_isSwingUpdateReady = false;
    m_chunkRate = 0.0;
    m_hookRate = 0.0;
    m_meterBar = 0.0;
    m_swingImpact = 0.0;
    m_swingImpactProcessed = 0.0;
    m_swingIncrement = m_defaultSwingIncrementSpeed;
    m_swingPower = 0.0;
    m_skullRate = 0.0;
    m_sliceRate = 0.0;
}


void AutoPlay::SetAim(DirectX::SimpleMath::Vector3 aShotPos, DirectX::SimpleMath::Vector3 aTargetPos)
{
    DirectX::SimpleMath::Vector3 lineToHole = aTargetPos - aShotPos;

    lineToHole.Normalize();
    DirectX::SimpleMath::Vector3 unitX = DirectX::SimpleMath::Vector3::UnitX;

    double angle = acos(lineToHole.Dot(unitX));
    angle = Utility::WrapAngle(angle);

    if (aShotPos.z < aTargetPos.z)
    {
        angle = -angle;
    }

    double angleDegrees = Utility::ToDegrees(angle);
    m_impactData.directionRads = angle;
    m_impactData.directionDegrees = angleDegrees;
}

void AutoPlay::SetPower()
{
    if (m_isSwingStart == true)
    {
        m_impactData.power = m_meterBar;
        m_swingPower = m_meterBar;
        m_isOnDownSwing = true;
        m_isSwingPowerSet = true;
    }
}




