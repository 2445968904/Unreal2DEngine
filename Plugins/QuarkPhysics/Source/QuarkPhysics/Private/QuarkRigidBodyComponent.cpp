#include "QuarkRigidBodyComponent.h"
#include "QuarkWorldComponent.h"

UQuarkRigidBodyComponent::UQuarkRigidBodyComponent()
{
}

void UQuarkRigidBodyComponent::BeginPlay()
{
	Super::BeginPlay();

	PhysicsBody = new QRigidBody();
	PhysicsBody->manualDeletion = true;

	static_cast<QRigidBody*>(PhysicsBody)->SetFixedRotationEnabled(bFixedRotation);
	static_cast<QRigidBody*>(PhysicsBody)->SetKinematicEnabled(bKinematic);

	if (OwnerWorld)
	{
		OwnerWorld->AddBody(this);
	}
}

void UQuarkRigidBodyComponent::ApplyForce(FVector2D InForce)
{
	if (PhysicsBody)
	{
		float PPU = OwnerWorld ? OwnerWorld->PixelsPerUnit : 1.0f;
		static_cast<QRigidBody*>(PhysicsBody)->ApplyForce(QVector(InForce.X * PPU, InForce.Y * PPU));
	}
}

void UQuarkRigidBodyComponent::ApplyImpulse(FVector2D InImpulse)
{
	if (PhysicsBody)
	{
		float PPU = OwnerWorld ? OwnerWorld->PixelsPerUnit : 1.0f;
		static_cast<QRigidBody*>(PhysicsBody)->ApplyImpulse(QVector(InImpulse.X * PPU, InImpulse.Y * PPU), QVector(0,0));
	}
}

void UQuarkRigidBodyComponent::SetFixedRotation(bool bFixed)
{
	bFixedRotation = bFixed;
	if (PhysicsBody)
	{
		static_cast<QRigidBody*>(PhysicsBody)->SetFixedRotationEnabled(bFixed);
	}
}

bool UQuarkRigidBodyComponent::GetFixedRotation() const
{
	return PhysicsBody ? static_cast<QRigidBody*>(PhysicsBody)->GetFixedRotationEnabled() : false;
}

void UQuarkRigidBodyComponent::SetKinematic(bool bKinematic)
{
	this->bKinematic = bKinematic;
	if (PhysicsBody)
	{
		static_cast<QRigidBody*>(PhysicsBody)->SetKinematicEnabled(bKinematic);
	}
}

bool UQuarkRigidBodyComponent::GetKinematic() const
{
	return PhysicsBody ? static_cast<QRigidBody*>(PhysicsBody)->GetKinematicEnabled() : false;
}

void UQuarkRigidBodyComponent::SetVelocityLimit(float Limit)
{
	if (PhysicsBody)
	{
		static_cast<QRigidBody*>(PhysicsBody)->SetVelocityLimit(Limit);
	}
}

FVector2D UQuarkRigidBodyComponent::GetLinearVelocity() const
{
	if (PhysicsBody)
	{
		QVector Vel = PhysicsBody->GetPosition() - PhysicsBody->GetPreviousPosition();
		float PPU = OwnerWorld ? OwnerWorld->PixelsPerUnit : 1.0f;
		return FVector2D(Vel.x / PPU, Vel.y / PPU);
	}
	return FVector2D::ZeroVector;
}

void UQuarkRigidBodyComponent::SetLinearVelocity(FVector2D InVelocity)
{
	if (PhysicsBody)
	{
		float PPU = OwnerWorld ? OwnerWorld->PixelsPerUnit : 1.0f;
		QVector Vel(InVelocity.X * PPU, InVelocity.Y * PPU);
		PhysicsBody->SetPreviousPosition(PhysicsBody->GetPosition() - Vel);
	}
}

void UQuarkRigidBodyComponent::AddCircleMesh(float Radius, FVector2D CenterOffset)
{
	if (PhysicsBody)
	{
		float PPU = OwnerWorld ? OwnerWorld->PixelsPerUnit : 1.0f;
		QMesh* Mesh = QMesh::CreateWithCircle(Radius * PPU, QVector(CenterOffset.X * PPU, CenterOffset.Y * PPU));
		Mesh->manualDeletion = true;
		PhysicsBody->AddMesh(Mesh);
	}
}

void UQuarkRigidBodyComponent::AddRectMesh(FVector2D Size, FVector2D CenterOffset)
{
	if (PhysicsBody)
	{
		float PPU = OwnerWorld ? OwnerWorld->PixelsPerUnit : 1.0f;
		QMesh* Mesh = QMesh::CreateWithRect(QVector(Size.X * PPU, Size.Y * PPU), QVector(CenterOffset.X * PPU, CenterOffset.Y * PPU));
		Mesh->manualDeletion = true;
		PhysicsBody->AddMesh(Mesh);
	}
}

void UQuarkRigidBodyComponent::AddPolygonMesh(float Radius, int32 SideCount, FVector2D CenterOffset)
{
	if (PhysicsBody)
	{
		float PPU = OwnerWorld ? OwnerWorld->PixelsPerUnit : 1.0f;
		QMesh* Mesh = QMesh::CreateWithPolygon(Radius * PPU, SideCount, QVector(CenterOffset.X * PPU, CenterOffset.Y * PPU));
		Mesh->manualDeletion = true;
		PhysicsBody->AddMesh(Mesh);
	}
}
