#pragma once

#include "CoreMinimal.h"
#include "QuarkRigidBodyComponent.h"
#include "QuarkPhysics/extensions/qplatformerbody.h"
#include "QuarkPlatformerBodyComponent.generated.h"

UCLASS(ClassGroup=(QuarkPhysics), meta=(BlueprintSpawnableComponent))
class QUARKPHYSICS_API UQuarkPlatformerBodyComponent : public UQuarkRigidBodyComponent
{
	GENERATED_BODY()

public:
	UQuarkPlatformerBodyComponent();

	virtual void BeginPlay() override;

	QPlatformerBody* GetPlatformerBody() const { return static_cast<QPlatformerBody*>(PhysicsBody); }

	// Floor
	UFUNCTION(BlueprintCallable, Category="QuarkPhysics|Platformer")
	void SetFloorMaxAngle(float InRadians);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics|Platformer")
	float GetFloorMaxAngle() const;

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics|Platformer")
	void SetFloorMaxAngleDegree(float InDegrees);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics|Platformer")
	float GetFloorMaxAngleDegree() const;

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics|Platformer")
	bool IsOnFloor() const;

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics|Platformer")
	bool IsOnCeiling() const;

	// Gravity
	UFUNCTION(BlueprintCallable, Category="QuarkPhysics|Platformer")
	void SetGravity(FVector2D InGravity);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics|Platformer")
	FVector2D GetGravity() const;

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics|Platformer")
	void SetGravityMultiplier(float InMultiplier);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics|Platformer")
	float GetGravityMultiplier() const;

	// Walk
	UFUNCTION(BlueprintCallable, Category="QuarkPhysics|Platformer")
	void SetWalkSpeed(float InSpeed);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics|Platformer")
	float GetWalkSpeed() const;

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics|Platformer")
	void SetWalkAccelerationRate(float InRate);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics|Platformer")
	float GetWalkAccelerationRate() const;

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics|Platformer")
	void SetWalkDecelerationRate(float InRate);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics|Platformer")
	float GetWalkDecelerationRate() const;

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics|Platformer")
	void Walk(int32 Side);

	// Controller velocities
	UFUNCTION(BlueprintCallable, Category="QuarkPhysics|Platformer")
	void SetControllerHorizontalVelocity(FVector2D InVelocity);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics|Platformer")
	FVector2D GetControllerHorizontalVelocity() const;

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics|Platformer")
	void SetControllerVerticalVelocity(FVector2D InVelocity);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics|Platformer")
	FVector2D GetControllerVerticalVelocity() const;

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics|Platformer")
	bool IsFalling() const;

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics|Platformer")
	bool IsRising() const;

	// Jump
	UFUNCTION(BlueprintCallable, Category="QuarkPhysics|Platformer")
	void SetJumpForce(float InForce);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics|Platformer")
	float GetJumpForce() const;

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics|Platformer")
	void SetMaxJumpCount(int32 InCount);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics|Platformer")
	int32 GetMaxJumpCount() const;

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics|Platformer")
	void SetJumpDurationFrameCount(int32 InCount);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics|Platformer")
	int32 GetJumpDurationFrameCount() const;

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics|Platformer")
	void SetJumpGravityMultiplier(float InMultiplier);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics|Platformer")
	float GetJumpGravityMultiplier() const;

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics|Platformer")
	void SetJumpFallGravityMultiplier(float InMultiplier);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics|Platformer")
	float GetJumpFallGravityMultiplier() const;

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics|Platformer")
	void Jump(float Force = 0.0f, bool bUnconditional = false);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics|Platformer")
	void ReleaseJump();

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics|Platformer")
	bool IsJumping() const;

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics|Platformer")
	bool IsJumpReleased() const;

	// Platforms
	UFUNCTION(BlueprintCallable, Category="QuarkPhysics|Platformer")
	void SetSpecificPlatformLayers(int32 LayersBit);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics|Platformer")
	int32 GetSpecificPlatformLayers() const;

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics|Platformer")
	void SetMovingFloorSnapOffset(float InOffset);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics|Platformer")
	float GetMovingFloorSnapOffset() const;

	// Collision test helpers
	UFUNCTION(BlueprintCallable, Category="QuarkPhysics|Platformer")
	bool GetFloorTest(float Offset);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics|Platformer")
	bool GetCeilingTest(float Offset);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics|Platformer")
	bool GetRightWallTest(float Offset);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics|Platformer")
	bool GetLeftWallTest(float Offset);

	// Editable properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuarkPhysics|Platformer")
	FVector2D Gravity = FVector2D(0, 0.3f);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuarkPhysics|Platformer")
	float GravityMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuarkPhysics|Platformer")
	float WalkSpeed = 3.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuarkPhysics|Platformer")
	float WalkAccelerationRate = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuarkPhysics|Platformer")
	float WalkDecelerationRate = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuarkPhysics|Platformer")
	float JumpForceValue = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuarkPhysics|Platformer")
	int32 MaxJumpCount = 2;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuarkPhysics|Platformer")
	int32 JumpDurationFrameCount = 30;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuarkPhysics|Platformer")
	float JumpGravityMultiplier = 0.4f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuarkPhysics|Platformer")
	float JumpFallGravityMultiplier = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuarkPhysics|Platformer")
	float FloorMaxAngleDegrees = 45.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuarkPhysics|Platformer")
	float MovingFloorSnapOffset = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuarkPhysics|Platformer")
	int32 PlatformLayersBit = 0;
};
