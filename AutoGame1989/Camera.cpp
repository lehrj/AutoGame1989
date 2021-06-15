#include "pch.h"
#include "Camera.h"

Camera::Camera()
{
	m_homePosition = DirectX::SimpleMath::Vector3(-1.0f, 0.0f, 0.0f);
	m_target = DirectX::SimpleMath::Vector3::Zero;
	m_up = DirectX::SimpleMath::Vector3::UnitY;
	m_homePitch = 0.0f;
	m_homeYaw = 0.0f;
	Reset();
	// ToDo WLJ: Add updated initilzation functions and test 
}

Camera::Camera(int aWidth, int aHeight)
{
	m_clientWidth = (int)aWidth;
	m_clientHeight = aHeight;
	m_homePosition = DirectX::SimpleMath::Vector3(0.0f, 0.4f, 0.0f);
	m_target = DirectX::SimpleMath::Vector3(0.0f, 0.0f, 0.0f);
	m_up = DirectX::SimpleMath::Vector3::UnitY;
	m_position = DirectX::SimpleMath::Vector3(-2.0f, 0.2f, 0.0f);
	m_homePitch = -0.053f;
	m_homeYaw = 0.0f;

	m_nearPlane = .1f;
	m_farPlane = 80.f;

	//m_cameraState = CameraState::CAMERASTATE_PRESWINGVIEW;
	//m_cameraState = CameraState::CAMERASTATE_FOLLOWVEHICLE;
	m_cameraState = CameraState::CAMERASTATE_SPRINGCAMERA;
	if (m_cameraState == CameraState::CAMERASTATE_SPRINGCAMERA)
	{
		//(Target aTarget, float aSpringConstant, float ahDist, float aVDist)
		Target testTarget;
		testTarget.forward = DirectX::SimpleMath::Vector3::UnitX;
		testTarget.up = DirectX::SimpleMath::Vector3::UnitY;
		testTarget.position = DirectX::SimpleMath::Vector3::Zero;
		float springConst = 52.00000;
		float hDist = 15.0;
		float vDist = 3.0;
		InitializeSpringCamera(testTarget, springConst, hDist, vDist);
	}

	Reset();
	InitializeViewMatrix();
	InitializeProjectionMatrix();
	InitializeOrthoganalMatrix();
}

Camera::~Camera()
{
	m_vehicleFocus = nullptr;
	delete m_vehicleFocus;
}

DirectX::SimpleMath::Vector3 Camera::GetPreSwingCamPos(DirectX::SimpleMath::Vector3 aPosition, float aDirection)
{
	DirectX::SimpleMath::Vector3 newCamPosition = DirectX::SimpleMath::Vector3::Transform(m_preSwingCamPosOffset,
		DirectX::SimpleMath::Matrix::CreateRotationY(aDirection)) + aPosition;
	return newCamPosition;
}

DirectX::SimpleMath::Vector3 Camera::GetPreSwingTargPos(DirectX::SimpleMath::Vector3 aPosition, float aDirectionDegrees)
{
	DirectX::SimpleMath::Vector3 newTargetPosition = DirectX::SimpleMath::Vector3::Transform(m_preSwingTargetPosOffset,
		DirectX::SimpleMath::Matrix::CreateRotationY(Utility::ToRadians(aDirectionDegrees))) + aPosition;
	return newTargetPosition;
}

DirectX::SimpleMath::Vector3 Camera::GetSwingCamPos(DirectX::SimpleMath::Vector3 aPosition, float aDirectionDegrees)
{
	DirectX::SimpleMath::Vector3 newCamPosition = DirectX::SimpleMath::Vector3::Transform(m_swingCamPosOffset,
		DirectX::SimpleMath::Matrix::CreateRotationY(Utility::ToRadians(aDirectionDegrees))) + aPosition;
	return newCamPosition;
}

