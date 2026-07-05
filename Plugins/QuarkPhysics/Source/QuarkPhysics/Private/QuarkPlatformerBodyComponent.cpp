#include "QuarkPlatformerBodyComponent.h"
#include "QuarkWorldComponent.h"
#include "GameFramework/Actor.h"

UQuarkPlatformerBodyComponent::UQuarkPlatformerBodyComponent()
{
}

void UQuarkPlatformerBodyComponent::BeginPlay()
{
	// Don't call Super::BeginPlay() because UQuarkRigidBodyComponent::BeginPlay
	// creates a QRigidBody. We need QPlatformerBody instead, so we replicate the
	// base class setup here with our own body type.

	// Replicate UQuarkBodyComponent::BeginPlay
	AActor* Owner = GetOwner();
	if (Owner)
	{
		OwnerWorld = Owner->FindComponentByClass<UQuarkWorldComponent>();
	}

	float PPU = OwnerWorld ? OwnerWorld->PixelsPerUnit : 1.0f;

	QPlatformerBody* PlatBody = new QPlatformerBody();
	PlatBody->manualDeletion = true;

	// Apply RigidBody properties
	PlatBody->SetFixedRotationEnabled(bFixedRotation);
	PlatBody->SetKinematicEnabled(bKinematic);

	// Apply PlatformerBody-specific editable properties
	PlatBody->SetGravity(QVector(Gravity.X * PPU, Gravity.Y * PPU));
	PlatBody->SetGravityMultiplier(GravityMultiplier);
	PlatBody->SetWalkSpeed(WalkSpeed * PPU);
	PlatBody->SetWalkAcelerationRate(WalkAccelerationRate);
	PlatBody->SetWalkDecelerationRate(WalkDecelerationRate);
	PlatBody->SetJumpForce(JumpForceValue * PPU);
	PlatBody->SetMaxJumpCount(MaxJumpCount);
	PlatBody->SetJumpDurationFrameCount(JumpDurationFrameCount);
	PlatBody->SetJumpGravityMultiplier(JumpGravityMultiplier);
	PlatBody->SetJumpFallGravityMultiplier(JumpFallGravityMultiplier);
	PlatBody->SetFloorMaxAngleDegree(FloorMaxAngleDegrees);
	PlatBody->SetMovingFloorSnapOffset(MovingFloorSnapOffset * PPU);
	PlatBody->SetSpecificPlatformLayers(PlatformLayersBit);

	PhysicsBody = PlatBody;

	if (OwnerWorld)
	{
		OwnerWorld->AddBody(this);
	}
}

// Floor
void UQuarkPlatformerBodyComponent::SetFloorMaxAngle(float InRadians)
{
	FloorMaxAngleDegrees = FMath::RadiansToDegrees(InRadians);
	if (PhysicsBody)
	{
		static_cast<QPlatformerBody*>(PhysicsBody)->SetFloorMaxAngle(InRadians);
	}
}

float UQuarkPlatformerBodyComponent::GetFloorMaxAngle() const
{
	return PhysicsBody ? static_cast<QPlatformerBody*>(PhysicsBody)->GetFloorMaxAngle() : 0.0f;
}

void UQuarkPlatformerBodyComponent::SetFloorMaxAngleDegree(float InDegrees)
{
	FloorMaxAngleDegrees = InDegrees;
	if (PhysicsBody)
	{
		static_cast<QPlatformerBody*>(PhysicsBody)->SetFloorMaxAngleDegree(InDegrees);
	}
}

float UQuarkPlatformerBodyComponent::GetFloorMaxAngleDegree() const
{
	return PhysicsBody ? static_cast<QPlatformerBody*>(PhysicsBody)->GetFloorMaxAngleDegree() : 0.0f;
}

bool UQuarkPlatformerBodyComponent::IsOnFloor() const
{
	return PhysicsBody ? static_cast<QPlatformerBody*>(PhysicsBody)->GetIsOnFloor() : false;
}

bool UQuarkPlatformerBodyComponent::IsOnCeiling() const
{
	return PhysicsBody ? static_cast<QPlatformerBody*>(PhysicsBody)->GetIsOnCeiling() : false;
}

