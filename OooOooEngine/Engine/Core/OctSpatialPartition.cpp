#include "OctSpatialPartition.h"

// OCT NODE CLASS FUNCTIONS ///////////////////////////////////////////////////////////////////////

OctNode::OctNode(glm::vec3 position_, float size_, OctNode* parent_) {
	octBounds = new BoundingBox();
	octBounds->minVert = position_;
	octBounds->maxVert = position_ + glm::vec3(size_);

	size = size_;
	parent = parent_;

	objects = std::vector<GameObject*>();
	objects.reserve(10);

	for (int i = 0; i < NUM_CHILDREN; i++) {
		children[i] = nullptr;
	}
}

OctNode::~OctNode() {
	OnDestroy();
}

void OctNode::OnDestroy() {
	delete octBounds, octBounds = nullptr;
	if (objects.size() > 0) {
		for (auto o : objects) {
			o = nullptr;
		}
		objects.clear();
	}
	parent = nullptr;
	for (int i = 0; i < NUM_CHILDREN; i++) {
		if (children[i] != nullptr) {
			delete children[i], children[i] = nullptr;
		}
	}
}

void OctNode::Octify(int depth_) {
	if (depth_ > 0 && this) { // "this" ensures that the current node exists in memory
		float halfSize = size / 2.0f;

		children[OCT_CHILDREN::TLF] = new OctNode(glm::vec3(octBounds->minVert.x, octBounds->minVert.y + halfSize, octBounds->minVert.z + halfSize), halfSize, this);
		children[OCT_CHILDREN::BLF] = new OctNode(glm::vec3(octBounds->minVert.x, octBounds->minVert.y, octBounds->minVert.z + halfSize), halfSize, this);
		children[OCT_CHILDREN::TRF] = new OctNode(glm::vec3(octBounds->minVert.x + halfSize, octBounds->minVert.y + halfSize, octBounds->minVert.z + halfSize), halfSize, this);
		children[OCT_CHILDREN::BRF] = new OctNode(glm::vec3(octBounds->minVert.x + halfSize, octBounds->minVert.y, octBounds->minVert.z + halfSize), halfSize, this);

		children[OCT_CHILDREN::TLR] = new OctNode(glm::vec3(octBounds->minVert.x, octBounds->minVert.y + halfSize, octBounds->minVert.z), halfSize, this);
		children[OCT_CHILDREN::BLR] = new OctNode(glm::vec3(octBounds->minVert), halfSize, this);
		children[OCT_CHILDREN::TRR] = new OctNode(glm::vec3(octBounds->minVert.x + halfSize, octBounds->minVert.y + halfSize, octBounds->minVert.z), halfSize, this);
		children[OCT_CHILDREN::BRR] = new OctNode(glm::vec3(octBounds->minVert.x + halfSize, octBounds->minVert.y, octBounds->minVert.z), halfSize, this);

		// Octify all the children
		for (int i = 0; i < NUM_CHILDREN; i++) {
			children[i]->Octify(depth_ - 1);
		}
	}
}

OctNode* OctNode::GetParent() {
	return parent;
}

OctNode* OctNode::GetChild(OCT_CHILDREN index_) {
	if (index_ < 0 || index_ >= NUM_CHILDREN) {
		std::cout << "Tried to access an invalid array index! Returning an index of 0 as a default." << std::endl;
		return children[0];	// Just return the first object in the list if they try to retrieve one that is out of the array's bounds
	}
	return children[index_];
}

bool OctNode::IsLeafNode() const {
	if (children[0] == nullptr) { // The array will only ever have nullptr or OctNode pointers, so only one element check is required
		return true;
	}
	return false;
}

void OctNode::AddCollisionObject(GameObject* object_) {
	objects.push_back(object_);
}

int OctNode::GetObjectCount() const {
	return objects.size();
}

BoundingBox* OctNode::GetBoundingBox() const {
	return octBounds;
}

///////////////////////////////////////////////////////////////////////////////////////////////////

// OCT SPACIAL PARTITION CLASS FUNCTIONS //////////////////////////////////////////////////////////

OctSpatialPartition::OctSpatialPartition(float worldSize_, float depth_) {
	rootNode = new OctNode(glm::vec3(-worldSize_ / 2.0f), worldSize_, nullptr);
	rootNode->Octify(depth_);

	rayIntersectList = std::vector<OctNode*>();
}

OctSpatialPartition::~OctSpatialPartition() {

}

void OctSpatialPartition::OnDestroy() {
	if (rayIntersectList.size() > 0) {
		for (auto node : rayIntersectList) {
			node = nullptr;
		}
		rayIntersectList.clear();
	}
	delete rootNode, rootNode = nullptr;
}

void OctSpatialPartition::AddObject(GameObject* object_) {
	AddObjectToNode(rootNode, object_);
}

GameObject* OctSpatialPartition::GetCollidedObject(Ray ray_) {
	if (rayIntersectList.size() > 0) {
		for (auto node : rayIntersectList) {
			node = nullptr;
		}
		rayIntersectList.clear();
	}
	rayIntersectList.reserve(20);
	PrepareCollisionQuery(rootNode, ray_);

	GameObject* result = nullptr;
	float shortestDist = FLT_MAX;
	for (auto node : rayIntersectList) {
		for (auto o : node->objects) {
			if (ray_.IsColliding(&o->GetBoundingBox())) {
				if (ray_.intersectDist < shortestDist) {
					shortestDist = ray_.intersectDist;
					result = o;
				}
			}
		}
		// If an object has been hit, instantly return it
		if (result != nullptr) {
			return result;
		}
	}
	return nullptr;
}

void OctSpatialPartition::AddObjectToNode(OctNode* node_, GameObject* object_) {
	if (node_) {
		BoundingBox* nodeBox = node_->GetBoundingBox();
		BoundingBox objectBox = object_->GetBoundingBox();
		if (objectBox.IntersectsBox(nodeBox)) {
			if (node_->IsLeafNode()) { // If it's a leaf node, add the object to the node
				node_->AddCollisionObject(object_);
			} else { // If not, loop through all the children nodes
				for (int i = 0; i < NUM_CHILDREN; i++) {
					AddObjectToNode(node_->children[i], object_);
				}
			}
		}
	}
}

void OctSpatialPartition::PrepareCollisionQuery(OctNode* node_, Ray ray_) {
	if (node_) {
		if (ray_.IsColliding(node_->GetBoundingBox())) {
			if (node_->IsLeafNode()) { // If it's a leaf node, add the node to the ray intersection list
				rayIntersectList.push_back(node_);
			} else { // If not, loop through all the children nodes
				for (int i = 0; i < NUM_CHILDREN; i++) {
					PrepareCollisionQuery(node_->children[i], ray_);
				}
			}
		}
	}
}

///////////////////////////////////////////////////////////////////////////////////////////////////