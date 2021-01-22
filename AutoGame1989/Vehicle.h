#pragma once


class Vehicle
{
public:


private:
    DirectX::SimpleMath::Vector3    m_pos;          // world position
    DirectX::SimpleMath::Vector3    m_allignment;   // direction the vehicle is facing
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

