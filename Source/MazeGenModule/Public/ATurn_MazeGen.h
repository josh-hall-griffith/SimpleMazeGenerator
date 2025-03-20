// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ATurn_MazeGen.generated.h"

UCLASS()
class MAZEGENMODULE_API AATurn_MazeGen : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AATurn_MazeGen();
	void GenerateMazeMeshes();

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

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
