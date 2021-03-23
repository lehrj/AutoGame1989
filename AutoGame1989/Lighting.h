#pragma once

enum class LightingState
{
    LIGHTINGSTATE_JI,
    LIGHTINGSTATE_BMW,
    LIGHTINGSTATE_STARTSCREEN,
    LIGHTINGSTATE_TEASERSCREEN,
    LIGHTINGSTATE_MANUAL,
    LIGHTINGSTATE_NULL,
    LIGHTINGSTATE_STARTUP,
    LIGHTINGSTATE_TEST01,
    LIGHTINGSTATE_,
};


class Lighting
{
public:

    void SetLighting(LightingState aLightState);

    void SetFogVals1(const DirectX::SimpleMath::Vector3 aTargetPos, const float aDimmerVal);
    void SetFogVals2(const DirectX::SimpleMath::Vector3 aTargetPos, const float aDimmerVal);
    void SetFogVals3(const DirectX::SimpleMath::Vector3 aTargetPos, const float aDimmerVal);
    void SetTerrainGridDimmer(const DirectX::SimpleMath::Vector3 aTargetPos, const float aDimmerVal);

    void UpdateLighting();

private:

    LightingState                               m_currentLightingState;

    DirectX::SimpleMath::Vector3                m_cameraFocusPos = DirectX::SimpleMath::Vector3::Zero;
    DirectX::SimpleMath::Vector3                m_lightPos0 = DirectX::SimpleMath::Vector3::UnitX;
    DirectX::SimpleMath::Vector3                m_lightPos1 = DirectX::SimpleMath::Vector3::UnitX;
    DirectX::SimpleMath::Vector3                m_lightPos2 = DirectX::SimpleMath::Vector3::UnitX;

    // light pos for m_effect2
    DirectX::SimpleMath::Vector3                m_lightEffect2Pos0 = DirectX::SimpleMath::Vector3::UnitX;
    DirectX::SimpleMath::Vector3                m_lightEffect2Pos1 = DirectX::SimpleMath::Vector3::UnitX;
    DirectX::SimpleMath::Vector3                m_lightEffect2Pos2 = DirectX::SimpleMath::Vector3::UnitX;

};

