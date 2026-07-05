#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "QuarkPhysicsDemo.generated.h"

class UQuarkWorldComponent;
class UQuarkRigidBodyComponent;
class UQuarkSoftBodyComponent;
class UQuarkPhysicsRendererComponent;

/**
 * Demonstrates a basic QuarkPhysics scene:
 * - A physics world with gravity
 * - A static ground platform
 * - Dynamic rigid bodies falling from above
 * - A soft body blob
 */
UCLASS()
class AQuarkPhysicsDemo : public AActor
{
	GENERATED_BODY()

public:
	AQuarkPhysicsDemo();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;

	// Physics world
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="QuarkPhysics")
	UQuarkWorldComponent* PhysicsWorld;

	// Ground (static rigid body)
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="QuarkPhysics")
	UQuarkRigidBodyComponent* GroundBody;

	// Ground renderer
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="QuarkPhysics")
	UQuarkPhysicsRendererComponent* GroundRenderer;

	// Falling box 1
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="QuarkPhysics")
	UQuarkRigidBodyComponent* FallingBox1;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="QuarkPhysics")
	UQuarkPhysicsRendererComponent* Box1Renderer;

	// Falling box 2
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="QuarkPhysics")
	UQuarkRigidBodyComponent* FallingBox2;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="QuarkPhysics")
	UQuarkPhysicsRendererComponent* Box2Renderer;

	// Falling circle
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="QuarkPhysics")
	UQuarkRigidBodyComponent* FallingCircle;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="QuarkPhysics")
	UQuarkPhysicsRendererComponent* CircleRenderer;

	// Soft body
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="QuarkPhysics")
	UQuarkSoftBodyComponent* SoftBody;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="QuarkPhysics")
	UQuarkPhysicsRendererComponent* SoftBodyRenderer;

	// Config
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuarkPhysics")
	float PixelsPerUnit = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuarkPhysics")
	bool bSpawnSoftBody = true;
};
