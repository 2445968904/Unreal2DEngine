#pragma once

#include "CoreMinimal.h"
#include "ProceduralMeshComponent.h"
#include "QuarkPhysics/qbody.h"
#include "QuarkPhysics/qmesh.h"
#include "QuarkPhysics/qsoftbody.h"
#include "QuarkPhysics/qrigidbody.h"
#include "QuarkPhysicsRendererComponent.generated.h"

UCLASS(ClassGroup=(QuarkPhysics), meta=(BlueprintSpawnableComponent))
class QUARKPHYSICS_API UQuarkPhysicsRendererComponent : public UProceduralMeshComponent
{
	GENERATED_BODY()

public:
	UQuarkPhysicsRendererComponent(const FObjectInitializer& ObjectInitializer);

	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	// Set which body to render (finds first body on owner if null)
	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	void SetTargetBody(class UQuarkBodyComponent* InTargetBody);

	// Rendering options
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuarkPhysics")
	UMaterialInterface* OverrideMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuarkPhysics")
	float DepthOffset = 0.1f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuarkPhysics")
	bool bRenderDebugGizmos = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuarkPhysics")
	FLinearColor Tint = FLinearColor(1, 1, 1, 1);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuarkPhysics")
	float DebugGizmoScale = 1.0f;

protected:
	void UpdateMesh();
	void BuildMeshFromBody(QBody* Body, float PPU);

private:
	UPROPERTY()
	class UQuarkBodyComponent* TargetBody = nullptr;

	UPROPERTY()
	UMaterialInstanceDynamic* DynamicMaterial = nullptr;

	UPROPERTY()
	UMaterialInterface* DefaultMaterial = nullptr;
};
