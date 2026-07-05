#pragma once

#include "CoreMinimal.h"
#include "QuarkBodyComponent.h"
#include "QuarkPhysics/qsoftbody.h"
#include "QuarkSoftBodyComponent.generated.h"

UCLASS(ClassGroup=(QuarkPhysics), meta=(BlueprintSpawnableComponent))
class QUARKPHYSICS_API UQuarkSoftBodyComponent : public UQuarkBodyComponent
{
	GENERATED_BODY()

public:
	UQuarkSoftBodyComponent();

	virtual void BeginPlay() override;

	QSoftBody* GetSoftBody() const { return static_cast<QSoftBody*>(PhysicsBody); }

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	void SetRigidity(float InRigidity);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	float GetRigidity() const;

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	void SetAreaPreservingEnabled(bool bEnabled);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	bool GetAreaPreservingEnabled() const;

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	void SetShapeMatchingEnabled(bool bEnabled);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	bool GetShapeMatchingEnabled() const;

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	void SetShapeMatchingRate(float Rate);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics")
	void SetSelfCollisionsEnabled(bool bEnabled);

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics|Mesh")
	void AddSoftRectMesh(FVector2D Size, FVector2D CenterOffset = FVector2D(0,0), FVector2D Grid = FVector2D(3,3));

	UFUNCTION(BlueprintCallable, Category="QuarkPhysics|Mesh")
	void AddSoftPolygonMesh(float Radius, int32 SideCount, FVector2D CenterOffset = FVector2D(0,0), int32 PolarGrid = 2);

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuarkPhysics")
	float Rigidity = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuarkPhysics")
	bool bAreaPreserving = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuarkPhysics")
	bool bShapeMatching = false;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="QuarkPhysics", meta=(ClampMin="0.0", ClampMax="1.0"))
	float ShapeMatchingRate = 0.4f;
};
