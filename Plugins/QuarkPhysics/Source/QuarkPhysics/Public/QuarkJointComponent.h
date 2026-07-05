#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuarkPhysics/qjoint.h"
#include "QuarkJointComponent.generated.h"

class UQuarkRigidBodyComponent;

UCLASS(ClassGroup=(QuarkPhysics), meta=(BlueprintSpawnableComponent))
class QUARKPHYSICS_API UQuarkJointComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UQuarkJointComponent();

	virtual void BeginPlay() override;
	virtual void DestroyComponent(bool bPromoteChildren = false) override;

	QJoint* GetPhysicsJoint() const { return PhysicsJoint; }

	// Body references
	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	void SetBodyA(UQuarkRigidBodyComponent* InBodyA);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	UQuarkRigidBodyComponent* GetBodyA() const { return BodyA; }

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	void SetBodyB(UQuarkRigidBodyComponent* InBodyB);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	UQuarkRigidBodyComponent* GetBodyB() const { return BodyB; }

	// Anchor positions (in UE world units, converted to pixels internally)
	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	void SetAnchorAPosition(FVector2D InWorldPosition);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	FVector2D GetAnchorAPosition() const;

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	void SetAnchorBPosition(FVector2D InWorldPosition);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	FVector2D GetAnchorBPosition() const;

	// Joint properties
	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	void SetRigidity(float InRigidity);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	float GetRigidity() const;

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	void SetLength(float InLength);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	float GetLength() const;

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	void SetBalance(float InBalance);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	float GetBalance() const;

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	void SetGrooveEnabled(bool bEnabled);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	bool GetGrooveEnabled() const;

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	void SetCollisionEnabled(bool bEnabled);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	bool GetCollisionEnabled() const;

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	void SetEnabled(bool bEnabled);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	bool GetEnabled() const;

	// Editable properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuarkPhysics")
	UQuarkRigidBodyComponent* BodyA = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuarkPhysics")
	UQuarkRigidBodyComponent* BodyB = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuarkPhysics")
	FVector2D AnchorA = FVector2D(0, 0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuarkPhysics")
	FVector2D AnchorB = FVector2D(0, 0);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuarkPhysics", meta=(ClampMin="0.0", ClampMax="1.0"))
	float Rigidity = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuarkPhysics")
	float Length = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuarkPhysics", meta=(ClampMin="0.0", ClampMax="1.0"))
	float Balance = 0.5f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuarkPhysics")
	bool bGrooveEnabled = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuarkPhysics")
	bool bCollisionEnabled = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuarkPhysics")
	bool bJointEnabled = true;

protected:
	QJoint* PhysicsJoint = nullptr;

	UPROPERTY()
	UQuarkWorldComponent* OwnerWorld = nullptr;

	float GetPPU() const;
};
