#include <Novice.h>
#include "MyMath.h"
#include "MyDraw.h"
#include "ImGuiManager.h"

const char kWindowTitle[] = "MT3_LE2A_22_ワタナベレイヤ";

//画面のサイズ
const int kWindowWidth = 1280;
const int kWindowHeight = 720;


// Windowsアプリでのエントリーポイント(main関数)
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {

	// ライブラリの初期化
	Novice::Initialize(kWindowTitle, kWindowWidth, kWindowHeight);

	//カメラ関係
	Vector3 cameraTranslate{ 0.0f, 1.9f, -6.49f };
	Vector3 cameraRotate{ 0.26f, 0.0f, 0.0f };

	//平面
	Vector3 rotate{ 0.0f, 0.0f, 0.0f };

	OBB obb{
		.center{-1.0f, 0.0f, 0.0f},
		.orientations = {{1.0f, 0.0f, 0.0f},
						 {0.0f, 1.0f,0.0f},
						 {0.0f, 0.0f, 1.0f}},
		.size{0.5f, 0.5f, 0.5f}
	};


	Sphere sphere{
		.center{0.0f, 0.0f, 0.0f},
		.radius{0.5f}
	};


	//色
	uint32_t colorS1 = WHITE;
	uint32_t colorS2 = WHITE;


	Matrix4x4 worldMatrix = MyMath::MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
	Matrix4x4 cameraMatrix = MyMath::MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, cameraRotate, cameraTranslate);
	Matrix4x4 viewMatrix = MyMath::Inverse(cameraMatrix);
	Matrix4x4 projectionMatrix = MyMath::MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
	Matrix4x4 worldViewProjectionMatrix = MyMath::Multiply(worldMatrix, MyMath::Multiply(viewMatrix, projectionMatrix));
	Matrix4x4 viewportMatrix = MyMath::MakeViewPortMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);

	// キー入力結果を受け取る箱
	char keys[256] = { 0 };
	char preKeys[256] = { 0 };

	// ウィンドウの×ボタンが押されるまでループ
	while (Novice::ProcessMessage() == 0) {
		// フレームの開始
		Novice::BeginFrame();

		// キー入力を受け取る
		memcpy(preKeys, keys, 256);
		Novice::GetHitKeyStateAll(keys);

		///
		/// ↓更新処理ここから
		///

		Matrix4x4 trans = MyMath::MakeTranslateMatrix(cameraTranslate);
		Vector3 move{};
		if (keys[DIK_SPACE]) {
			move.y += 0.1f;
		}
		if (keys[DIK_LCONTROL]) {
			move.y -= 0.1f;
		}
		if (keys[DIK_W]) {
			move.z += 0.1f;
		}
		if (keys[DIK_S]) {
			move.z -= 0.1f;
		}
		if (keys[DIK_A]) {
			move.x -= 0.1f;
		}
		if (keys[DIK_D]) {
			move.x += 0.1f;
		}
		if (keys[DIK_RIGHTARROW]) {
			cameraRotate.y += 0.1f;
		}
		if (keys[DIK_LEFTARROW]) {
			cameraRotate.y -= 0.1f;
		}
		//Vector3 cameraDir = MyMath::Normalize(cameraRotate);
		/*move.x = move.x * cameraDir.x;
		move.y = move.y * cameraDir.y;
		move.z = move.z * cameraDir.z;*/

		cameraTranslate = MyMath::TransformCoord(move, trans);
		//諸々の変換
		worldMatrix = MyMath::MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 0.0f }, { 0.0f, 0.0f, 0.0f });
		cameraMatrix = MyMath::MakeAffineMatrix({ 1.0f, 1.0f, 1.0f }, cameraRotate, cameraTranslate);
		viewMatrix = MyMath::Inverse(cameraMatrix);
		projectionMatrix = MyMath::MakePerspectiveFovMatrix(0.45f, float(kWindowWidth) / float(kWindowHeight), 0.1f, 100.0f);
		worldViewProjectionMatrix = MyMath::Multiply(worldMatrix, MyMath::Multiply(viewMatrix, projectionMatrix));
		viewportMatrix = MyMath::MakeViewPortMatrix(0, 0, float(kWindowWidth), float(kWindowHeight), 0.0f, 1.0f);


		Matrix4x4 rotateMatrix = MyMath::Multiply(MyMath::MakeRotateXMatrix(rotate.x), MyMath::Multiply(MyMath::MakeRotateYMatrix(rotate.y), MyMath::MakeRotateZMatrix(rotate.z)));

		obb.orientations[0].x = rotateMatrix.m[0][0];
		obb.orientations[0].y = rotateMatrix.m[0][1];
		obb.orientations[0].z = rotateMatrix.m[0][2];

		obb.orientations[1].x = rotateMatrix.m[1][0];
		obb.orientations[1].y = rotateMatrix.m[1][1];
		obb.orientations[1].z = rotateMatrix.m[1][2];

		obb.orientations[2].x = rotateMatrix.m[2][0];
		obb.orientations[2].y = rotateMatrix.m[2][1];
		obb.orientations[2].z = rotateMatrix.m[2][2];



		//当たり判定
	/*	if (MyMath::IsCollision(aabb1, segment)) {
			colorS1 = RED;
		}
		else {
			colorS1 = WHITE;
		}*/



		///
		/// ↑更新処理ここまで
		///



		///
		/// ↓描画処理ここから
		///

		MyDraw::DrawGrid(worldViewProjectionMatrix, viewportMatrix);

		//各描画
		MyDraw::DrawShere(sphere, worldViewProjectionMatrix, viewportMatrix, colorS1);
	
		
		MyDraw::DrawOBB(obb, worldViewProjectionMatrix, viewportMatrix, colorS2);
		//MyDraw::DrawLine(segment, worldViewProjectionMatrix, viewportMatrix, colorS2);



		//デバッグ
		ImGui::Begin("Debug");
		ImGui::SliderFloat3("abb.center", &obb.center.x, -1.0f, 1.0f);
		ImGui::SliderAngle("rotateX", &rotate.x);
		ImGui::SliderAngle("rotateY", &rotate.y);
		ImGui::SliderAngle("rotateZ", &rotate.z);
		ImGui::SliderFloat3("abb.orientations[0]", &obb.orientations[0].x, -1.0f, 1.0f);
		ImGui::SliderFloat3("abb.orientations[1]", &obb.orientations[1].x, -1.0f, 1.0f);
		ImGui::SliderFloat3("abb.orientations[2]", &obb.orientations[2].x, -1.0f, 1.0f);
		ImGui::SliderFloat3("abb.size", &obb.size.x, -1.0f, 1.0f);
		ImGui::SliderFloat3("sphere.center", &sphere.center.x, -1.0f, 1.0f);
		ImGui::SliderFloat("sphere.radius", &sphere.radius, -1.0f, 1.0f);


		/*ImGui::DragFloat3("cameraTRa", &cameraTranslate.x, 0.1f, -50.0f, 50.0f);
		ImGui::DragFloat3("cameraRot", &cameraRotate.x, 0.1f, -50.0f, 50.0f);

		ImGui::DragFloat3("AABB1min", &aabb1.min.x, 0.1f, -1.0f, 5.0f);
		ImGui::DragFloat3("AABB1max", &aabb1.max.x, 0.1f, -1.0f, 5.0f);
		ImGui::DragFloat3("SegO", &segment.origin.x, 0.1f, -1.0f, 5.0f);
		ImGui::DragFloat3("SegD", &segment.diff.x, 0.1f, -1.0f, 5.0f);*/

		//ImGui::DragFloat3("sphereC", &sphere.center.x, 0.1f, -1.0f, 5.0f);
		//ImGui::DragFloat("sphereR", &sphere.radius, 0.1f, -1.0f, 5.0f);
		ImGui::End();

		///
		/// ↑描画処理ここまで
		///

		// フレームの終了
		Novice::EndFrame();

		// ESCキーが押されたらループを抜ける
		if (preKeys[DIK_ESCAPE] == 0 && keys[DIK_ESCAPE] != 0) {
			break;
		}
	}

	// ライブラリの終了
	Novice::Finalize();
	return 0;
}
