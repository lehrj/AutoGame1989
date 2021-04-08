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
    //char* mode;
    double gearRatio[7];  //  gear ratios
    std::vector<double> testRation[6];

    //////////////////////
    double maxAccelerationRate;
    double maxBrakeRate;
    double steeringAngle;
    double steeringAngleMax;
    DirectX::SimpleMath::Vector3 position;         // world position
    DirectX::SimpleMath::Vector3 heading;          // direction the vehicle is facing
    double speed;        // speed vehicle is traveling
    DirectX::SimpleMath::Vector3 velocity;     // direction the vehicle is traveling as it could be sliding or fishtailing
};

class Vehicle
{
public:
    Vehicle();

    void GearDown();
    void GearUp();

    DirectX::SimpleMath::Vector3 GetHeading() { return m_car.heading; };
    DirectX::SimpleMath::Vector3 GetPos() { return m_car.position; };
    float GetSpeed() { return m_car.speed; };
    DirectX::SimpleMath::Vector3 GetVelocity() { return m_car.velocity; };
    
    void ResetVehicle();
    void UpdateVehicle();

private:
    void InitializeVehicle();

    Car m_car;

    DirectX::SimpleMath::Vector3    m_pos;          // world position
    DirectX::SimpleMath::Vector3    m_heading;   // direction the vehicle is facing
    double                          m_speed;        // speed vehicle is traveling
    DirectX::SimpleMath::Vector3    m_velocity;     // direction the vehicle is traveling

    DirectX::SimpleMath::Vector4    m_defaultForward = DirectX::XMVectorSet(1.0, 0.0, 0.0, 0.0);
    DirectX::SimpleMath::Vector4    m_forward = DirectX::XMVectorSet(1.0, 0.0, 0.0, 0.0);
    DirectX::SimpleMath::Vector4    m_defaultRight = DirectX::XMVectorSet(0.0, 0.0, 1.0, 0.0);
    DirectX::SimpleMath::Vector4    m_right = DirectX::XMVectorSet(0.0, 0.0, 1.0, 0.0);

    double                          m_moveBackForward = 0.0;
    double                          m_moveLeftRight = 0.0;
    double                          m_moveUpDown = 0.0;
};