DirectX::SimpleMath::Vector3 Camera::GetSwingTargPos(DirectX::SimpleMath::Vector3 aPosition, float aDirectionDegrees)
{
	DirectX::SimpleMath::Vector3 newTargetPosition = DirectX::SimpleMath::Vector3::Transform(m_swingTargetPosOffset,
		DirectX::SimpleMath::Matrix::CreateRotationY(Utility::ToRadians(aDirectionDegrees))) + aPosition;
	return newTargetPosition;
}

void Camera::InitializeOrthoganalMatrix()
{
	m_orthogonalMatrix = DirectX::SimpleMath::Matrix::CreateOrthographic((float)m_clientWidth, (float)m_clientHeight, m_nearPlane, m_farPlane);
}

void Camera::InitializeProjectionMatrix()
{
	m_projectionMatrix = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(DirectX::XM_PI / 4.f, static_cast<float>(m_clientWidth) / static_cast<float>(m_clientHeight), m_nearPlane, m_farPlane);
}

void Camera::InintializePreSwingCamera(DirectX::SimpleMath::Vector3 aPosition, float aDirectionDegrees)
{	
	DirectX::SimpleMath::Vector3 newCamPosition = DirectX::SimpleMath::Vector3::Transform(m_preSwingCamPosOffset,
		DirectX::SimpleMath::Matrix::CreateRotationY(Utility::ToRadians(aDirectionDegrees))) + aPosition;

	DirectX::SimpleMath::Vector3 newTargetPosition = DirectX::SimpleMath::Vector3::Transform(m_preSwingTargetPosOffset,
		DirectX::SimpleMath::Matrix::CreateRotationY(Utility::ToRadians(aDirectionDegrees))) + aPosition;

	SetTargetPos(newTargetPosition);
	SetPos(newCamPosition);
}

void Camera::InitializeViewMatrix()
{
	m_viewMatrix = DirectX::SimpleMath::Matrix::CreateLookAt(m_position, m_target, m_up);
}

bool Camera::IsCameraAtDestination()
{
	if (m_position == m_destinationPosition || m_isCameraAtDestination == true)
	{
		return true;
	}
	else
	{
		if (m_position == m_destinationPosition)
		{
			m_isCameraAtDestination = true;
			return true;
		}
		else
		{
			return false;
		}
	}
}

void Camera::OnResize(uint32_t aWidth, uint32_t aHeight)
{
	m_clientWidth = aWidth;
	m_clientHeight = aHeight;
	UpdateOrthoganalMatrix();
	UpdateProjectionMatrix();
	UpdateViewMatrix();
}

void Camera::Reset()
{
	m_position = m_homePosition;
	m_up = DirectX::SimpleMath::Vector3::UnitY; 
	m_pitch = m_homePitch;
	m_yaw = m_homeYaw;
}

void Camera::ResetCameraTransition(DX::StepTimer const& aTimer)
{
	DirectX::SimpleMath::Vector3 cameraStartPos = m_cameraStartPos;

	DirectX::SimpleMath::Vector3 cameraEndPos = m_cameraEndPos;

	float cameraDistance = DirectX::SimpleMath::Vector3::Distance(cameraStartPos, cameraEndPos);
	DirectX::SimpleMath::Vector3 cameraDirection = cameraEndPos - cameraStartPos;

	cameraDirection.Normalize();

	DirectX::SimpleMath::Vector3 targetStartPos = m_targetStartPos;
	DirectX::SimpleMath::Vector3 targetEndPos = m_targetEndPos;

	float targetDistance = DirectX::SimpleMath::Vector3::Distance(targetStartPos, targetEndPos);
	DirectX::SimpleMath::Vector3 targetDirection = targetEndPos - targetStartPos;
	targetDirection.Normalize();

	double elapsedTime = double(aTimer.GetElapsedSeconds());
	float cameraSpeed = m_cameraTransitionSpeed;

	float targetSpeed;
	if (abs(cameraDistance > 0.0)) // prevent divide by zero if camera position doesn't change
	{
		targetSpeed = cameraSpeed * (targetDistance / cameraDistance);
	}
	else
	{
		targetSpeed = cameraSpeed;
	}

	m_position += cameraDirection * cameraSpeed * static_cast<float>(elapsedTime);

	if (targetDistance > 0.0f)
	{
		m_target += targetDirection * targetSpeed * static_cast<float>(elapsedTime);
	}

	m_up = DirectX::SimpleMath::Vector3::UnitY;

	if (DirectX::SimpleMath::Vector3::Distance(cameraStartPos, m_position) >= cameraDistance && DirectX::SimpleMath::Vector3::Distance(targetStartPos, m_target) >= targetDistance)
	{
		m_position = cameraEndPos;
		m_isCameraAtDestination = true;
	}
	else
	{

	}
}

