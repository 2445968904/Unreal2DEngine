#include "QuarkSoftBodyComponent.h"
#include "QuarkWorldComponent.h"

UQuarkSoftBodyComponent::UQuarkSoftBodyComponent()
{
}

void UQuarkSoftBodyComponent::BeginPlay()
{
	Super::BeginPlay();

	QSoftBody* SoftBody = new QSoftBody();
	SoftBody->manualDeletion = true;
	SoftBody->SetRigidity(Rigidity);
	SoftBody->SetAreaPreservingEnabled(bAreaPreserving);
	SoftBody->SetShapeMatchingEnabled(bShapeMatching);
	SoftBody->SetShapeMatchingRate(ShapeMatchingRate);
	PhysicsBody = SoftBody;

	if (OwnerWorld)
	{
		OwnerWorld->AddBody(this);
	}
}

void UQuarkSoftBodyComponent::SetRigidity(float InRigidity)
{
	Rigidity = InRigidity;
	if (PhysicsBody)
	{
		static_cast<QSoftBody*>(PhysicsBody)->SetRigidity(InRigidity);
	}
}

float UQuarkSoftBodyComponent::GetRigidity() const
{
	return PhysicsBody ? static_cast<QSoftBody*>(PhysicsBody)->GetRigidity() : 0.0f;
}

void UQuarkSoftBodyComponent::SetAreaPreservingEnabled(bool bEnabled)
{
	bAreaPreserving = bEnabled;
	if (PhysicsBody)
	{
		static_cast<QSoftBody*>(PhysicsBody)->SetAreaPreservingEnabled(bEnabled);
	}
}

bool UQuarkSoftBodyComponent::GetAreaPreservingEnabled() const
{
	return PhysicsBody ? static_cast<QSoftBody*>(PhysicsBody)->GetAreaPreservingEnabled() : false;
}

void UQuarkSoftBodyComponent::SetShapeMatchingEnabled(bool bEnabled)
{
	bShapeMatching = bEnabled;
	if (PhysicsBody)
	{
		static_cast<QSoftBody*>(PhysicsBody)->SetShapeMatchingEnabled(bEnabled);
	}
}

bool UQuarkSoftBodyComponent::GetShapeMatchingEnabled() const
{
	return PhysicsBody ? static_cast<QSoftBody*>(PhysicsBody)->GetShapeMatchingEnabled() : false;
}

void UQuarkSoftBodyComponent::SetShapeMatchingRate(float Rate)
{
	ShapeMatchingRate = Rate;
	if (PhysicsBody)
	{
		static_cast<QSoftBody*>(PhysicsBody)->SetShapeMatchingRate(Rate);
	}
}

void UQuarkSoftBodyComponent::SetSelfCollisionsEnabled(bool bEnabled)
{
	if (PhysicsBody)
	{
		static_cast<QSoftBody*>(PhysicsBody)->SetSelfCollisionsEnabled(bEnabled);
	}
}

void UQuarkSoftBodyComponent::AddSoftRectMesh(FVector2D Size, FVector2D CenterOffset, FVector2D Grid)
{
	if (PhysicsBody)
	{
		float PPU = OwnerWorld ? OwnerWorld->PixelsPerUnit : 1.0f;
		QMesh* Mesh = QMesh::CreateWithRect(
			QVector(Size.X * PPU, Size.Y * PPU),
			QVector(CenterOffset.X * PPU, CenterOffset.Y * PPU),
			QVector(Grid.X, Grid.Y),
			true, true, 0.5f
		);
		Mesh->manualDeletion = true;
		PhysicsBody->AddMesh(Mesh);
	}
}

void UQuarkSoftBodyComponent::AddSoftPolygonMesh(float Radius, int32 SideCount, FVector2D CenterOffset, int32 PolarGrid)
{
	if (PhysicsBody)
	{
		float PPU = OwnerWorld ? OwnerWorld->PixelsPerUnit : 1.0f;
		QMesh* Mesh = QMesh::CreateWithPolygon(
			Radius * PPU, SideCount,
			QVector(CenterOffset.X * PPU, CenterOffset.Y * PPU),
			PolarGrid, true, true, 0.5f
		);
		Mesh->manualDeletion = true;
		PhysicsBody->AddMesh(Mesh);
	}
}
