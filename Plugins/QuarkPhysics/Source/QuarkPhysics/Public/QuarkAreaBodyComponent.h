#pragma once

#include "CoreMinimal.h"
#include "QuarkBodyComponent.h"
#include "QuarkPhysics/qareabody.h"
#include "QuarkAreaBodyComponent.generated.h"

UCLASS(ClassGroup=(QuarkPhysics), meta=(BlueprintSpawnableComponent))
class QUARKPHYSICS_API UQuarkAreaBodyComponent : public UQuarkBodyComponent
{
	GENERATED_BODY()

public:
	UQuarkAreaBodyComponent();

	virtual void BeginPlay() override;

	QAreaBody* GetAreaBody() const { return static_cast<QAreaBody*>(PhysicsBody); }

	// Gravity-free zone
	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	void SetGravityFreeEnabled(bool bEnabled);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	bool GetGravityFreeEnabled() const;

	// Linear force applied to bodies inside the area
	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	void SetLinearForceToApply(FVector2D InForce);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	FVector2D GetLinearForceToApply() const;

	// Query bodies inside the area
	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	TArray<UQuarkBodyComponent*> GetBodiesInArea() const;

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	bool HasBodyInArea(UQuarkBodyComponent* Body) const;

	// Blueprint events
	UFUNCTION(BlueprintImplementableEvent, Category="QuarkPhysics", meta=(DisplayName="On Area Body Enter"))
	void ReceiveOnAreaBodyEnter(UQuarkBodyComponent* EnteredBody);

	UFUNCTION(BlueprintImplementableEvent, Category="QuarkPhysics", meta=(DisplayName="On Area Body Exit"))
	void ReceiveOnAreaBodyExit(UQuarkBodyComponent* ExitedBody);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuarkPhysics")
	bool bGravityFree = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuarkPhysics")
	FVector2D LinearForce = FVector2D(0, 0);

private:
	void OnPhysicsCollisionEnter(QBody* CollidedBody);
	void OnPhysicsCollisionExit(QBody* CollidedBody);

	UQuarkBodyComponent* FindBodyComponent(QBody* InPhysicsBody) const;
};
