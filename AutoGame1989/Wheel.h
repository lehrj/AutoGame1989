#pragma once



struct WheelData
{
    int wheelID = 0;
    bool isStearable = false;
    bool isGrounded;
    DirectX::SimpleMath::Vector3 position = DirectX::SimpleMath::Vector3::Zero;
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

    private:

};

