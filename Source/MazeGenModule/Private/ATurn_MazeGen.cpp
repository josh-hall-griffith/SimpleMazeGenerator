// Author: Joshua Hall - Griffith University
// Class: AATurn_MazeGen
// Purpose: This class generates and visualises a procedural maze using a turn algorithm. 
// It creates maze floor and wall meshes using instanced static meshes for optimized performance. 
// The maze generation ensures random, complete, but isn't solvable 
// License: MIT


#include "ATurn_MazeGen.h"
#include "Components/InstancedStaticMeshComponent.h"

// Sets default values
AATurn_MazeGen::AATurn_MazeGen()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	// Create and set Root Component if not already set
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	// Create floor instanced mesh component
	m_floorStaticMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("FloorMeshComponent"));
	m_floorStaticMeshComponent->SetupAttachment(RootComponent);
	m_floorStaticMeshComponent->SetMobility(EComponentMobility::Static);  // Optimize for performance

	// Create default wall instanced mesh component
	m_defaultWallStaticMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("DefaultWallMeshComponent"));
	m_defaultWallStaticMeshComponent->SetupAttachment(RootComponent);
	m_defaultWallStaticMeshComponent->SetMobility(EComponentMobility::Static);

	// Create rotated wall instanced mesh component
	m_rotatedWallStaticMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("RotatedWallMeshComponent"));
	m_rotatedWallStaticMeshComponent->SetupAttachment(RootComponent);
	m_rotatedWallStaticMeshComponent->SetMobility(EComponentMobility::Static);
}

void AATurn_MazeGen::GenerateMazeMeshes()
{
	// Ensure components are valid before proceeding
	if (!m_floorStaticMeshComponent || !m_defaultWallStaticMeshComponent || !m_rotatedWallStaticMeshComponent)
	{
		GEngine->AddOnScreenDebugMessage(-1, 10, FColor::Red, TEXT("Failed: One or more static mesh components are null!"));
		return;
	}

	// Assign Static Meshes
	if (floorStaticMesh)
	{
		m_floorStaticMeshComponent->SetStaticMesh(floorStaticMesh);
	}

	if (wallStaticMesh)
	{
		m_defaultWallStaticMeshComponent->SetStaticMesh(wallStaticMesh);
		m_rotatedWallStaticMeshComponent->SetStaticMesh(wallStaticMesh);
	}

	// Create and Assign Materials
	m_floorInstancedMaterial = UMaterialInstanceDynamic::Create(floorMaterial, this);
	m_defaultWallInstancedMaterial = UMaterialInstanceDynamic::Create(defaultWallMaterial, this);
	m_rotatedWallInstancedMaterial = UMaterialInstanceDynamic::Create(rotatedWallMaterial, this);

	// Apply material to mesh component
	if (m_floorInstancedMaterial)
	{
		m_floorStaticMeshComponent->SetMaterial(0, m_floorInstancedMaterial);
	}

	if (m_defaultWallInstancedMaterial)
	{
		m_defaultWallStaticMeshComponent->SetMaterial(0, m_defaultWallInstancedMaterial);
	}

	if (m_rotatedWallInstancedMaterial)
	{
		m_rotatedWallStaticMeshComponent->SetMaterial(0, m_rotatedWallInstancedMaterial);
	}

	// Loop through the grid and generate floor and walls
	for (int x = 0; x < levelWidth; x++)
	{
		for (int y = 0; y < levelHeight; y++)
		{
			// Calculate Position
			float xPos = x * positionScaling;
			float yPos = y * positionScaling;
			float zPos = 0; // Ground level

			FVector spawnLocation(xPos, yPos, zPos);

			// Determine wall rotation (randomized)
			int randValue = FMath::RandRange(0, 1);
			FRotator wallRotation;

			if (randValue == 1)
			{
				wallRotation = FRotator(0.0f, wallRotationDeg, 0.0f);
			}
			else
			{
				wallRotation = FRotator(0.0f, 0.0f, 0.0f);
			}

			// Define Scaling
			FVector wallScale(1.0f * meshScaling.X, 0.1f * meshScaling.Y, 1.0f * meshScaling.Z);
			FVector floorScale(1.0f * meshScaling.X, 1.0f * meshScaling.Y, 0.1f * meshScaling.Z);

			// Create Transformations
			FTransform wallTransform(wallRotation, spawnLocation, wallScale);
			FTransform floorTransform(FRotator::ZeroRotator, spawnLocation, floorScale);


			// Spawn Walls
			if (randValue == 1)
			{
				m_rotatedWallStaticMeshComponent->AddInstance(wallTransform, true);
			}
			else
			{
				m_defaultWallStaticMeshComponent->AddInstance(wallTransform, true);
			}

			// Spawn Floor
			m_floorStaticMeshComponent->AddInstance(floorTransform, true);
		}
	}
}



// Called when the game starts or when spawned
void AATurn_MazeGen::BeginPlay()
{
	Super::BeginPlay();
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Green, "Generate Turn Maze");
	}
	GenerateMazeMeshes();
}

// Called every frame
void AATurn_MazeGen::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

