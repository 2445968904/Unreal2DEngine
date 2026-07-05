#include "QuarkBodyComponent.h"
#include "QuarkWorldComponent.h"
#include "GameFramework/Actor.h"

UQuarkBodyComponent::UQuarkBodyComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bAutoActivate = true;
}

void UQuarkBodyComponent::BeginPlay()
{
	Super::BeginPlay();

	// Find the world component on the same actor or parent
	AActor* Owner = GetOwner();
	if (Owner)
	{
		OwnerWorld = Owner->FindComponentByClass<UQuarkWorldComponent>();
	}
}

void UQuarkBodyComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PhysicsBody)
	{
		// Sync physics body position back to UE actor transform
		AActor* Owner = GetOwner();
		if (Owner && PhysicsBody->GetMode() == QBody::Modes::DYNAMIC)
		{
			QVector Pos = PhysicsBody->GetPosition();
			float Rot = PhysicsBody->GetRotationDegree();

			if (OwnerWorld)
			{
				float PPU = OwnerWorld->PixelsPerUnit;
				Owner->SetActorLocation(FVector(Pos.x / PPU, 0.0f, Pos.y / PPU));
			}
			Owner->SetActorRotation(FRotator(0.0f, -Rot, 0.0f));
		}
	}
}

void UQuarkBodyComponent::SetPosition(FVector2D InPosition)
{
	if (PhysicsBody)
	{
		if (OwnerWorld)
		{
			float PPU = OwnerWorld->PixelsPerUnit;
			PhysicsBody->SetPosition(QVector(InPosition.X * PPU, InPosition.Y * PPU));
		}
		else
		{
			PhysicsBody->SetPosition(QVector(InPosition.X, InPosition.Y));
		}
	}
}

FVector2D UQuarkBodyComponent::GetPosition() const
{
	if (PhysicsBody)
	{
		QVector Pos = PhysicsBody->GetPosition();
		if (OwnerWorld)
		{
			float PPU = OwnerWorld->PixelsPerUnit;
			return FVector2D(Pos.x / PPU, Pos.y / PPU);
		}
		return FVector2D(Pos.x, Pos.y);
	}
	return FVector2D::ZeroVector;
}

void UQuarkBodyComponent::SetRotation(float InDegrees)
{
	if (PhysicsBody)
	{
		PhysicsBody->SetRotationDegree(InDegrees);
	}
}

float UQuarkBodyComponent::GetRotation() const
{
	if (PhysicsBody)
	{
		return PhysicsBody->GetRotationDegree();
	}
	return 0.0f;
}

void UQuarkBodyComponent::SetModeStatic()
{
	if (PhysicsBody) PhysicsBody->SetMode(QBody::Modes::STATIC);
}

void UQuarkBodyComponent::SetModeDynamic()
{
	if (PhysicsBody) PhysicsBody->SetMode(QBody::Modes::DYNAMIC);
}

bool UQuarkBodyComponent::IsStatic() const
{
	return PhysicsBody && PhysicsBody->GetMode() == QBody::Modes::STATIC;
}

bool UQuarkBodyComponent::IsDynamic() const
{
	return PhysicsBody && PhysicsBody->GetMode() == QBody::Modes::DYNAMIC;
}

void UQuarkBodyComponent::SetMass(float InMass)
{
	if (PhysicsBody) PhysicsBody->SetMass(InMass);
}

float UQuarkBodyComponent::GetMass() const
{
	return PhysicsBody ? PhysicsBody->GetMass() : 0.0f;
}

void UQuarkBodyComponent::SetFriction(float InFriction)
{
	if (PhysicsBody) PhysicsBody->SetFriction(InFriction);
}

float UQuarkBodyComponent::GetFriction() const
{
	return PhysicsBody ? PhysicsBody->GetFriction() : 0.0f;
}

void UQuarkBodyComponent::SetRestitution(float InRestitution)
{
	if (PhysicsBody) PhysicsBody->SetRestitution(InRestitution);
}

float UQuarkBodyComponent::GetRestitution() const
{
	return PhysicsBody ? PhysicsBody->GetRestitution() : 0.0f;
}

void UQuarkBodyComponent::SetAirFriction(float InAirFriction)
{
	if (PhysicsBody) PhysicsBody->SetAirFriction(InAirFriction);
}

float UQuarkBodyComponent::GetAirFriction() const
{
	return PhysicsBody ? PhysicsBody->GetAirFriction() : 0.0f;
}

void UQuarkBodyComponent::SetLayersBit(int32 Bit)
{
	if (PhysicsBody) PhysicsBody->SetLayersBit(Bit);
}

int32 UQuarkBodyComponent::GetLayersBit() const
{
	return PhysicsBody ? PhysicsBody->GetLayersBit() : 0;
}

void UQuarkBodyComponent::SetCollidableLayersBit(int32 Bit)
{
	if (PhysicsBody) PhysicsBody->SetCollidableLayersBit(Bit);
}

int32 UQuarkBodyComponent::GetCollidableLayersBit() const
{
	return PhysicsBody ? PhysicsBody->GetCollidableLayersBit() : 0;
}

void UQuarkBodyComponent::SetEnabled(bool bEnabled)
{
	if (PhysicsBody) PhysicsBody->SetEnabled(bEnabled);
}

bool UQuarkBodyComponent::GetEnabled() const
{
	return PhysicsBody ? PhysicsBody->GetEnabled() : false;
}

void UQuarkBodyComponent::DestroyComponent(bool bPromoteChildren)
{
	if (PhysicsBody)
	{
		if (OwnerWorld && OwnerWorld->GetPhysicsWorld())
		{
			OwnerWorld->GetPhysicsWorld()->RemoveBody(PhysicsBody);
		}
		if (!PhysicsBody->manualDeletion)
		{
			delete PhysicsBody;
		}
		PhysicsBody = nullptr;
	}
	Super::DestroyComponent(bPromoteChildren);
}
