#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "QuarkPhysics/qspring.h"
#include "QuarkSpringComponent.generated.h"

class UQuarkBodyComponent;

UCLASS(ClassGroup=(QuarkPhysics), meta=(BlueprintSpawnableComponent))
class QUARKPHYSICS_API UQuarkSpringComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UQuarkSpringComponent();

	virtual void BeginPlay() override;
	virtual void DestroyComponent(bool bPromoteChildren = false) override;

	QSpring* GetPhysicsSpring() const { return PhysicsSpring; }

	// Body references — the spring connects the closest particles of each body
	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	void SetBodyA(UQuarkBodyComponent* InBodyA);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	UQuarkBodyComponent* GetBodyA() const { return BodyA; }

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	void SetBodyB(UQuarkBodyComponent* InBodyB);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	UQuarkBodyComponent* GetBodyB() const { return BodyB; }

	// Spring properties
	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	void SetRigidity(float InRigidity);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	float GetRigidity() const;

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	void SetLength(float InLength);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	float GetLength() const;

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	void SetDistanceLimitEnabled(bool bEnabled);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	bool GetDistanceLimitEnabled() const;

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	void SetMinimumDistanceFactor(float InFactor);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	float GetMinimumDistanceFactor() const;

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	void SetMaximumDistanceFactor(float InFactor);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	float GetMaximumDistanceFactor() const;

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	void SetEnabled(bool bEnabled);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	bool GetEnabled() const;

	// Editable properties
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuarkPhysics")
	UQuarkBodyComponent* BodyA = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuarkPhysics")
	UQuarkBodyComponent* BodyB = nullptr;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuarkPhysics", meta=(ClampMin="0.0", ClampMax="1.0"))
	float Rigidity = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuarkPhysics")
	float SpringLength = 0.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuarkPhysics")
	bool bDistanceLimitEnabled = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuarkPhysics")
	float MinimumDistanceFactor = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuarkPhysics")
	float MaximumDistanceFactor = 4.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuarkPhysics")
	bool bSpringEnabled = true;

protected:
	QSpring* PhysicsSpring = nullptr;

	UPROPERTY()
	UQuarkWorldComponent* OwnerWorld = nullptr;

	float GetPPU() const;
};