void Camera::ReverseTransitionDirection()
{
	std::swap(m_cameraStartPos, m_cameraEndPos);
	std::swap(m_targetStartPos, m_targetEndPos);
}

void Camera::Rotate(DirectX::SimpleMath::Vector3 aAxis, float aDegrees)
{
	if (aAxis == DirectX::SimpleMath::Vector3::Zero || aDegrees == 0.0f)
	{
		return;
	}

	DirectX::SimpleMath::Vector3 lookAtTarget = m_target - m_position;
	DirectX::SimpleMath::Vector3 lookAtUp = m_up - m_position;
	lookAtTarget = DirectX::SimpleMath::Vector3::Transform(lookAtTarget, DirectX::SimpleMath::Matrix::CreateFromAxisAngle(aAxis, static_cast<float>(Utility::ToRadians(aDegrees))));
	lookAtUp = DirectX::SimpleMath::Vector3::Transform(lookAtUp, DirectX::SimpleMath::Matrix::CreateFromAxisAngle(aAxis, static_cast<float>(Utility::ToRadians(aDegrees))));

	m_target = DirectX::SimpleMath::Vector3(m_position + lookAtTarget);
	m_up = DirectX::SimpleMath::Vector3(m_position + lookAtUp);

	this->InitializeViewMatrix();
}

void Camera::RotateAtSpeed(float aDx, float aDy)
{
	m_yaw = Utility::WrapAngle(m_yaw + aDx * m_rotationTravelSpeed);
	float rotation = m_pitch + aDy * m_rotationTravelSpeed;
	float min = 0.995f * static_cast<float>(-Utility::GetPi()) / 2.0f;
	float max = 0.995f * static_cast<float>(Utility::GetPi()) / 2.0f;

	m_pitch = std::clamp(rotation, min, max);
}

void Camera::RotateCounterClockWise()
{
	float spinRate = 3.0f;
	m_yaw = Utility::WrapAngle(m_yaw + spinRate * m_rotationTravelSpeed);
	UpdateViewMatrix();
}

void Camera::RotateClockWise()
{
	float spinRate = 3.0f;
	m_yaw = Utility::WrapAngle(m_yaw - spinRate * m_rotationTravelSpeed);
	UpdateViewMatrix();
}

void Camera::SetCameraEndPos(DirectX::SimpleMath::Vector3 aEndPos)
{
	m_cameraEndPos = aEndPos;
}

void Camera::SetCameraStartPos(DirectX::SimpleMath::Vector3 aStartPos)
{
	m_cameraStartPos = aStartPos;
}

void Camera::SetCameraState(const CameraState aCameraState)
{
	m_cameraState = aCameraState;
}

void Camera::SetDestinationPos(const DirectX::SimpleMath::Vector3 aDestPos)
{
	if (aDestPos == m_target)
	{
		int testBreak = 0;
		// add error handling to prevent crash
		//std::cerr << "Error in Camera::UpdatePosition, updated position = current target position";
		return;
	}
	m_destinationPosition = aDestPos;
}

void Camera::SetFollowCamDirection(const DirectX::SimpleMath::Vector3 aDirection)
{
	m_followCamDirection = aDirection;
}

void Camera::SetFollowCamPos(const DirectX::SimpleMath::Vector3 aPos)
{
	m_followCamPos = aPos;
}

