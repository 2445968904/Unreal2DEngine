#include "QuarkPhysicsRendererComponent.h"
#include "QuarkBodyComponent.h"
#include "QuarkWorldComponent.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "UObject/ConstructorHelpers.h"
#include "GameFramework/Actor.h"

UQuarkPhysicsRendererComponent::UQuarkPhysicsRendererComponent(const FObjectInitializer& ObjectInitializer)
	: UProceduralMeshComponent(ObjectInitializer)
{
	PrimaryComponentTick.bCanEverTick = true;
	bTickInEditor = true;
	bAutoActivate = true;

	// Load default material in constructor (ConstructorHelpers only works here)
	static ConstructorHelpers::FObjectFinderOptional<UMaterialInterface> DefaultMaterialRef(TEXT("/Engine/EngineMaterials/UnlitMaterial"));
	if (DefaultMaterialRef.Succeeded())
	{
		DefaultMaterial = DefaultMaterialRef.Get();
	}
}

void UQuarkPhysicsRendererComponent::BeginPlay()
{
	Super::BeginPlay();

	UMaterialInterface* MatToUse = OverrideMaterial ? OverrideMaterial : DefaultMaterial;
	if (MatToUse)
	{
		DynamicMaterial = UMaterialInstanceDynamic::Create(MatToUse, this);
	}

	// Auto-find target body if not set
	if (!TargetBody)
	{
		AActor* Owner = GetOwner();
		if (Owner)
		{
			TargetBody = Owner->FindComponentByClass<UQuarkBodyComponent>();
		}
	}
}

void UQuarkPhysicsRendererComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	UpdateMesh();
}

void UQuarkPhysicsRendererComponent::SetTargetBody(UQuarkBodyComponent* InTargetBody)
{
	TargetBody = InTargetBody;
}

void UQuarkPhysicsRendererComponent::UpdateMesh()
{
	if (!TargetBody || !TargetBody->GetPhysicsBody())
	{
		ClearAllMeshSections();
		return;
	}

	QBody* Body = TargetBody->GetPhysicsBody();

	float PPU = 100.0f;
	AActor* Owner = GetOwner();
	if (Owner)
	{
		UQuarkWorldComponent* WorldComp = Owner->FindComponentByClass<UQuarkWorldComponent>();
		if (WorldComp)
		{
			PPU = WorldComp->PixelsPerUnit;
		}
	}

	BuildMeshFromBody(Body, PPU);
}

