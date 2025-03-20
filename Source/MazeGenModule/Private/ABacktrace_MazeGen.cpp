// Author: Joshua Hall - Griffith University
// Class: AABacktrace_MazeGen
// Purpose: This class generates and visualises a procedural maze using a backtracking algorithm. 
// It creates maze floor and wall meshes using instanced static meshes for optimized performance. 
// The maze generation ensures random, complete, and solvable mazes with specified start and end points.
// License: MIT



#include "ABacktrace_MazeGen.h"
#include "Components/InstancedStaticMeshComponent.h"

/*===================
AABacktrace_MazeGen 

Constructor
Initializes the actor with static mesh components for the floor and walls.
Enables Tick() to run every frame.
===================*/
AABacktrace_MazeGen::AABacktrace_MazeGen()
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

/*===================
GenerateMazeMeshes

Sets up the maze meshes and materials for the maze generation process.
It assigns static meshes to floor and wall components and creates dynamic materials for instancing.
The maze grid is initialized with random rotations, and the maze is generated with a start and end point.
The start and end points are assigned openings in the walls, and the maze is visualized using the `VisualiseMaze` function.
Validations are performed to ensure components are correctly initialized before proceeding.
===================*/
void AABacktrace_MazeGen::GenerateMazeMeshes()
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


	/*NEW*/
	// Step 1: Init maze with random rotations

	grid.SetNum(levelWidth);
	for (int x = 0; x < levelWidth; x++) {
		grid[x].SetNum(levelHeight);
	}

	// Step 2: Generate the maze
	int startX = 0;
	int startY = 0;

	// Randomly choose the end point on the right or top edge
	int endX, endY;
	bool isRightEdge = FMath::RandBool(); // 50/50 chance for right or top edge
	if (isRightEdge) {
		endX = levelWidth - 1; // Right edge
		endY = FMath::RandRange(0, levelHeight - 1);
	}
	else {
		endX = FMath::RandRange(0, levelWidth - 1); // Top edge
		endY = levelHeight - 1;
	}
	GenerateMaze(startX, startY);

	// Step 4: Create openings at start and end
	grid[startX][startY].westWall = false; // Entrance opening at (0,0) on the left side

	// Remove the appropriate wall at the end point
	if (isRightEdge) {
		grid[endX][endY].eastWall = false; // Exit opening on the right side
	}
	else {
		grid[endX][endY].northWall = false; // Exit opening on the top side
	}

	// Step 3: Visualize it
	VisualiseMaze();

}


/*===================
VisualiseMaze

Visualizes the generated maze by instantiating floor and wall meshes at the correct positions.
Walls are placed based on the state of each grid cell, with north, south, east, and west walls added accordingly.
Materials are applied to the mesh components for proper rendering.
Instances are added in bulk for performance optimization.
===================*/
void AABacktrace_MazeGen::VisualiseMaze()
{
	// Assume these are set up in the actor's constructor
	UInstancedStaticMeshComponent* floorComponent = m_floorStaticMeshComponent;
	UInstancedStaticMeshComponent* hWallComponent = m_defaultWallStaticMeshComponent;
	UInstancedStaticMeshComponent* vWallComponent = m_rotatedWallStaticMeshComponent;

	// Assign Material
	m_floorStaticMeshComponent->SetMaterial(0, m_floorInstancedMaterial);
	m_defaultWallStaticMeshComponent->SetMaterial(0, m_defaultWallInstancedMaterial);
	m_rotatedWallStaticMeshComponent->SetMaterial(0, m_rotatedWallInstancedMaterial);

	// Create arrays to hold the instances for floor and walls
	TArray<FTransform> floorInstances;
	TArray<FTransform> hWallInstances;
	TArray<FTransform> vWallInstances;

	/*NEW*/
	for (int x = 0; x < levelWidth; x++) {
		for (int y = 0; y < levelHeight; y++) {
			FVector location = FVector(x * positionScaling, y * positionScaling, 0);

			// Floor (unchanged)
			FVector floorScale = FVector(1.0f * meshScaling.X, 1.0f * meshScaling.Y, 0.1f * meshScaling.Z);
			FTransform floorTransform(FRotator::ZeroRotator, location, floorScale);
			floorInstances.Add(floorTransform);

			// Walls adjusted to cell edges
			if (grid[x][y].northWall) {
				FVector wallLocation = FVector(x * positionScaling + zOffset, (y + 1) * positionScaling, 0);
				FVector wallScale = FVector(1.0f * meshScaling.X, 0.1f * meshScaling.Y, 1.0f * meshScaling.Z);
				FTransform wallTransform(FRotator::ZeroRotator, wallLocation, wallScale);
				hWallInstances.Add(wallTransform);
			}
			if (grid[x][y].southWall) {
				FVector wallLocation = FVector(x * positionScaling + zOffset, y * positionScaling, 0);
				FVector wallScale = FVector(1.0f * meshScaling.X, 0.1f * meshScaling.Y, 1.0f * meshScaling.Z);
				FTransform wallTransform(FRotator::ZeroRotator, wallLocation, wallScale);
				hWallInstances.Add(wallTransform);
			}
			if (grid[x][y].eastWall) {
				FVector wallLocation = FVector((x + 1) * positionScaling, y * positionScaling + zOffset, 0);
				FVector wallScale = FVector(0.1f * meshScaling.X, 1.0f * meshScaling.Y, 1.0f * meshScaling.Z);
				FTransform wallTransform(FRotator::ZeroRotator, wallLocation, wallScale);
				vWallInstances.Add(wallTransform);
			}
			if (grid[x][y].westWall) {
				FVector wallLocation = FVector(x * positionScaling, y * positionScaling + zOffset, 0);
				FVector wallScale = FVector(0.1f * meshScaling.X, 1.0f * meshScaling.Y, 1.0f * meshScaling.Z);
				FTransform wallTransform(FRotator::ZeroRotator, wallLocation, wallScale);
				vWallInstances.Add(wallTransform);
			}
		}
	}

	// Now add all the instances at once
	floorComponent->AddInstances(floorInstances, true);
	hWallComponent->AddInstances(hWallInstances, true);
	vWallComponent->AddInstances(vWallInstances, true);
}

