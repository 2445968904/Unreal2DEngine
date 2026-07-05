#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuarkPhysics/qbody.h"
#include "QuarkWorldComponent.h"
#include "QuarkBodyComponent.generated.h"

UCLASS(ClassGroup=(QuarkPhysics), Abstract)
class QUARKPHYSICS_API UQuarkBodyComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UQuarkBodyComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	QBody* GetPhysicsBody() const { return PhysicsBody; }

	// Transform
	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	void SetPosition(FVector2D InPosition);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	FVector2D GetPosition() const;

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	void SetRotation(float InDegrees);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	float GetRotation() const;

	// Mode
	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	void SetModeStatic();

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	void SetModeDynamic();

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	bool IsStatic() const;

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	bool IsDynamic() const;

	// Material
	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	void SetMass(float InMass);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	float GetMass() const;

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	void SetFriction(float InFriction);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	float GetFriction() const;

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	void SetRestitution(float InRestitution);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	float GetRestitution() const;

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	void SetAirFriction(float InAirFriction);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	float GetAirFriction() const;

	// Collision layers
	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	void SetLayersBit(int32 Bit);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	int32 GetLayersBit() const;

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	void SetCollidableLayersBit(int32 Bit);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	int32 GetCollidableLayersBit() const;

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	void SetEnabled(bool bEnabled);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	bool GetEnabled() const;

	// Blueprint collision event
	UFUNCTION(BlueprintImplementableEvent, Category="QuarkPhysics", meta=(DisplayName="On Collision"))
	void ReceiveOnCollision(FVector2D ContactPosition, FVector2D Normal, float Penetration, UQuarkBodyComponent* OtherBody);

protected:
	QBody* PhysicsBody = nullptr;

	UPROPERTY()
	UQuarkWorldComponent* OwnerWorld = nullptr;

public:
	virtual void DestroyComponent(bool bPromoteChildren = false) override;
};