void Camera::SetFollowCamTarget(const DirectX::SimpleMath::Vector3 aTarg)
{
	m_followCamTarget = aTarg;
}

void Camera::SetFollowCamUp(const DirectX::SimpleMath::Vector3 aUp)
{
	m_followCamUp = aUp;
}

void Camera::SetHomePos(DirectX::SimpleMath::Vector3 aHomePos)
{
	if (aHomePos == m_target)
	{
		// add error handling to prevent crash
		//std::cerr << "Error in Camera::UpdatePosition, updated position = current target position";
		return;
	}
	m_position = aHomePos;
}

void Camera::SetPos(DirectX::SimpleMath::Vector3 aPos)
{
	if (aPos == m_target)
	{
		// add error handling to prevent crash
		//std::cerr << "Error in Camera::UpdatePosition, updated position = current target position";
		return;
	}
	m_position = aPos;
}

void Camera::SetTargetEndPos(DirectX::SimpleMath::Vector3 aEndPos)
{
	m_targetEndPos = aEndPos;
}

void Camera::SetTargetStartPos(DirectX::SimpleMath::Vector3 aStartPos)
{
	m_targetStartPos = aStartPos;
}

void Camera::SetTargetPos(const DirectX::SimpleMath::Vector3 aTarget)
{
	if (aTarget == m_position)
	{
		// add error handling to prevent crash
		//std::cerr << "Error in Camera::SetTargetPos, updated target position = current camera position";
		return;
	}
	m_target = aTarget;
}

void Camera::SetUpPos(const DirectX::SimpleMath::Vector3 aPos)
{
	m_up = aPos;
}

void Camera::UpdateTransitionCamera(DX::StepTimer const& aTimer)
{
	DirectX::SimpleMath::Vector3 cameraStartPos = m_cameraStartPos;

	DirectX::SimpleMath::Vector3 cameraEndPos = m_cameraEndPos;

	float cameraDistance = DirectX::SimpleMath::Vector3::Distance(cameraStartPos, cameraEndPos);
	DirectX::SimpleMath::Vector3 cameraDirection = cameraEndPos - cameraStartPos;

	cameraDirection.Normalize();

	DirectX::SimpleMath::Vector3 targetStartPos = m_targetStartPos;
	DirectX::SimpleMath::Vector3 targetEndPos = m_targetEndPos;

	float targetDistance = DirectX::SimpleMath::Vector3::Distance(targetStartPos, targetEndPos);
	DirectX::SimpleMath::Vector3 targetDirection = targetEndPos - targetStartPos;
	targetDirection.Normalize();

	double elapsedTime = double(aTimer.GetElapsedSeconds());
	float cameraSpeed = m_cameraTransitionSpeed;

	float targetSpeed;
	if (abs(cameraDistance > 0.0)) // prevent divide by zero if camera position doesn't change
	{
		targetSpeed = cameraSpeed * (targetDistance / cameraDistance);
	}
	else
	{
		targetSpeed = cameraSpeed;
	}

	m_position += cameraDirection * cameraSpeed * static_cast<float>(elapsedTime);

	if (targetDistance > 0.0f)
	{
		m_target += targetDirection * targetSpeed * static_cast<float>(elapsedTime);
	}

	m_up = DirectX::SimpleMath::Vector3::UnitY;

	if (DirectX::SimpleMath::Vector3::Distance(cameraStartPos, m_position) >= cameraDistance && DirectX::SimpleMath::Vector3::Distance(targetStartPos, m_target) >= targetDistance)
	{
		m_position = cameraEndPos;
		m_isCameraAtDestination = true;
	}
	else
	{

	}
}
void Camera::SetTransitionSpeed(const float aSpeed)
{
	if (aSpeed > 0.0)
	{
		m_cameraTransitionSpeed = aSpeed;
	}
	else
	{
		// add error handling to prevent transition stall or backwards motion that could break things
		// cerr << "input range out of bounds"
	}
}

