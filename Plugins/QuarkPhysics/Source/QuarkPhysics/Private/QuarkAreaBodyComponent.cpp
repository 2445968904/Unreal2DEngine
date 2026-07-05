#include "QuarkAreaBodyComponent.h"
#include "QuarkWorldComponent.h"
#include "GameFramework/Actor.h"

UQuarkAreaBodyComponent::UQuarkAreaBodyComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bAutoActivate = true;
}

void UQuarkAreaBodyComponent::BeginPlay()
{
	Super::BeginPlay();

	QAreaBody* AreaBody = new QAreaBody();
	AreaBody->manualDeletion = true;
	AreaBody->SetGravityFreeEnabled(bGravityFree);

	float PPU = OwnerWorld ? OwnerWorld->PixelsPerUnit : 1.0f;
	AreaBody->SetLinearForceToApply(QVector(LinearForce.X * PPU, LinearForce.Y * PPU));

	// Bind collision enter/exit callbacks to forward to UE events
	AreaBody->CollisionEnterEventListener = [this](QAreaBody*, QBody* CollidedBody) {
		OnPhysicsCollisionEnter(CollidedBody);
	};
	AreaBody->CollisionExitEventListener = [this](QAreaBody*, QBody* CollidedBody) {
		OnPhysicsCollisionExit(CollidedBody);
	};

	PhysicsBody = AreaBody;

	if (OwnerWorld)
	{
		OwnerWorld->AddBody(this);
	}
}

void UQuarkAreaBodyComponent::SetGravityFreeEnabled(bool bEnabled)
{
	bGravityFree = bEnabled;
	if (PhysicsBody)
	{
		static_cast<QAreaBody*>(PhysicsBody)->SetGravityFreeEnabled(bEnabled);
	}
}

bool UQuarkAreaBodyComponent::GetGravityFreeEnabled() const
{
	return PhysicsBody ? static_cast<QAreaBody*>(PhysicsBody)->GetGravityFreeEnabled() : false;
}

void UQuarkAreaBodyComponent::SetLinearForceToApply(FVector2D InForce)
{
	LinearForce = InForce;
	if (PhysicsBody)
	{
		float PPU = OwnerWorld ? OwnerWorld->PixelsPerUnit : 1.0f;
		static_cast<QAreaBody*>(PhysicsBody)->SetLinearForceToApply(QVector(InForce.X * PPU, InForce.Y * PPU));
	}
}

FVector2D UQuarkAreaBodyComponent::GetLinearForceToApply() const
{
	if (PhysicsBody)
	{
		QVector Force = static_cast<QAreaBody*>(PhysicsBody)->GetLinearForceToApply();
		float PPU = OwnerWorld ? OwnerWorld->PixelsPerUnit : 1.0f;
		return FVector2D(Force.x / PPU, Force.y / PPU);
	}
	return FVector2D::ZeroVector;
}

TArray<UQuarkBodyComponent*> UQuarkAreaBodyComponent::GetBodiesInArea() const
{
	TArray<UQuarkBodyComponent*> Result;
	if (!PhysicsBody) return Result;

	std::vector<QBody*> Bodies = static_cast<QAreaBody*>(PhysicsBody)->GetBodies();
	for (QBody* Body : Bodies)
	{
		UQuarkBodyComponent* Comp = FindBodyComponent(Body);
		if (Comp)
		{
			Result.Add(Comp);
		}
	}
	return Result;
}

bool UQuarkAreaBodyComponent::HasBodyInArea(UQuarkBodyComponent* Body) const
{
	if (!PhysicsBody || !Body || !Body->GetPhysicsBody()) return false;
	return static_cast<QAreaBody*>(PhysicsBody)->HasBody(Body->GetPhysicsBody());
}

void UQuarkAreaBodyComponent::OnPhysicsCollisionEnter(QBody* CollidedBody)
{
	UQuarkBodyComponent* Comp = FindBodyComponent(CollidedBody);
	if (Comp)
	{
		ReceiveOnAreaBodyEnter(Comp);
	}
}

void UQuarkAreaBodyComponent::OnPhysicsCollisionExit(QBody* CollidedBody)
{
	UQuarkBodyComponent* Comp = FindBodyComponent(CollidedBody);
	if (Comp)
	{
		ReceiveOnAreaBodyExit(Comp);
	}
}

UQuarkBodyComponent* UQuarkAreaBodyComponent::FindBodyComponent(QBody* InPhysicsBody) const
{
	if (!InPhysicsBody) return nullptr;

	// Search all QuarkBodyComponents in the world for the matching physics body
	UWorld* World = GetWorld();
	if (!World) return nullptr;

	for (TObjectIterator<UQuarkBodyComponent> It; It; ++It)
	{
		UQuarkBodyComponent* Comp = *It;
		if (Comp->GetPhysicsBody() == InPhysicsBody && Comp->GetWorld() == World)
		{
			return Comp;
		}
	}
	return nullptr;
}
