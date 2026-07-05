#include "QuarkPhysicsDemo.h"
#include "QuarkWorldComponent.h"
#include "QuarkRigidBodyComponent.h"
#include "QuarkSoftBodyComponent.h"
#include "QuarkPhysicsRendererComponent.h"
#include "QuarkPhysics/qworld.h"

AQuarkPhysicsDemo::AQuarkPhysicsDemo()
{
	PrimaryActorTick.bCanEverTick = true;

	// Create physics world
	PhysicsWorld = CreateDefaultSubobject<UQuarkWorldComponent>(TEXT("PhysicsWorld"));
	PhysicsWorld->PixelsPerUnit = PixelsPerUnit;
	PhysicsWorld->TimeScale = 1.0f;

	// --- Ground ---
	GroundBody = CreateDefaultSubobject<UQuarkRigidBodyComponent>(TEXT("GroundBody"));
	GroundRenderer = CreateDefaultSubobject<UQuarkPhysicsRendererComponent>(TEXT("GroundRenderer"));

	// --- Falling Box 1 ---
	FallingBox1 = CreateDefaultSubobject<UQuarkRigidBodyComponent>(TEXT("FallingBox1"));
	Box1Renderer = CreateDefaultSubobject<UQuarkPhysicsRendererComponent>(TEXT("Box1Renderer"));

	// --- Falling Box 2 ---
	FallingBox2 = CreateDefaultSubobject<UQuarkRigidBodyComponent>(TEXT("FallingBox2"));
	Box2Renderer = CreateDefaultSubobject<UQuarkPhysicsRendererComponent>(TEXT("Box2Renderer"));

	// --- Falling Circle ---
	FallingCircle = CreateDefaultSubobject<UQuarkRigidBodyComponent>(TEXT("FallingCircle"));
	CircleRenderer = CreateDefaultSubobject<UQuarkPhysicsRendererComponent>(TEXT("CircleRenderer"));

	// --- Soft Body ---
	SoftBody = CreateDefaultSubobject<UQuarkSoftBodyComponent>(TEXT("SoftBody"));
	SoftBodyRenderer = CreateDefaultSubobject<UQuarkPhysicsRendererComponent>(TEXT("SoftBodyRenderer"));
}

void AQuarkPhysicsDemo::BeginPlay()
{
	Super::BeginPlay();

	// Configure world
	PhysicsWorld->SetGravity(FVector2D(0.0f, 9.8f));
	PhysicsWorld->SetIterationCount(8);

	// --- Ground: static rigid body, wide rectangle ---
	GroundBody->SetModeStatic();
	GroundBody->AddRectMesh(FVector2D(12.0f, 0.5f));
	GroundBody->SetPosition(FVector2D(0.0f, -3.0f));
	GroundBody->SetFriction(0.5f);
	GroundBody->SetRestitution(0.1f);

	// --- Falling Box 1: dynamic, drops from upper-left ---
	FallingBox1->AddRectMesh(FVector2D(1.0f, 1.0f));
	FallingBox1->SetPosition(FVector2D(-2.0f, 4.0f));
	FallingBox1->SetMass(2.0f);
	FallingBox1->SetFriction(0.3f);
	FallingBox1->SetRestitution(0.3f);

	// --- Falling Box 2: dynamic, drops from upper-right, rotated ---
	FallingBox2->AddRectMesh(FVector2D(0.8f, 1.5f));
	FallingBox2->SetPosition(FVector2D(2.0f, 6.0f));
	FallingBox2->SetRotation(30.0f);
	FallingBox2->SetMass(1.5f);
	FallingBox2->SetFriction(0.3f);
	FallingBox2->SetRestitution(0.2f);

	// --- Falling Circle: dynamic ---
	FallingCircle->AddCircleMesh(0.6f);
	FallingCircle->SetPosition(FVector2D(0.0f, 8.0f));
	FallingCircle->SetMass(1.0f);
	FallingCircle->SetFriction(0.4f);
	FallingCircle->SetRestitution(0.6f);

	// --- Soft Body: soft rect that deforms on impact ---
	if (bSpawnSoftBody)
	{
		SoftBody->AddSoftRectMesh(FVector2D(2.0f, 2.0f), FVector2D(0, 0), FVector2D(4, 4));
		SoftBody->SetPosition(FVector2D(0.0f, 10.0f));
		SoftBody->SetRigidity(0.8f);
		SoftBody->SetShapeMatchingEnabled(true);
		SoftBody->SetShapeMatchingRate(0.4f);
		SoftBody->SetAreaPreservingEnabled(true);
		SoftBody->SetMass(3.0f);
		SoftBody->SetFriction(0.3f);
	}

	// Give box1 a sideways impulse after spawn
	FTimerHandle ImpulseTimer;
	GetWorldTimerManager().SetTimer(ImpulseTimer, [this]()
	{
		if (FallingBox1)
		{
			FallingBox1->ApplyImpulse(FVector2D(3.0f, 0.0f));
		}
	}, 0.5f, false);
}

void AQuarkPhysicsDemo::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