void Camera::SetVehicleFocus(const Vehicle* aVehicle)
{
	m_vehicleFocus = aVehicle;
}

void Camera::TranslateAtSpeed(DirectX::SimpleMath::Vector3 aTranslation)
{
	DirectX::XMStoreFloat3(&aTranslation, DirectX::XMVector3Transform(
		DirectX::XMLoadFloat3(&aTranslation),
		DirectX::XMMatrixRotationRollPitchYaw(m_pitch, m_yaw, 0.0f) *
		DirectX::XMMatrixScaling(m_posTravelSpeed, m_posTravelSpeed, m_posTravelSpeed)
	));
	m_position = { m_position.x + aTranslation.x, m_position.y + aTranslation.y, m_position.z + aTranslation.z };
}

void Camera::UpdateCamera(DX::StepTimer const& aTimer)
{
	UpdateTimer(aTimer);
	if (m_cameraState == CameraState::CAMERASTATE_FIRSTPERSON)
	{
		UpdateFirstPersonCamera();
	}
	if (m_cameraState == CameraState::CAMERASTATE_SWINGVIEW)
	{
		// no update needed in current state
	}
	if (m_cameraState == CameraState::CAMERASTATE_PROJECTILEFLIGHTVIEW)
	{
		// no update needed in current state
	}
	if (m_cameraState == CameraState::CAMERASTATE_PRESWINGVIEW)
	{
		// no update needed in current state
	}
	if (m_cameraState == CameraState::CAMERASTATE_TRANSITION)
	{
		bool testIsAtDest = m_isCameraAtDestination;
		DirectX::SimpleMath::Vector3 testPos = m_position;
		DirectX::SimpleMath::Vector3 testDestPos = m_destinationPosition;

		if (IsCameraAtDestination() == false)
		{
			UpdateTransitionCamera(aTimer);
		}
		else
		{
			m_cameraState = CameraState::CAMERASTATE_SWINGVIEW;
			m_isCameraAtDestination = false;
		}
	}
	if (m_cameraState == CameraState::CAMERASTATE_RESET)
	{
		if (IsCameraAtDestination() == false)
		{
			UpdateTransitionCamera(aTimer);
		}
		else
		{
			m_cameraState = CameraState::CAMERASTATE_PRESWINGVIEW;
			m_isCameraAtDestination = false;
		}
	}
	if (m_cameraState == CameraState::CAMERASTATE_TRANSTONEWSHOT)
	{
		if (IsCameraAtDestination() == false)
		{
			UpdateTransitionCamera(aTimer);
		}
		else
		{
			SetCameraStartPos(GetPos());     
			SetCameraEndPos(GetPreSwingCamPos(GetPos(), 0.0));
			SetTargetStartPos(GetTargetPos());
			SetTargetEndPos(GetPreSwingTargPos(GetPos(), 0.0));

			m_cameraState = CameraState::CAMERASTATE_PRESWINGVIEW;
			m_isCameraAtDestination = false;
		}
	}
	if (m_cameraState == CameraState::CAMERASTATE_FOLLOWVEHICLE)
	{
		UpdateChaseCamera();
	}
	if (m_cameraState == CameraState::CAMERASTATE_SPRINGCAMERA)
	{
		m_springTarget.position = m_vehicleFocus->GetPos();
		//m_springTarget.forward = m_vehicleFocus->GetHeading();
		DirectX::SimpleMath::Vector3 testHeading = DirectX::SimpleMath::Vector3::UnitX;
		DirectX::SimpleMath::Matrix rotMat = DirectX::SimpleMath::Matrix::CreateRotationY(m_vehicleFocus->GetCarRotation());

		testHeading = DirectX::SimpleMath::Vector3::Transform(testHeading, rotMat);
		m_springTarget.forward = testHeading;

		UpdateSpringCamera(aTimer);
		m_viewMatrix = m_springCameraMatrix;
	}
	else
	{
		m_viewMatrix = DirectX::SimpleMath::Matrix::CreateLookAt(m_position, m_target, m_up);
	}
}