void UQuarkPhysicsRendererComponent::BuildMeshFromBody(QBody* Body, float PPU)
{
	TArray<FVector> Vertices;
	TArray<int32> Indices;
	TArray<FVector> Normals;
	TArray<FVector2D> UVs;
	TArray<FColor> Colors;
	TArray<FProcMeshTangent> Tangents;

	ClearAllMeshSections();

	int32 VertexOffset = 0;
	float Depth = 0.0f;

	for (int32 MeshIdx = 0; MeshIdx < Body->GetMeshCount(); MeshIdx++)
	{
		QMesh* Mesh = Body->GetMeshAt(MeshIdx);

		if (Body->GetBodyType() == QBody::BodyTypes::RIGID)
		{
			if (Mesh->GetPolygonParticleCount() >= 3)
			{
				int32 SubPolyCount = Mesh->GetSubConvexPolygonCount();
				for (int32 sp = 0; sp < SubPolyCount; sp++)
				{
					std::vector<QParticle*>& SubPoly = Mesh->GetSubConvexPolygonAt(sp);
					if ((int32)SubPoly.size() < 3) continue;

					QParticle* P0 = SubPoly[0];
					Vertices.Add(FVector(P0->GetGlobalPosition().x / PPU, Depth, P0->GetGlobalPosition().y / PPU));
					Normals.Add(FVector(0, -1, 0));
					UVs.Add(FVector2D(0.5f, 0.5f));
					Colors.Add(FColor::White);

					for (int32 i = 1; i < (int32)SubPoly.size() - 1; i++)
					{
						QParticle* Pi = SubPoly[i];
						QParticle* Pi1 = SubPoly[i + 1];

						Vertices.Add(FVector(Pi->GetGlobalPosition().x / PPU, Depth, Pi->GetGlobalPosition().y / PPU));
						Normals.Add(FVector(0, -1, 0));
						UVs.Add(FVector2D(0.5f, 0.5f));
						Colors.Add(FColor::White);

						Vertices.Add(FVector(Pi1->GetGlobalPosition().x / PPU, Depth, Pi1->GetGlobalPosition().y / PPU));
						Normals.Add(FVector(0, -1, 0));
						UVs.Add(FVector2D(0.5f, 0.5f));
						Colors.Add(FColor::White);

						Indices.Add(VertexOffset);
						Indices.Add(VertexOffset + (i - 1) * 2 + 1);
						Indices.Add(VertexOffset + (i - 1) * 2 + 2);
					}
					VertexOffset = Vertices.Num();
				}
			}

			for (int32 p = 0; p < Mesh->GetParticleCount(); p++)
			{
				QParticle* Particle = Mesh->GetParticleAt(p);
				if (Particle->GetRadius() > 0.5f)
				{
					float R = Particle->GetRadius() / PPU;
					FVector Center(Particle->GetGlobalPosition().x / PPU, Depth, Particle->GetGlobalPosition().y / PPU);

					int32 Segments = 16;
					for (int32 s = 0; s < Segments; s++)
					{
						float Angle1 = 2.0f * PI * s / Segments;
						float Angle2 = 2.0f * PI * (s + 1) / Segments;

						FVector V1 = Center + FVector(R * FMath::Cos(Angle1), 0, R * FMath::Sin(Angle1));
						FVector V2 = Center + FVector(R * FMath::Cos(Angle2), 0, R * FMath::Sin(Angle2));

						Vertices.Add(Center);
						Normals.Add(FVector(0, -1, 0));
						UVs.Add(FVector2D(0.5f, 0.5f));
						Colors.Add(FColor::White);

						Vertices.Add(V1);
						Normals.Add(FVector(0, -1, 0));
						UVs.Add(FVector2D(0.5f + 0.5f * FMath::Cos(Angle1), 0.5f + 0.5f * FMath::Sin(Angle1)));
						Colors.Add(FColor::White);

						Vertices.Add(V2);
						Normals.Add(FVector(0, -1, 0));
						UVs.Add(FVector2D(0.5f + 0.5f * FMath::Cos(Angle2), 0.5f + 0.5f * FMath::Sin(Angle2)));
						Colors.Add(FColor::White);

						Indices.Add(VertexOffset + s * 3);
						Indices.Add(VertexOffset + s * 3 + 1);
						Indices.Add(VertexOffset + s * 3 + 2);
					}
					VertexOffset = Vertices.Num();
				}
			}
		}
		else if (Body->GetBodyType() == QBody::BodyTypes::SOFT)
		{
			for (int32 p = 0; p < Mesh->GetParticleCount(); p++)
			{
				QParticle* Particle = Mesh->GetParticleAt(p);
				float R = Particle->GetRadius() / PPU;
				if (R < 0.001f) R = 0.5f / PPU;
				FVector Center(Particle->GetGlobalPosition().x / PPU, Depth, Particle->GetGlobalPosition().y / PPU);

				int32 Segments = 8;
				for (int32 s = 0; s < Segments; s++)
				{
					float Angle1 = 2.0f * PI * s / Segments;
					float Angle2 = 2.0f * PI * (s + 1) / Segments;

					FVector V1 = Center + FVector(R * FMath::Cos(Angle1), 0, R * FMath::Sin(Angle1));
					FVector V2 = Center + FVector(R * FMath::Cos(Angle2), 0, R * FMath::Sin(Angle2));

					Vertices.Add(Center);
					Normals.Add(FVector(0, -1, 0));
					UVs.Add(FVector2D(0.5f, 0.5f));
					Colors.Add(FColor::White);

					Vertices.Add(V1);
					Normals.Add(FVector(0, -1, 0));
					UVs.Add(FVector2D(0.5f + 0.5f * FMath::Cos(Angle1), 0.5f + 0.5f * FMath::Sin(Angle1)));
					Colors.Add(FColor::White);

					Vertices.Add(V2);
					Normals.Add(FVector(0, -1, 0));
					UVs.Add(FVector2D(0.5f + 0.5f * FMath::Cos(Angle2), 0.5f + 0.5f * FMath::Sin(Angle2)));
					Colors.Add(FColor::White);

					Indices.Add(VertexOffset + s * 3);
					Indices.Add(VertexOffset + s * 3 + 1);
					Indices.Add(VertexOffset + s * 3 + 2);
				}
				Depth += DepthOffset;
				VertexOffset = Vertices.Num();
			}

			if (Mesh->GetPolygonParticleCount() >= 3)
			{
				int32 UVMapCount = Mesh->GetUVMapCount();
				if (UVMapCount > 0)
				{
					for (int32 uv = 0; uv < UVMapCount; uv++)
					{
						std::vector<int> UVMap = Mesh->GetUVMapAt(uv);
						if ((int32)UVMap.size() < 3) continue;

						for (int32 i = 0; i < (int32)UVMap.size(); i++)
						{
							QParticle* P = Mesh->GetParticleAt(UVMap[i]);
							FVector Pos(P->GetGlobalPosition().x / PPU, Depth, P->GetGlobalPosition().y / PPU);
							Vertices.Add(Pos);
							Normals.Add(FVector(0, -1, 0));
							UVs.Add(FVector2D(P->GetPosition().x, P->GetPosition().y));
							Colors.Add(FColor::White);
						}

						for (int32 i = 1; i < (int32)UVMap.size() - 1; i++)
						{
							Indices.Add(VertexOffset);
							Indices.Add(VertexOffset + i);
							Indices.Add(VertexOffset + i + 1);
						}

						Depth += DepthOffset;
						VertexOffset = Vertices.Num();
					}
				}
			}
		}
	}

	if (Vertices.Num() > 0)
	{
		CreateMeshSection(0, Vertices, Indices, Normals, UVs, Colors, Tangents, false);

		if (DynamicMaterial)
		{
			SetMaterial(0, DynamicMaterial);
		}
	}
}
