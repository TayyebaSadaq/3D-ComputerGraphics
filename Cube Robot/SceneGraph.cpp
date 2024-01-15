// SceneGraph.cpp

#include "SceneGraph.h"  // Include the header file that declares the SceneNode class

// Implementation of the SceneNode class methods

bool SceneGraph::Initialise() {
    // Implement the logic for Initialise method
    for (auto child : _children) {
        if (!child->Initialise()) {
            return false;
        }
    }
    return true;
}

void SceneGraph::Update(const Matrix& worldTransformation) {
    // Implement the logic for Update method
    SceneNode::Update(worldTransformation);
    for (SceneNodePointer child : _children) {
        child->Update(_cumulativeWorldTransformation);
    }
}

void SceneGraph::Render() {
    // Implement the logic for Render method
    for (auto child : _children) {
        child->Render();
    }
}

void SceneGraph::Shutdown() {
    // Implement the logic for Shutdown method
    for (auto child : _children) {
        child->Shutdown();
    }
}

void SceneGraph::Add(SceneNodePointer node) {
    // Implement the logic for Add method
    _children.push_back(node);
}

void SceneGraph::Remove(SceneNodePointer node) {
    auto it = std::remove(_children.begin(), _children.end(), node);
    _children.erase(it, _children.end());
}

SceneNodePointer SceneGraph::Find(const std::wstring name) {
    // Implement the logic for Find method
    if (_name == name) {
        return shared_from_this();
    }

    for (auto child : _children) {
        SceneNodePointer  foundNode = child->Find(name);
        if (foundNode != nullptr) {
            return foundNode;
        }
    }

    return nullptr;
}