// Gravity
void UQuarkPlatformerBodyComponent::SetGravity(FVector2D InGravity)
{
	Gravity = InGravity;
	if (PhysicsBody)
	{
		float PPU = OwnerWorld ? OwnerWorld->PixelsPerUnit : 1.0f;
		static_cast<QPlatformerBody*>(PhysicsBody)->SetGravity(QVector(InGravity.X * PPU, InGravity.Y * PPU));
	}
}

FVector2D UQuarkPlatformerBodyComponent::GetGravity() const
{
	if (PhysicsBody)
	{
		QVector G = static_cast<QPlatformerBody*>(PhysicsBody)->GetGravity();
		float PPU = OwnerWorld ? OwnerWorld->PixelsPerUnit : 1.0f;
		return FVector2D(G.x / PPU, G.y / PPU);
	}
	return Gravity;
}

void UQuarkPlatformerBodyComponent::SetGravityMultiplier(float InMultiplier)
{
	GravityMultiplier = InMultiplier;
	if (PhysicsBody)
	{
		static_cast<QPlatformerBody*>(PhysicsBody)->SetGravityMultiplier(InMultiplier);
	}
}

float UQuarkPlatformerBodyComponent::GetGravityMultiplier() const
{
	return PhysicsBody ? static_cast<QPlatformerBody*>(PhysicsBody)->GetGravityMultiplier() : GravityMultiplier;
}

// Walk
void UQuarkPlatformerBodyComponent::SetWalkSpeed(float InSpeed)
{
	WalkSpeed = InSpeed;
	if (PhysicsBody)
	{
		float PPU = OwnerWorld ? OwnerWorld->PixelsPerUnit : 1.0f;
		static_cast<QPlatformerBody*>(PhysicsBody)->SetWalkSpeed(InSpeed * PPU);
	}
}

float UQuarkPlatformerBodyComponent::GetWalkSpeed() const
{
	if (PhysicsBody)
	{
		float PPU = OwnerWorld ? OwnerWorld->PixelsPerUnit : 1.0f;
		return static_cast<QPlatformerBody*>(PhysicsBody)->GetWalkSpeed() / PPU;
	}
	return WalkSpeed;
}

void UQuarkPlatformerBodyComponent::SetWalkAccelerationRate(float InRate)
{
	WalkAccelerationRate = InRate;
	if (PhysicsBody)
	{
		static_cast<QPlatformerBody*>(PhysicsBody)->SetWalkAcelerationRate(InRate);
	}
}

float UQuarkPlatformerBodyComponent::GetWalkAccelerationRate() const
{
	return PhysicsBody ? static_cast<QPlatformerBody*>(PhysicsBody)->GetWalkAcelerationRate() : WalkAccelerationRate;
}

void UQuarkPlatformerBodyComponent::SetWalkDecelerationRate(float InRate)
{
	WalkDecelerationRate = InRate;
	if (PhysicsBody)
	{
		static_cast<QPlatformerBody*>(PhysicsBody)->SetWalkDecelerationRate(InRate);
	}
}

float UQuarkPlatformerBodyComponent::GetWalkDecelerationRate() const
{
	return PhysicsBody ? static_cast<QPlatformerBody*>(PhysicsBody)->GetWalkDecelerationRate() : WalkDecelerationRate;
}

void UQuarkPlatformerBodyComponent::Walk(int32 Side)
{
	if (PhysicsBody)
	{
		static_cast<QPlatformerBody*>(PhysicsBody)->Walk(Side);
	}
}

// Controller velocities
void UQuarkPlatformerBodyComponent::SetControllerHorizontalVelocity(FVector2D InVelocity)
{
	if (PhysicsBody)
	{
		float PPU = OwnerWorld ? OwnerWorld->PixelsPerUnit : 1.0f;
		static_cast<QPlatformerBody*>(PhysicsBody)->SetControllerHorizontalVelocity(QVector(InVelocity.X * PPU, InVelocity.Y * PPU));
	}
}

FVector2D UQuarkPlatformerBodyComponent::GetControllerHorizontalVelocity() const
{
	if (PhysicsBody)
	{
		QVector Vel = static_cast<QPlatformerBody*>(PhysicsBody)->GetControllerHorizontalVelocity();
		float PPU = OwnerWorld ? OwnerWorld->PixelsPerUnit : 1.0f;
		return FVector2D(Vel.x / PPU, Vel.y / PPU);
	}
	return FVector2D::ZeroVector;
}

