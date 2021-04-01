#pragma once

struct Car
{
    int numEqns;
    double s;
    double q[6];
    double mass;
    double area;
    double density;
    double Cd;

    double muR;
    double omegaE;
    double redline;
    double finalDriveRatio;
    double wheelRadius;
    int gearNumber;     //  gear the car is in
    int numberOfGears;  //  total number of gears
    char* mode;
    double gearRatio[7];  //  gear ratios
    std::vector<double> testRation[6];

    //////////////////////
    double steeringAngle;
    DirectX::SimpleMath::Vector3 position;         // world position
    DirectX::SimpleMath::Vector3 heading;          // direction the vehicle is facing
    double speed;        // speed vehicle is traveling
    DirectX::SimpleMath::Vector3    m_velocity;     // direction the vehicle is traveling as it could be sliding or fishtailing
};

class Vehicle
{
public:
    Vehicle();

    DirectX::SimpleMath::Vector3 GetHeading() { return m_heading; };
    DirectX::SimpleMath::Vector3 GetPos() { return m_pos; };
    float GetSpeed() { return m_speed; };
    DirectX::SimpleMath::Vector3 GetVelocity() { return m_velocity; };
    void InitializeVehicle();
    void UpdateVehicle();

private:
    DirectX::SimpleMath::Vector3    m_pos;          // world position
    DirectX::SimpleMath::Vector3    m_heading;   // direction the vehicle is facing
    float                           m_speed;        // speed vehicle is traveling
    DirectX::SimpleMath::Vector3    m_velocity;     // direction the vehicle is traveling

    Vehicle m_vehicle;

    DirectX::SimpleMath::Vector4    m_defaultForward = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
    DirectX::SimpleMath::Vector4    m_forward = DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);
    DirectX::SimpleMath::Vector4    m_defaultRight = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
    DirectX::SimpleMath::Vector4    m_right = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

    float                           m_moveBackForward = 0.0f;
    float                           m_moveLeftRight = 0.0f;
    float                           m_moveUpDown = 0.0f;

};

