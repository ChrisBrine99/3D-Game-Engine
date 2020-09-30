#ifndef PARTITION_H
#define PARTITION_H

#include "../Math/Ray.h"
#include "../Rendering/3D/GameObject.h"

enum OCT_CHILDREN {
	TLF,	// The top left front node
	BLF,	// The bottom left front node
	BRF,	// The bottom right front node
	TRF,	// The top right front node

	TLR,	// The top left rear node
	BLR,	// The bottom left rear node
	BRR,	// The bottom right rear node
	TRR,	// The top right rear node
};

#define NUM_CHILDREN 8
class OctNode {
private:
	friend class OctSpatialPartition;	// Allows the OctSpatialPartition class to access all the private memmbers of OctNode
	std::vector<GameObject*> objects;	// The list of game objects that currently reside in this node's area

	BoundingBox* octBounds;				// The bounding box for the current partition area
	OctNode* children[NUM_CHILDREN];	// Holds the current node's child nodes
	OctNode* parent;					// Holds a pointer value to the parent of this node object
	float size;							// The size of the current node (Is 1/2 size of the parent, 2x size of its children)
public:
	OctNode(glm::vec3 position_, float size_, OctNode* parent_);	// Takes in position in world, size of the node, and reference to its parent
	~OctNode();			// Standard destructor that calls OnDestroy
	void OnDestroy();	// Deletes any pointers and allocated memory

	void Octify(int depth_);	// Creates the children for this node

	OctNode* GetParent();						// Gets this node's parent node
	OctNode* GetChild(OCT_CHILDREN index_);		// Gets a specific child that this node has
	bool IsLeafNode() const;					// Checks if this node has no children

	void AddCollisionObject(GameObject* object_);	// Adds a game object to this node's vector of game objects
	int GetObjectCount() const;						// Returns the total number of objects in this current node's region

	BoundingBox* GetBoundingBox() const;		// Returns this node's bounding box
};

class OctSpatialPartition {
private:
	OctNode* rootNode;
	std::vector<OctNode*> rayIntersectList;
public:
	OctSpatialPartition(float worldSize_, float depth_);	// Takes in a size that the root node will inherit as well as the depth of the partitioning
	~OctSpatialPartition();		// Standard destructor that calls OnDestroy
	void OnDestroy();			// Deletes any pointers and allocated memory

	void AddObject(GameObject* object_);
	GameObject* GetCollidedObject(Ray ray_);
private:
	void AddObjectToNode(OctNode* node_, GameObject* object_);	// Adds a game object to the desired node
	void PrepareCollisionQuery(OctNode* node_, Ray ray_);
};

#endif