/*NEW*/
/*===================
GenerateMaze

Generates a maze using a recursive backtracking algorithm. It marks cells as visited,
identifies valid neighbors, shuffles them for randomness, and removes walls between
neighboring cells. The function recursively explores and backtracks until the maze is complete.
===================*/
void AABacktrace_MazeGen::GenerateMaze(int x, int y)
{
	// Step 1: Mark the current cell as visited
	// Step 2: Identify valid neighbors (left, right, up, down)
	// Step 3: Shuffle the neighbors to ensure random path generation
	// Step 4: Visit each neighbor
	// Step 5: Recursively visit the neighbor
	// Step 6: Backtrack when no unvisited neighbors remain


	// Step 1: Mark the current cell as visited
	grid[x][y].visited = true;

	// Step 2: Identify valid neighbors (left, right, up, down)
	TArray<FIntPoint> neighbors;
	if (x > 0 && !grid[x - 1][y].visited)
	{
		neighbors.Add(FIntPoint(x - 1, y)); // Left
	}

	if (x < levelWidth - 1 && !grid[x + 1][y].visited) {
		neighbors.Add(FIntPoint(x + 1, y)); // Right
	}

	if (y > 0 && !grid[x][y - 1].visited) {
		neighbors.Add(FIntPoint(x, y - 1)); // Down
	}

	if (y < levelHeight - 1 && !grid[x][y + 1].visited) {
		neighbors.Add(FIntPoint(x, y + 1)); // Up
	}

	// Step 3: Shuffle the neighbors to ensure random path generation
	for (int i = neighbors.Num() - 1; i > 0; i--) {
		int j = FMath::RandRange(0, i);
		neighbors.Swap(i, j);
	}

	// Step 4: Visit each neighbor
	for (int i = 0; i < neighbors.Num(); i++) {
		int nx = neighbors[i].X;
		int ny = neighbors[i].Y;

		if (!grid[nx][ny].visited) {
			// Remove the wall between the current cell and the neighbor
			if (nx < x) { // Neighbor is to the left
				grid[x][y].westWall = false;
				grid[nx][ny].eastWall = false;
			}
			else if (nx > x) { // Neighbor is to the right
				grid[x][y].eastWall = false;
				grid[nx][ny].westWall = false;
			}
			else if (ny < y) { // Neighbor is below
				grid[x][y].southWall = false;
				grid[nx][ny].northWall = false;
			}
			else if (ny > y) { // Neighbor is above
				grid[x][y].northWall = false;
				grid[nx][ny].southWall = false;
			}

			// Step 5: Recursively visit the neighbor
			GenerateMaze(nx, ny);
		}
	}

	// Step 6: Backtrack when no unvisited neighbors remain
	// (Handled implicitly by the recursion: the function returns when there are no unvisited neighbors)
}

// Called when the game starts or when spawned
void AABacktrace_MazeGen::BeginPlay()
{
	Super::BeginPlay();
	if (GEngine) {
		GEngine->AddOnScreenDebugMessage(-1, 5.0f, FColor::Orange, "Generate Backtrace Maze");
	}
	GenerateMazeMeshes();
}

// Called every frame
void AABacktrace_MazeGen::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

