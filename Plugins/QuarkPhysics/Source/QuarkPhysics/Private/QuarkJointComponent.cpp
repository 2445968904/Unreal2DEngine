#include "QuarkJointComponent.h"
#include "QuarkRigidBodyComponent.h"
#include "QuarkWorldComponent.h"
#include "GameFramework/Actor.h"

UQuarkJointComponent::UQuarkJointComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bAutoActivate = true;
}

void UQuarkJointComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* Owner = GetOwner();
	if (Owner)
	{
		OwnerWorld = Owner->FindComponentByClass<UQuarkWorldComponent>();
	}

	float PPU = GetPPU();

	QRigidBody* RigidBodyA = BodyA ? static_cast<QRigidBody*>(BodyA->GetPhysicsBody()) : nullptr;
	QRigidBody* RigidBodyB = BodyB ? static_cast<QRigidBody*>(BodyB->GetPhysicsBody()) : nullptr;

	QVector AnchorAPos(AnchorA.X * PPU, AnchorA.Y * PPU);
	QVector AnchorBPos(AnchorB.X * PPU, AnchorB.Y * PPU);

	PhysicsJoint = new QJoint(RigidBodyA, AnchorAPos, AnchorBPos, RigidBodyB);
	PhysicsJoint->manualDeletion = true;
	PhysicsJoint->SetRigidity(Rigidity);
	PhysicsJoint->SetLength(Length * PPU);
	PhysicsJoint->SetBalance(Balance);
	PhysicsJoint->SetGrooveEnabled(bGrooveEnabled);
	PhysicsJoint->SetCollisionEnabled(bCollisionEnabled);
	PhysicsJoint->SetEnabled(bJointEnabled);

	if (OwnerWorld && OwnerWorld->GetPhysicsWorld())
	{
		OwnerWorld->GetPhysicsWorld()->AddJoint(PhysicsJoint);
	}
}

void UQuarkJointComponent::DestroyComponent(bool bPromoteChildren)
{
	if (PhysicsJoint)
	{
		if (OwnerWorld && OwnerWorld->GetPhysicsWorld())
		{
			OwnerWorld->GetPhysicsWorld()->RemoveJoint(PhysicsJoint);
		}
		if (!PhysicsJoint->manualDeletion)
		{
			delete PhysicsJoint;
		}
		PhysicsJoint = nullptr;
	}
	Super::DestroyComponent(bPromoteChildren);
}

void UQuarkJointComponent::SetBodyA(UQuarkRigidBodyComponent* InBodyA)
{
	BodyA = InBodyA;
	if (PhysicsJoint)
	{
		PhysicsJoint->SetBodyA(InBodyA ? static_cast<QRigidBody*>(InBodyA->GetPhysicsBody()) : nullptr);
	}
}

void UQuarkJointComponent::SetBodyB(UQuarkRigidBodyComponent* InBodyB)
{
	BodyB = InBodyB;
	if (PhysicsJoint)
	{
		PhysicsJoint->SetBodyB(InBodyB ? static_cast<QRigidBody*>(InBodyB->GetPhysicsBody()) : nullptr);
	}
}

void UQuarkJointComponent::SetAnchorAPosition(FVector2D InWorldPosition)
{
	AnchorA = InWorldPosition;
	if (PhysicsJoint)
	{
		float PPU = GetPPU();
		PhysicsJoint->SetAnchorAPosition(QVector(InWorldPosition.X * PPU, InWorldPosition.Y * PPU));
	}
}

FVector2D UQuarkJointComponent::GetAnchorAPosition() const
{
	if (PhysicsJoint)
	{
		QVector Pos = PhysicsJoint->GetAnchorAPosition();
		float PPU = GetPPU();
		return FVector2D(Pos.x / PPU, Pos.y / PPU);
	}
	return AnchorA;
}

void UQuarkJointComponent::SetAnchorBPosition(FVector2D InWorldPosition)
{
	AnchorB = InWorldPosition;
	if (PhysicsJoint)
	{
		float PPU = GetPPU();
		PhysicsJoint->SetAnchorBPosition(QVector(InWorldPosition.X * PPU, InWorldPosition.Y * PPU));
	}
}

FVector2D UQuarkJointComponent::GetAnchorBPosition() const
{
	if (PhysicsJoint)
	{
		QVector Pos = PhysicsJoint->GetAnchorBPosition();
		float PPU = GetPPU();
		return FVector2D(Pos.x / PPU, Pos.y / PPU);
	}
	return AnchorB;
}

void UQuarkJointComponent::SetRigidity(float InRigidity)
{
	Rigidity = InRigidity;
	if (PhysicsJoint) PhysicsJoint->SetRigidity(InRigidity);
}

float UQuarkJointComponent::GetRigidity() const
{
	return PhysicsJoint ? PhysicsJoint->GetRigidity() : Rigidity;
}

void UQuarkJointComponent::SetLength(float InLength)
{
	Length = InLength;
	if (PhysicsJoint)
	{
		float PPU = GetPPU();
		PhysicsJoint->SetLength(InLength * PPU);
	}
}

float UQuarkJointComponent::GetLength() const
{
	if (PhysicsJoint)
	{
		float PPU = GetPPU();
		return PhysicsJoint->GetLength() / PPU;
	}
	return Length;
}

void UQuarkJointComponent::SetBalance(float InBalance)
{
	Balance = InBalance;
	if (PhysicsJoint) PhysicsJoint->SetBalance(InBalance);
}

float UQuarkJointComponent::GetBalance() const
{
	return PhysicsJoint ? PhysicsJoint->GetBalance() : Balance;
}

void UQuarkJointComponent::SetGrooveEnabled(bool bEnabled)
{
	bGrooveEnabled = bEnabled;
	if (PhysicsJoint) PhysicsJoint->SetGrooveEnabled(bEnabled);
}

bool UQuarkJointComponent::GetGrooveEnabled() const
{
	return PhysicsJoint ? PhysicsJoint->GetGrooveEnabled() : bGrooveEnabled;
}

void UQuarkJointComponent::SetCollisionEnabled(bool bEnabled)
{
	bCollisionEnabled = bEnabled;
	if (PhysicsJoint) PhysicsJoint->SetCollisionEnabled(bEnabled);
}

bool UQuarkJointComponent::GetCollisionEnabled() const
{
	return PhysicsJoint ? PhysicsJoint->GetCollisionEnabled() : bCollisionEnabled;
}

void UQuarkJointComponent::SetEnabled(bool bEnabled)
{
	bJointEnabled = bEnabled;
	if (PhysicsJoint) PhysicsJoint->SetEnabled(bEnabled);
}

bool UQuarkJointComponent::GetEnabled() const
{
	return PhysicsJoint ? PhysicsJoint->GetEnabled() : bJointEnabled;
}

float UQuarkJointComponent::GetPPU() const
{
	return OwnerWorld ? OwnerWorld->PixelsPerUnit : 1.0f;
}
