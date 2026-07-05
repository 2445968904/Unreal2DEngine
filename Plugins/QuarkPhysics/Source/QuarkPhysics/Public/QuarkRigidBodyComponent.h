#pragma once

#include "CoreMinimal.h"
#include "QuarkBodyComponent.h"
#include "QuarkPhysics/qrigidbody.h"
#include "QuarkRigidBodyComponent.generated.h"

UCLASS(ClassGroup=(QuarkPhysics), meta=(BlueprintSpawnableComponent))
class QUARKPHYSICS_API UQuarkRigidBodyComponent : public UQuarkBodyComponent
{
	GENERATED_BODY()

public:
	UQuarkRigidBodyComponent();

	virtual void BeginPlay() override;

	QRigidBody* GetRigidBody() const { return static_cast<QRigidBody*>(PhysicsBody); }

	// RigidBody-specific
	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	void ApplyForce(FVector2D InForce);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	void ApplyImpulse(FVector2D InImpulse);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	void SetFixedRotation(bool bFixed);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	bool GetFixedRotation() const;

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	void SetKinematic(bool bKinematic);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	bool GetKinematic() const;

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	void SetVelocityLimit(float Limit);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	FVector2D GetLinearVelocity() const;

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	void SetLinearVelocity(FVector2D InVelocity);

	// Quick mesh creation helpers
	UFUNCTION(BlueprintCallable, Category="QuarkPhysics|Mesh")
	void AddCircleMesh(float Radius, FVector2D CenterOffset = FVector2D(0,0));

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics|Mesh")
	void AddRectMesh(FVector2D Size, FVector2D CenterOffset = FVector2D(0,0));

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics|Mesh")
	void AddPolygonMesh(float Radius, int32 SideCount, FVector2D CenterOffset = FVector2D(0,0));

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuarkPhysics")
	bool bFixedRotation = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuarkPhysics")
	bool bKinematic = false;
};
