#pragma once



struct WheelData
{
    int wheelID = 0;
    bool isStearable = false;
    bool isGrounded;
    DirectX::SimpleMath::Vector3 localPosition;
    DirectX::SimpleMath::Vector3 worldPosition;
    DirectX::SimpleMath::Vector3 forward = DirectX::SimpleMath::Vector3::UnitX;
    DirectX::SimpleMath::Vector3 up = DirectX::SimpleMath::Vector3::UnitY;

    DirectX::SimpleMath::Vector3 axle;
    DirectX::SimpleMath::Vector3 terrainNormal;
    double radius;
    double friction;
    double sidewallHeight;
    double width;
    double tireDampening;
    double mass;
    double rpm;

};


class Wheel
{
    public:
        void InitializeWheel();

        
        
        void SetAxelPos(DirectX::SimpleMath::Vector3 aPos) { m_wheelData.axle = aPos; };
        void SetForward(DirectX::SimpleMath::Vector3 aPos) { m_wheelData.forward = aPos; };
        void SetLocalPos(DirectX::SimpleMath::Vector3 aPos) { m_wheelData.localPosition = aPos; };      
        void SetTerrainNormal(DirectX::SimpleMath::Vector3 aPos) { m_wheelData.terrainNormal = aPos; };
        void SetUp(DirectX::SimpleMath::Vector3 aPos) { m_wheelData.up = aPos; };
        void SetWorldPos(DirectX::SimpleMath::Vector3 aPos) { m_wheelData.worldPosition = aPos; };


    private:
        WheelData m_wheelData;

};

