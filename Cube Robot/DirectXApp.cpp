#include "DirectXApp.h"
#include "CubeNode.h"



DirectXApp app;

void DirectXApp::CreateSceneGraph()
{
    SceneGraphPointer sceneGraph = GetSceneGraph();

    // Body
    shared_ptr<CubeNode> body = make_shared<CubeNode>(L"Body", Vector4(1.0f, 0.0f, 1.0f, 1.0f)); //Magenta
    body->SetWorldTransform(Matrix::CreateScale(Vector3(5.0f, 8.0f, 2.5f)) * Matrix::CreateTranslation(Vector3(0.0f, 23.0f, 0.0f)));
    sceneGraph->Add(body);

    // Left Leg
    shared_ptr<CubeNode> leftLeg = make_shared<CubeNode>(L"LeftLeg", Vector4(1.0f, 0.0f, 1.0f, 1.0f)); //Magenta
    leftLeg->SetWorldTransform(Matrix::CreateScale(Vector3(1.0f, 7.5f, 1.0f)) * Matrix::CreateTranslation(Vector3(-4.0f, 7.5f, 0.0f)));
    sceneGraph->Add(leftLeg);

    // Right Leg
    shared_ptr<CubeNode> rightLeg = make_shared<CubeNode>(L"RightLeg", Vector4(1.0f, 0.0f, 1.0f, 1.0f)); //Magenta
    rightLeg->SetWorldTransform(Matrix::CreateScale(Vector3(1.0f, 7.5f, 1.0f)) * Matrix::CreateTranslation(Vector3(4.0f, 7.5f, 0.0f)));
    sceneGraph->Add(rightLeg);

    // Head
    shared_ptr<CubeNode> head = make_shared<CubeNode>(L"Head", Vector4(1.0f, 0.0f, 1.0f, 1.0f)); //Magenta
    head->SetWorldTransform(Matrix::CreateScale(Vector3(3.0f, 3.0f, 3.0f)) * Matrix::CreateTranslation(Vector3(0.0f, 34.0f, 0.0f)));
    sceneGraph->Add(head);

    // Create a scene graph for the left shoulder
    SceneGraphPointer leftShoulderSceneGraph = std::make_shared<SceneGraph>(L"LeftShoulder"); 
    leftShoulderSceneGraph->SetWorldTransform(Matrix::CreateTranslation(Vector3(-shoulderOffsetX, shoulderOffsetY, shoulderOffsetZ)));
    sceneGraph->Add(leftShoulderSceneGraph);

    // Create a scene graph for the right shoulder
    SceneGraphPointer rightShoulderSceneGraph = std::make_shared<SceneGraph>(L"RightShoulder");
    rightShoulderSceneGraph->SetWorldTransform(Matrix::CreateTranslation(Vector3(shoulderOffsetX, shoulderOffsetY, shoulderOffsetZ)));
    sceneGraph->Add(rightShoulderSceneGraph);

    // Left Arm
    shared_ptr<CubeNode> leftArm = make_shared<CubeNode>(L"LeftArm", Vector4(1.0f, 0.0f, 1.0f, 1.0f)); //Magenta
    leftArm->SetWorldTransform(Matrix::CreateTranslation(Vector3(-6.0f, 22.0f, 0.0f)));
    leftShoulderSceneGraph->Add(leftArm);

    // Right Arm
    shared_ptr<CubeNode> rightArm = make_shared<CubeNode>(L"RightArm", Vector4(1.0f, 0.0f, 1.0f, 1.0f)); //Magenta  
    rightArm->SetWorldTransform(Matrix::CreateTranslation(Vector3(6.0f, 22.0f, 0.0f)));
    rightShoulderSceneGraph->Add(rightArm);

    _rotationAngle = 0;
    _yOffset = 0.0f;

}


void DirectXApp::UpdateSceneGraph()
{

    shoulderOffsetX = 0.0f;
    shoulderOffsetY = -4.25;
    shoulderOffsetZ = 0.0f;

    SceneGraphPointer sceneGraph = GetSceneGraph();

    // Apply rotation to the entire robot
    _rotationAngle += 0.5f;
    Matrix rotationMatrix = Matrix::CreateRotationY(_rotationAngle * XM_PI / 180.0f);
    sceneGraph->SetWorldTransform(rotationMatrix);

    // Define rotation angles for the arms
    float leftArmRotation = sin(_rotationAngle * XM_PI / 180.0f) * 180.0f;  // Swinging left arm
    float rightArmRotation = -sin(_rotationAngle * XM_PI / 180.0f) * 180.0f;  // Swinging right arm

    // Find and update the left arm node
    SceneNodePointer leftShoulderNode = sceneGraph->Find(L"LeftShoulder");
    if (leftShoulderNode) {
        // Set world transformation for the left arm directly attached to the body
        Matrix leftShoulderTransform =
            Matrix::CreateTranslation(Vector3(-shoulderOffsetX, shoulderOffsetY, shoulderOffsetZ))
            * Matrix::CreateRotationX(leftArmRotation * XM_PI / 180.0f)
            * Matrix::CreateTranslation(Vector3(-6.0f, 30.0f, 0.0f));

        leftShoulderNode->SetWorldTransform(leftShoulderTransform);


        // Find and update the left arm node
        SceneNodePointer leftArmNode = sceneGraph->Find(L"LeftArm");
        if (leftArmNode) {
            leftArmNode->SetWorldTransform(Matrix::CreateScale(Vector3(1.0f, 8.5f, 1.0f)) * Matrix::CreateTranslation(Vector3(0, -4.25f, 0)) * Matrix::CreateRotationY(leftArmRotation * XM_PI / 180.0f));

        }

    }


        // Find and update the right arm node
        SceneNodePointer rightShoulderNode = sceneGraph->Find(L"RightShoulder");
        if (rightShoulderNode) {
            // Set world transformation for the right arm directly attached to the body
            Matrix rightShoulderTransform =
                Matrix::CreateTranslation(Vector3(shoulderOffsetX, shoulderOffsetY, shoulderOffsetZ))
                * Matrix::CreateRotationX(rightArmRotation * XM_PI / 180.0f)
                * Matrix::CreateTranslation(Vector3(6.0f, 30.0f, 0.0f));

            rightShoulderNode->SetWorldTransform(rightShoulderTransform);


            // Find and update the right arm node
            SceneNodePointer rightArmNode = sceneGraph->Find(L"RightArm");
            if (rightArmNode) {
                rightArmNode->SetWorldTransform(Matrix::CreateScale(Vector3(1.0f, 8.5f, 1.0f)) * Matrix::CreateTranslation(Vector3(0, -4.25f, 0)) * Matrix::CreateRotationY(leftArmRotation * XM_PI / 180.0f));
            }

        }
}

 


       