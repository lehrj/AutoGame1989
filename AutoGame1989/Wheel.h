#pragma once



struct WheelData
{
    int wheelID{};
    bool isSteerable{};
    bool isGrounded{};
    DirectX::SimpleMath::Vector3 localPosition;
    DirectX::SimpleMath::Vector3 worldPosition;
    DirectX::SimpleMath::Vector3 forward = DirectX::SimpleMath::Vector3::UnitX;
    DirectX::SimpleMath::Vector3 up = DirectX::SimpleMath::Vector3::UnitY;

    DirectX::SimpleMath::Vector3 axle;
    DirectX::SimpleMath::Vector3 terrainNormal;
    float radius{};
    float friction{};
    float sidewallHeight{};
    float width{};
    float tireDampening{};
    float mass{};
    float angularVelocity{};  // in radians per second
};

class Wheel
{
    public:      
        void InitializeWheel(DirectX::SimpleMath::Vector3 aLocalPos, DirectX::SimpleMath::Vector3 aWorldPos, bool aIsSteerable, float aRadius, float aWidth, float aMass);
        void UpdateWheel();


    private:
        void SetAngularVelocity(const float aAngVel) { m_wheelData.angularVelocity = aAngVel; };
        void SetAxelPos(DirectX::SimpleMath::Vector3 aPos) { m_wheelData.axle = aPos; };
        void SetForward(DirectX::SimpleMath::Vector3 aPos) { m_wheelData.forward = aPos; };
        void SetLocalPos(DirectX::SimpleMath::Vector3 aPos) { m_wheelData.localPosition = aPos; };
        void SetMass(const float aMass) { m_wheelData.mass = aMass; };
        void SetRadius(float aRadius) { m_wheelData.radius = aRadius; };
        void SetSteerable(const bool aIsSteerable) { m_wheelData.isSteerable = aIsSteerable; };
        void SetTerrainNormal(DirectX::SimpleMath::Vector3 aPos) { m_wheelData.terrainNormal = aPos; };
        void SetUp(DirectX::SimpleMath::Vector3 aPos) { m_wheelData.up = aPos; };
        void SetWidth(float aWidth) { m_wheelData.radius = aWidth; };
        void SetWorldPos(DirectX::SimpleMath::Vector3 aPos) { m_wheelData.worldPosition = aPos; };
        
        WheelData m_wheelData;

};

