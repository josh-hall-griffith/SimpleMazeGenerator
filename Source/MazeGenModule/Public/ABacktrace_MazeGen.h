// Author: Joshua Hall - Griffith University
// Class: AATurn_MazeGen
// Purpose: This class generates and visualises a procedural maze using a turn algorithm. 
// It creates maze floor and wall meshes using instanced static meshes for optimized performance. 
// The maze generation ensures random, complete, but isn't solvable 
// License: MIT
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ABacktrace_MazeGen.generated.h"

/*NEW*/
// Define the MazeCell struct first
USTRUCT()
struct FMazeCell {	
	GENERATED_BODY()
public:
	bool visited = false;
	bool northWall = true;
	bool southWall = true;
	bool eastWall = true;
	bool westWall = true;
};

UCLASS()
class MAZEGENMODULE_API AABacktrace_MazeGen : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AABacktrace_MazeGen();
	void GenerateMazeMeshes();

	/*NEW*/
	void VisualiseMaze();
	void GenerateMaze(int x, int y);

	// Width of the maze in grid cells 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maze Settings")
	int levelWidth = 128;

	// Height of the maze in grid cells 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maze Settings")
	int levelHeight = 128;

	// Distance between cells (affects positioning) 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maze Settings")
	int positionScaling = 200;

	// Wall rotation in degrees. 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maze Settings")
	float wallRotationDeg = 90.0f;

	// Scaling factor for wall and floor meshes 
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Maze Settings")
	FVector meshScaling = FVector{ 1.0f, 1.0f, 1.0f };

	/*NEW*/
	// Small offset value to add to remove z fighting
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Mesh ZOffset")
	float zOffset = 0.1f;

	// Static mesh for the floor 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Settings")
	UStaticMesh* floorStaticMesh;

	// Static mesh for the walls 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Mesh Settings")
	UStaticMesh* wallStaticMesh;

	// Default material for walls 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material Settings")
	UMaterial* defaultWallMaterial;

	// Material for rotated walls 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material Settings")
	UMaterial* rotatedWallMaterial;

	// Material for the floor 
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Material Settings")
	UMaterial* floorMaterial;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Instanced Static Mesh for default walls (not exposed to editor)
	UPROPERTY()
	UInstancedStaticMeshComponent* m_defaultWallStaticMeshComponent;

	// Instanced Static Mesh for rotated walls (not exposed to editor)
	UPROPERTY()
	UInstancedStaticMeshComponent* m_rotatedWallStaticMeshComponent;

	// Instanced Static Mesh for the floor (not exposed to editor)
	UPROPERTY()
	UInstancedStaticMeshComponent* m_floorStaticMeshComponent;

	// Dynamic material instance for rotated walls 
	UPROPERTY()
	UMaterialInstanceDynamic* m_rotatedWallInstancedMaterial;

	// Dynamic material instance for default walls 
	UPROPERTY()
	UMaterialInstanceDynamic* m_defaultWallInstancedMaterial;

	// Dynamic material instance for the floor 
	UPROPERTY()
	UMaterialInstanceDynamic* m_floorInstancedMaterial;

	/*NEW*/
	// Double Dynamic Array to hold our struct
	TArray<TArray<FMazeCell>> grid;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