void Camera::UpdateFirstPersonCamera()
{
	// apply setting for inverting first person camer Y axis controls 
	if (m_isFpYaxisInverted == true)
	{
		m_rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(0, m_yaw, -m_pitch);
	}
	else
	{
		m_rotationMatrix = DirectX::XMMatrixRotationRollPitchYaw(0, m_yaw, m_pitch);
	}

	m_target = DirectX::XMVector3TransformCoord(m_defaultForward, m_rotationMatrix);
	m_target.Normalize();

	m_right = DirectX::XMVector3TransformCoord(m_defaultRight, m_rotationMatrix);
	m_forward = DirectX::XMVector3TransformCoord(m_defaultForward, m_rotationMatrix);

	m_up = DirectX::XMVector3Cross(m_right, m_forward);

	m_position += DirectX::operator*(m_moveLeftRight, m_right);
	m_position += DirectX::operator*(m_moveBackForward, m_forward);
	m_position += DirectX::operator*(m_moveUpDown, m_up);

	m_moveLeftRight = 0.0f;
	m_moveBackForward = 0.0f;
	m_moveUpDown = 0.0f;

	m_target = m_position + m_target;
}

void Camera::ComputeSpringMatrix()
{
	DirectX::SimpleMath::Vector3 cameraForward = m_springTarget.position - m_actualPosition;
	cameraForward.Normalize();
	DirectX::SimpleMath::Vector3 cameraLeft = m_springTarget.up.Cross(cameraForward);
	cameraLeft.Normalize();
	DirectX::SimpleMath::Vector3 cameraUp = cameraForward.Cross(cameraLeft);
	cameraUp.Normalize();
	m_springCameraMatrix = DirectX::SimpleMath::Matrix::CreateLookAt(m_actualPosition, m_springTarget.position, cameraUp);

}

void Camera::InitializeSpringCamera(Target aTarget, float aSpringConstant, float ahDist, float aVDist)
{
	m_springTarget = aTarget;
	m_springConstant = aSpringConstant;
	m_hDistance = ahDist;
	m_vDistance = aVDist;

	m_dampConstant = 2.0f * sqrt(m_springConstant);
	m_actualPosition = m_springTarget.position - m_springTarget.forward * ahDist + m_springTarget.up * aVDist;
	m_velocity = DirectX::SimpleMath::Vector3::Zero;
	ComputeSpringMatrix();
}

void Camera::UpdateSpringCamera(DX::StepTimer const& aTimeDelta)
{
	DirectX::SimpleMath::Vector3 idealPosition = m_springTarget.position - m_springTarget.forward * m_hDistance + m_springTarget.up * m_vDistance;
	DirectX::SimpleMath::Vector3 displacement = m_actualPosition - idealPosition;
	DirectX::SimpleMath::Vector3 springAccel = (-m_springConstant * displacement) - (m_dampConstant * m_velocity);
	m_velocity += springAccel * aTimeDelta.GetElapsedSeconds();
	//m_actualPosition = idealPosition;
	m_actualPosition += m_velocity * aTimeDelta.GetElapsedSeconds();
	ComputeSpringMatrix();
}

