#include "QuarkSpringComponent.h"
#include "QuarkBodyComponent.h"
#include "QuarkWorldComponent.h"
#include "GameFramework/Actor.h"
#include "QuarkPhysics/qsoftbody.h"

UQuarkSpringComponent::UQuarkSpringComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
	bAutoActivate = true;
}

void UQuarkSpringComponent::BeginPlay()
{
	Super::BeginPlay();

	AActor* Owner = GetOwner();
	if (Owner)
	{
		OwnerWorld = Owner->FindComponentByClass<UQuarkWorldComponent>();
	}

	if (!BodyA || !BodyB || !BodyA->GetPhysicsBody() || !BodyB->GetPhysicsBody())
	{
		return;
	}

	float PPU = GetPPU();

	// Find the closest particles between the two bodies
	QParticle* ParticleA = nullptr;
	QParticle* ParticleB = nullptr;

	QBody* PhysBodyA = BodyA->GetPhysicsBody();
	QBody* PhysBodyB = BodyB->GetPhysicsBody();

	QVector PosA = PhysBodyA->GetPosition();
	QVector PosB = PhysBodyB->GetPosition();

	float MinDist = std::numeric_limits<float>::max();

	// Search particles in body A
	for (QMesh* Mesh : PhysBodyA->_meshes)
	{
		for (QParticle* P : Mesh->particles)
		{
			float Dist = (P->GetGlobalPosition() - PosB).Length();
			if (Dist < MinDist)
			{
				MinDist = Dist;
				ParticleA = P;
			}
		}
	}

	MinDist = std::numeric_limits<float>::max();

	// Search particles in body B
	for (QMesh* Mesh : PhysBodyB->_meshes)
	{
		for (QParticle* P : Mesh->particles)
		{
			float Dist = (P->GetGlobalPosition() - PosA).Length();
			if (Dist < MinDist)
			{
				MinDist = Dist;
				ParticleB = P;
			}
		}
	}

	if (!ParticleA || !ParticleB)
	{
		return;
	}

	if (SpringLength > 0.0f)
	{
		PhysicsSpring = new QSpring(ParticleA, ParticleB, SpringLength * PPU);
	}
	else
	{
		PhysicsSpring = new QSpring(ParticleA, ParticleB);
	}

	PhysicsSpring->manualDeletion = true;
	PhysicsSpring->SetRigidity(Rigidity);
	PhysicsSpring->SetDistanceLimitEnabled(bDistanceLimitEnabled);
	PhysicsSpring->SetMinimumDistanceFactor(MinimumDistanceFactor);
	PhysicsSpring->SetMaximumDistanceFactor(MaximumDistanceFactor);
	PhysicsSpring->SetEnabled(bSpringEnabled);

	if (OwnerWorld && OwnerWorld->GetPhysicsWorld())
	{
		OwnerWorld->GetPhysicsWorld()->AddSpring(PhysicsSpring);
	}
}

void UQuarkSpringComponent::DestroyComponent(bool bPromoteChildren)
{
	if (PhysicsSpring)
	{
		if (OwnerWorld && OwnerWorld->GetPhysicsWorld())
		{
			OwnerWorld->GetPhysicsWorld()->RemoveSpring(PhysicsSpring);
		}
		if (!PhysicsSpring->manualDeletion)
		{
			delete PhysicsSpring;
		}
		PhysicsSpring = nullptr;
	}
	Super::DestroyComponent(bPromoteChildren);
}

void UQuarkSpringComponent::SetBodyA(UQuarkBodyComponent* InBodyA)
{
	BodyA = InBodyA;
}

void UQuarkSpringComponent::SetBodyB(UQuarkBodyComponent* InBodyB)
{
	BodyB = InBodyB;
}

void UQuarkSpringComponent::SetRigidity(float InRigidity)
{
	Rigidity = InRigidity;
	if (PhysicsSpring) PhysicsSpring->SetRigidity(InRigidity);
}

float UQuarkSpringComponent::GetRigidity() const
{
	return PhysicsSpring ? PhysicsSpring->GetRigidity() : Rigidity;
}

void UQuarkSpringComponent::SetLength(float InLength)
{
	SpringLength = InLength;
	if (PhysicsSpring)
	{
		float PPU = GetPPU();
		PhysicsSpring->SetLength(InLength * PPU);
	}
}

float UQuarkSpringComponent::GetLength() const
{
	if (PhysicsSpring)
	{
		float PPU = GetPPU();
		return PhysicsSpring->GetLength() / PPU;
	}
	return SpringLength;
}

void UQuarkSpringComponent::SetDistanceLimitEnabled(bool bEnabled)
{
	bDistanceLimitEnabled = bEnabled;
	if (PhysicsSpring) PhysicsSpring->SetDistanceLimitEnabled(bEnabled);
}

bool UQuarkSpringComponent::GetDistanceLimitEnabled() const
{
	return PhysicsSpring ? PhysicsSpring->GetDistanceLimitEnabled() : bDistanceLimitEnabled;
}

void UQuarkSpringComponent::SetMinimumDistanceFactor(float InFactor)
{
	MinimumDistanceFactor = InFactor;
	if (PhysicsSpring) PhysicsSpring->SetMinimumDistanceFactor(InFactor);
}

float UQuarkSpringComponent::GetMinimumDistanceFactor() const
{
	return PhysicsSpring ? PhysicsSpring->GetMinimumDistanceFactor() : MinimumDistanceFactor;
}

void UQuarkSpringComponent::SetMaximumDistanceFactor(float InFactor)
{
	MaximumDistanceFactor = InFactor;
	if (PhysicsSpring) PhysicsSpring->SetMaximumDistanceFactor(InFactor);
}

float UQuarkSpringComponent::GetMaximumDistanceFactor() const
{
	return PhysicsSpring ? PhysicsSpring->GetMaximumDistanceFactor() : MaximumDistanceFactor;
}

void UQuarkSpringComponent::SetEnabled(bool bEnabled)
{
	bSpringEnabled = bEnabled;
	if (PhysicsSpring) PhysicsSpring->SetEnabled(bEnabled);
}

bool UQuarkSpringComponent::GetEnabled() const
{
	return PhysicsSpring ? PhysicsSpring->GetEnabled() : bSpringEnabled;
}

float UQuarkSpringComponent::GetPPU() const
{
	return OwnerWorld ? OwnerWorld->PixelsPerUnit : 1.0f;
}