void UQuarkPlatformerBodyComponent::SetControllerVerticalVelocity(FVector2D InVelocity)
{
	if (PhysicsBody)
	{
		float PPU = OwnerWorld ? OwnerWorld->PixelsPerUnit : 1.0f;
		static_cast<QPlatformerBody*>(PhysicsBody)->SetControllerVerticalVelocity(QVector(InVelocity.X * PPU, InVelocity.Y * PPU));
	}
}

FVector2D UQuarkPlatformerBodyComponent::GetControllerVerticalVelocity() const
{
	if (PhysicsBody)
	{
		QVector Vel = static_cast<QPlatformerBody*>(PhysicsBody)->GetControllerVerticalVelocity();
		float PPU = OwnerWorld ? OwnerWorld->PixelsPerUnit : 1.0f;
		return FVector2D(Vel.x / PPU, Vel.y / PPU);
	}
	return FVector2D::ZeroVector;
}

bool UQuarkPlatformerBodyComponent::IsFalling() const
{
	return PhysicsBody ? static_cast<QPlatformerBody*>(PhysicsBody)->GetIsFalling() : false;
}

bool UQuarkPlatformerBodyComponent::IsRising() const
{
	return PhysicsBody ? static_cast<QPlatformerBody*>(PhysicsBody)->GetIsRising() : false;
}

// Jump
void UQuarkPlatformerBodyComponent::SetJumpForce(float InForce)
{
	JumpForceValue = InForce;
	// QPlatformerBody doesn't expose a SetJumpForce setter;
	// the force is passed per-Jump() call. Store for default usage.
}

float UQuarkPlatformerBodyComponent::GetJumpForce() const
{
	return JumpForceValue;
}

void UQuarkPlatformerBodyComponent::SetMaxJumpCount(int32 InCount)
{
	MaxJumpCount = InCount;
	if (PhysicsBody)
	{
		static_cast<QPlatformerBody*>(PhysicsBody)->SetMaxJumpCount(InCount);
	}
}

int32 UQuarkPlatformerBodyComponent::GetMaxJumpCount() const
{
	return PhysicsBody ? static_cast<QPlatformerBody*>(PhysicsBody)->GetMaxJumpCount() : MaxJumpCount;
}

void UQuarkPlatformerBodyComponent::SetJumpDurationFrameCount(int32 InCount)
{
	JumpDurationFrameCount = InCount;
	if (PhysicsBody)
	{
		static_cast<QPlatformerBody*>(PhysicsBody)->SetJumpDurationFrameCount(InCount);
	}
}

int32 UQuarkPlatformerBodyComponent::GetJumpDurationFrameCount() const
{
	return PhysicsBody ? static_cast<QPlatformerBody*>(PhysicsBody)->GetJumpDurationFrameCount() : JumpDurationFrameCount;
}

void UQuarkPlatformerBodyComponent::SetJumpGravityMultiplier(float InMultiplier)
{
	JumpGravityMultiplier = InMultiplier;
	if (PhysicsBody)
	{
		static_cast<QPlatformerBody*>(PhysicsBody)->SetJumpGravityMultiplier(InMultiplier);
	}
}

float UQuarkPlatformerBodyComponent::GetJumpGravityMultiplier() const
{
	return PhysicsBody ? static_cast<QPlatformerBody*>(PhysicsBody)->GetJumpGravityMultiplier() : JumpGravityMultiplier;
}

void UQuarkPlatformerBodyComponent::SetJumpFallGravityMultiplier(float InMultiplier)
{
	JumpFallGravityMultiplier = InMultiplier;
	if (PhysicsBody)
	{
		static_cast<QPlatformerBody*>(PhysicsBody)->SetJumpFallGravityMultiplier(InMultiplier);
	}
}

float UQuarkPlatformerBodyComponent::GetJumpFallGravityMultiplier() const
{
	return PhysicsBody ? static_cast<QPlatformerBody*>(PhysicsBody)->GetJumpFallGravityMultiplier() : JumpFallGravityMultiplier;
}