void Camera::UpdateChaseCamera()
{
	double accel = m_vehicleFocus->GetAccel() * 0.001;
	DirectX::SimpleMath::Vector3 accelCamPos = m_followCamPos;
	accelCamPos.x += accel;

	SetUpPos(m_followCamUp);
	SetTargetPos(m_vehicleFocus->GetPos() + m_followCamTargOffset);
	//m_chaseCameQuat = DirectX::SimpleMath::Quaternion::Lerp(m_chaseCameQuat, DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(m_vehicleFocus->GetRotation() + Utility::ToRadians(90.0), 0.0, 0.0), m_chaseCamLerpFactor);
	//m_chaseCameQuat = DirectX::SimpleMath::Quaternion::Lerp(m_chaseCameQuat, DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(m_vehicleFocus->GetRotation(), 0.0, 0.0), m_chaseCamLerpFactor);
	m_chaseCameQuat = DirectX::SimpleMath::Quaternion::Lerp(m_chaseCameQuat, DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(m_vehicleFocus->GetRotation(), 0.0, accel), m_chaseCamLerpFactor);

	//m_chaseCameQuat = DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(m_vehicleFocus->GetRotation(), 0.0, 0.0);
	//DirectX::SimpleMath::Vector3 cameraPos = m_followCamPos;
	//DirectX::SimpleMath::Vector3 cameraPos = accelCamPos;
	//DirectX::SimpleMath::Vector3 testAccelPos = DirectX::SimpleMath::Vector3::Lerp(accelCamPos, m_followCamPos, 0.0001);
	DirectX::SimpleMath::Vector3 testAccelPos = DirectX::SimpleMath::Vector3::SmoothStep(accelCamPos, m_followCamPos, 0.0001);
	DirectX::SimpleMath::Vector3 cameraPos = testAccelPos;
	cameraPos = DirectX::SimpleMath::Vector3::Transform(cameraPos, m_chaseCameQuat);
	cameraPos += m_vehicleFocus->GetPos();
	SetPos(cameraPos);
}

/*
void Camera::UpdateChaseCamera()
{
	double accel = m_vehicleFocus->GetAccel();
	DirectX::SimpleMath::Vector3 camOffset = m_followCamPos;
	camOffset.x += accel;

	SetUpPos(m_followCamUp);
	SetTargetPos(m_vehicleFocus->GetPos() + m_followCamTargOffset);
	//m_chaseCameQuat = DirectX::SimpleMath::Quaternion::Lerp(m_chaseCameQuat, DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(m_vehicleFocus->GetRotation() + Utility::ToRadians(90.0), 0.0, 0.0), m_chaseCamLerpFactor);
	m_chaseCameQuat = DirectX::SimpleMath::Quaternion::Lerp(m_chaseCameQuat, DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(m_vehicleFocus->GetRotation(), 0.0, 0.0), m_chaseCamLerpFactor);
	DirectX::SimpleMath::Vector3 cameraPos = m_followCamPos;
	camOffset
	//cameraPos = DirectX::SimpleMath::Vector3::Transform(cameraPos, m_chaseCameQuat);

	cameraPos = DirectX::SimpleMath::Vector3::Transform(camOffset, m_chaseCameQuat);
	cameraPos += m_vehicleFocus->GetPos();

	SetPos(cameraPos);
}
*/

/*
void Camera::UpdateChaseCamera()
{
	double accel = m_vehicleFocus->GetAccel() * 0.5;
	DirectX::SimpleMath::Vector3 camOffset = m_followCamTargOffset;
	m_followCamTargOffset.x += accel;
	SetUpPos(m_followCamUp);
	SetTargetPos(m_vehicleFocus->GetPos() + m_followCamTargOffset);
	//m_chaseCameQuat = DirectX::SimpleMath::Quaternion::Lerp(m_chaseCameQuat, DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(m_vehicleFocus->GetRotation() + Utility::ToRadians(90.0), 0.0, 0.0), m_chaseCamLerpFactor);
	m_chaseCameQuat = DirectX::SimpleMath::Quaternion::Lerp(m_chaseCameQuat, DirectX::SimpleMath::Quaternion::CreateFromYawPitchRoll(m_vehicleFocus->GetRotation(), 0.0, 0.0), m_chaseCamLerpFactor);
	DirectX::SimpleMath::Vector3 cameraPos = m_followCamPos;
	cameraPos = DirectX::SimpleMath::Vector3::Transform(cameraPos, m_chaseCameQuat);
	cameraPos += m_vehicleFocus->GetPos();

	m_followCamTargOffset = camOffset;
	SetPos(cameraPos);
}
*/

