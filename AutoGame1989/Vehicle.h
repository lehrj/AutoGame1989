#pragma once

struct Car
{

};

class Vehicle
{
public:
    Vehicle();

    DirectX::SimpleMath::Vector3 GetHeading() { return m_heading; };
    DirectX::SimpleMath::Vector3 GetPos() { return m_pos; };
    float GetSpeed() { return m_speed; };
    DirectX::SimpleMath::Vector3 GetVelocity() { return m_velocity; };

    void UpdateVehicle();

private:
    DirectX::SimpleMath::Vector3    m_pos;          // world position
    DirectX::SimpleMath::Vector3    m_heading;   // direction the vehicle is facing
    float                           m_speed;        // speed vehicle is traveling
    DirectX::SimpleMath::Vector3    m_velocity;     // direction the vehicle is traveling

    DirectX::SimpleMath::Vector4    m_defaultForward = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
    DirectX::SimpleMath::Vector4    m_forward = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
    DirectX::SimpleMath::Vector4    m_defaultRight = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
    DirectX::SimpleMath::Vector4    m_right = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

    float                           m_moveBackForward = 0.0f;
    float                           m_moveLeftRight = 0.0f;
    float                           m_moveUpDown = 0.0f;

};