void UQuarkPlatformerBodyComponent::Jump(float Force, bool bUnconditional)
{
	if (PhysicsBody)
	{
		float PPU = OwnerWorld ? OwnerWorld->PixelsPerUnit : 1.0f;
		float ActualForce = (Force > 0.0f) ? Force * PPU : JumpForceValue * PPU;
		static_cast<QPlatformerBody*>(PhysicsBody)->Jump(ActualForce, bUnconditional);
	}
}

void UQuarkPlatformerBodyComponent::ReleaseJump()
{
	if (PhysicsBody)
	{
		static_cast<QPlatformerBody*>(PhysicsBody)->ReleaseJump();
	}
}

bool UQuarkPlatformerBodyComponent::IsJumping() const
{
	// QPlatformerBody doesn't have GetIsJumping(). We check via !IsJumpReleased().
	if (PhysicsBody)
	{
		return !static_cast<QPlatformerBody*>(PhysicsBody)->GetIsJumpReleased();
	}
	return false;
}

bool UQuarkPlatformerBodyComponent::IsJumpReleased() const
{
	return PhysicsBody ? static_cast<QPlatformerBody*>(PhysicsBody)->GetIsJumpReleased() : true;
}

// Platforms
void UQuarkPlatformerBodyComponent::SetSpecificPlatformLayers(int32 LayersBit)
{
	PlatformLayersBit = LayersBit;
	if (PhysicsBody)
	{
		static_cast<QPlatformerBody*>(PhysicsBody)->SetSpecificPlatformLayers(LayersBit);
	}
}

int32 UQuarkPlatformerBodyComponent::GetSpecificPlatformLayers() const
{
	return PhysicsBody ? static_cast<QPlatformerBody*>(PhysicsBody)->GetSpecificPlatformLayers() : PlatformLayersBit;
}

void UQuarkPlatformerBodyComponent::SetMovingFloorSnapOffset(float InOffset)
{
	MovingFloorSnapOffset = InOffset;
	if (PhysicsBody)
	{
		float PPU = OwnerWorld ? OwnerWorld->PixelsPerUnit : 1.0f;
		static_cast<QPlatformerBody*>(PhysicsBody)->SetMovingFloorSnapOffset(InOffset * PPU);
	}
}

float UQuarkPlatformerBodyComponent::GetMovingFloorSnapOffset() const
{
	if (PhysicsBody)
	{
		float PPU = OwnerWorld ? OwnerWorld->PixelsPerUnit : 1.0f;
		return static_cast<QPlatformerBody*>(PhysicsBody)->GetMovingFloorSnapOffset() / PPU;
	}
	return MovingFloorSnapOffset;
}

// Collision test helpers
bool UQuarkPlatformerBodyComponent::GetFloorTest(float Offset)
{
	if (!PhysicsBody) return false;
	float PPU = OwnerWorld ? OwnerWorld->PixelsPerUnit : 1.0f;
	auto Info = static_cast<QPlatformerBody*>(PhysicsBody)->GetFloor(Offset * PPU);
	return Info.body != nullptr;
}

bool UQuarkPlatformerBodyComponent::GetCeilingTest(float Offset)
{
	if (!PhysicsBody) return false;
	float PPU = OwnerWorld ? OwnerWorld->PixelsPerUnit : 1.0f;
	auto Info = static_cast<QPlatformerBody*>(PhysicsBody)->GetCeiling(Offset * PPU);
	return Info.body != nullptr;
}

bool UQuarkPlatformerBodyComponent::GetRightWallTest(float Offset)
{
	if (!PhysicsBody) return false;
	float PPU = OwnerWorld ? OwnerWorld->PixelsPerUnit : 1.0f;
	auto Info = static_cast<QPlatformerBody*>(PhysicsBody)->GetRightWall(Offset * PPU);
	return Info.body != nullptr;
}

bool UQuarkPlatformerBodyComponent::GetLeftWallTest(float Offset)
{
	if (!PhysicsBody) return false;
	float PPU = OwnerWorld ? OwnerWorld->PixelsPerUnit : 1.0f;
	auto Info = static_cast<QPlatformerBody*>(PhysicsBody)->GetLeftWall(Offset * PPU);
	return Info.body != nullptr;
}