void Camera::UpdatePitchYaw(const float aPitch, const float aYaw)
{
	m_pitch += aPitch * m_rotationTravelSpeed;
	m_yaw += aYaw * m_rotationTravelSpeed;

	// keep longitude in sane range by wrapping
	if (m_yaw > DirectX::XM_PI)
	{
		m_yaw -= DirectX::XM_PI * 2.0f;
	}
	else if (m_yaw < -DirectX::XM_PI)
	{
		m_yaw += DirectX::XM_PI * 2.0f;
	}

	// limit pitch to straight up or straight down
	// with a little fudge-factor to avoid gimbal lock
	float limit = DirectX::XM_PI / 2.0f - 0.01f;
	m_pitch = std::max(-limit, m_pitch);
	m_pitch = std::min(+limit, m_pitch);
}

void Camera::UpdatePos(const float aX, const float aY, const float aZ)
{
	m_moveLeftRight += aX * m_posTravelSpeed;
	m_moveBackForward += aZ * m_posTravelSpeed;
	m_moveUpDown += aY * m_posTravelSpeed;
}

void Camera::UpdateOrthoganalMatrix()
{
	m_orthogonalMatrix = DirectX::SimpleMath::Matrix::CreateOrthographic((float)m_clientWidth, (float)m_clientHeight, m_nearPlane, m_farPlane);
}

void Camera::UpdateProjectionMatrix()
{
	m_projectionMatrix = DirectX::SimpleMath::Matrix::CreatePerspectiveFieldOfView(DirectX::XM_PI / 4.f, (float)m_clientWidth / (float)m_clientHeight, m_nearPlane, m_farPlane);
}

void Camera::UpdateViewMatrix()
{
	DirectX::SimpleMath::Vector3 newPosition = DirectX::SimpleMath::Vector3::Transform(m_position, DirectX::SimpleMath::Matrix::CreateRotationY(m_yaw));
	m_viewMatrix = DirectX::SimpleMath::Matrix::CreateLookAt(newPosition, m_target, m_up);
}

void Camera::YawSpin(float aTurn)
{
	Utility::WrapAngle(m_yaw += aTurn * m_cameraTransitionSpeed);
}

void Camera::TurnAroundPoint(float aTurn, DirectX::SimpleMath::Vector3 aCenterPoint)
{
	DirectX::SimpleMath::Vector3 updateTarget = m_target;
	DirectX::SimpleMath::Vector3 updateCamPos = m_position;

	updateTarget -= aCenterPoint;
	updateCamPos -= aCenterPoint;

	DirectX::SimpleMath::Matrix rotMatrix = DirectX::SimpleMath::Matrix::CreateRotationY(aTurn * m_aimTurnRate);
	updateTarget = DirectX::SimpleMath::Vector3::Transform(updateTarget, rotMatrix);
	updateCamPos = DirectX::SimpleMath::Vector3::Transform(updateCamPos, rotMatrix);

	updateTarget += aCenterPoint;
	updateCamPos += aCenterPoint;
	
	m_target = updateTarget;
	m_position = updateCamPos;
}

void Camera::TurnEndPosAroundPoint(float aTurn, DirectX::SimpleMath::Vector3 aCenterPoint)
{
	DirectX::SimpleMath::Vector3 updateTarget = m_targetEndPos; 
	DirectX::SimpleMath::Vector3 updateCamPos = m_cameraEndPos; 

	updateTarget -= aCenterPoint;
	updateCamPos -= aCenterPoint;

	DirectX::SimpleMath::Matrix rotMatrix = DirectX::SimpleMath::Matrix::CreateRotationY(aTurn);
	updateTarget = DirectX::SimpleMath::Vector3::Transform(updateTarget, rotMatrix);
	updateCamPos = DirectX::SimpleMath::Vector3::Transform(updateCamPos, rotMatrix);

	updateTarget += aCenterPoint;
	updateCamPos += aCenterPoint;

	m_targetEndPos = updateTarget;
	m_cameraEndPos = updateCamPos;
}