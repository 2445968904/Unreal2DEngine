#include "QuarkWorldComponent.h"
#include "QuarkBodyComponent.h"
#include "QuarkJointComponent.h"
#include "QuarkSpringComponent.h"

UQuarkWorldComponent::UQuarkWorldComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	bAutoActivate = true;
}

void UQuarkWorldComponent::BeginPlay()
{
	Super::BeginPlay();
	PhysicsWorld = new QWorld();
}

void UQuarkWorldComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (PhysicsWorld && PhysicsWorld->GetEnabled())
	{
		PhysicsWorld->SetTimeScale(TimeScale);
		PhysicsWorld->Update();
	}
}

void UQuarkWorldComponent::SetGravity(FVector2D InGravity)
{
	if (PhysicsWorld)
	{
		PhysicsWorld->SetGravity(QVector(InGravity.X, InGravity.Y));
	}
}

FVector2D UQuarkWorldComponent::GetGravity() const
{
	if (PhysicsWorld)
	{
		QVector G = PhysicsWorld->GetGravity();
		return FVector2D(G.x, G.y);
	}
	return FVector2D(0.0f, 9.8f);
}

void UQuarkWorldComponent::SetIterationCount(int32 Count)
{
	if (PhysicsWorld)
	{
		PhysicsWorld->SetIterationCount(Count);
	}
}

int32 UQuarkWorldComponent::GetIterationCount() const
{
	if (PhysicsWorld)
	{
		return PhysicsWorld->GetIterationCount();
	}
	return 4;
}

void UQuarkWorldComponent::SetTimeScale(float Scale)
{
	TimeScale = Scale;
}

float UQuarkWorldComponent::GetTimeScale() const
{
	return TimeScale;
}

void UQuarkWorldComponent::SetSleepingEnabled(bool bEnabled)
{
	if (PhysicsWorld)
	{
		PhysicsWorld->SetSleepingEnabled(bEnabled);
	}
}

void UQuarkWorldComponent::SetBroadphaseEnabled(bool bEnabled)
{
	if (PhysicsWorld)
	{
		PhysicsWorld->SetBroadphaseEnabled(bEnabled);
	}
}

void UQuarkWorldComponent::AddBody(UQuarkBodyComponent* Body)
{
	if (PhysicsWorld && Body && Body->GetPhysicsBody())
	{
		PhysicsWorld->AddBody(Body->GetPhysicsBody());
	}
}

void UQuarkWorldComponent::RemoveBody(UQuarkBodyComponent* Body)
{
	if (PhysicsWorld && Body && Body->GetPhysicsBody())
	{
		PhysicsWorld->RemoveBody(Body->GetPhysicsBody());
	}
}

void UQuarkWorldComponent::AddJoint(UQuarkJointComponent* Joint)
{
	if (PhysicsWorld && Joint && Joint->GetPhysicsJoint())
	{
		PhysicsWorld->AddJoint(Joint->GetPhysicsJoint());
	}
}

void UQuarkWorldComponent::RemoveJoint(UQuarkJointComponent* Joint)
{
	if (PhysicsWorld && Joint && Joint->GetPhysicsJoint())
	{
		PhysicsWorld->RemoveJoint(Joint->GetPhysicsJoint());
	}
}

void UQuarkWorldComponent::AddSpring(UQuarkSpringComponent* Spring)
{
	if (PhysicsWorld && Spring && Spring->GetPhysicsSpring())
	{
		PhysicsWorld->AddSpring(Spring->GetPhysicsSpring());
	}
}

void UQuarkWorldComponent::RemoveSpring(UQuarkSpringComponent* Spring)
{
	if (PhysicsWorld && Spring && Spring->GetPhysicsSpring())
	{
		PhysicsWorld->RemoveSpring(Spring->GetPhysicsSpring());
	}
}

bool UQuarkWorldComponent::Raycast(FVector2D Start, FVector2D End, TArray<FVector2D>& OutHitPositions, TArray<UQuarkBodyComponent*>& OutHitBodies)
{
	if (!PhysicsWorld) return false;

	float PPU = PixelsPerUnit;
	QVector RayStart(Start.X * PPU, Start.Y * PPU);
	QVector RayVec((End.X - Start.X) * PPU, (End.Y - Start.Y) * PPU);

	std::vector<QRaycast::Contact> Contacts = QRaycast::RaycastTo(PhysicsWorld, RayStart, RayVec);

	bool bHit = !Contacts.empty();
	if (bHit)
	{
		for (auto& Hit : Contacts)
		{
			OutHitPositions.Add(FVector2D(Hit.position.x / PPU, Hit.position.y / PPU));
			if (Hit.body)
			{
				for (TObjectIterator<UQuarkBodyComponent> It; It; ++It)
				{
					if (It->GetPhysicsBody() == Hit.body && It->GetWorld() == GetWorld())
					{
						OutHitBodies.Add(*It);
						break;
					}
				}
			}
		}
	}
	return bHit;
}

void UQuarkWorldComponent::DestroyComponent(bool bPromoteChildren)
{
	if (PhysicsWorld)
	{
		PhysicsWorld->ClearWorld();
		delete PhysicsWorld;
		PhysicsWorld = nullptr;
	}
	Super::DestroyComponent(bPromoteChildren);
}
