#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuarkPhysics/qworld.h"
#include "QuarkPhysics/qraycast.h"
#include "QuarkWorldComponent.generated.h"

class UQuarkJointComponent;
class UQuarkSpringComponent;
class UQuarkBodyComponent;

UCLASS(ClassGroup=(QuarkPhysics), meta=(BlueprintSpawnableComponent))
class QUARKPHYSICS_API UQuarkWorldComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UQuarkWorldComponent();

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	QWorld* GetPhysicsWorld() const { return PhysicsWorld; }

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	void SetGravity(FVector2D InGravity);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	FVector2D GetGravity() const;

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	void SetIterationCount(int32 Count);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	int32 GetIterationCount() const;

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	void SetTimeScale(float Scale);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	float GetTimeScale() const;

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	void SetSleepingEnabled(bool bEnabled);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	void SetBroadphaseEnabled(bool bEnabled);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	void AddBody(class UQuarkBodyComponent* Body);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	void RemoveBody(class UQuarkBodyComponent* Body);

	// Joint/Spring management
	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	void AddJoint(class UQuarkJointComponent* Joint);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	void RemoveJoint(class UQuarkJointComponent* Joint);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	void AddSpring(class UQuarkSpringComponent* Spring);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	void RemoveSpring(class UQuarkSpringComponent* Spring);

	// Raycast
	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	bool Raycast(FVector2D Start, FVector2D End, TArray<FVector2D>& OutHitPositions, TArray<UQuarkBodyComponent*>& OutHitBodies);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuarkPhysics")
	float PixelsPerUnit = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuarkPhysics", meta=(ClampMin="0.0", ClampMax="10.0"))
	float TimeScale = 1.0f;

protected:
	QWorld* PhysicsWorld = nullptr;

public:
	virtual void DestroyComponent(bool bPromoteChildren = false) override;
};
