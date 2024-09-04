#pragma once
#include "Matrix4x4.h"
#include "Vector3.h"


#include <cassert>
#define _USE_MATH_DEFINES
#include <cmath>
#include <math.h>


namespace Matrix4x4Calculation {
	
	/// <summary>
	/// 単位行列を作成する。
	/// 斜めに1が並ぶ行列
	/// </summary>
	/// <returns></returns>
	Matrix4x4 MakeIdentity4x4();

	/// <summary>
	/// 乗算
	/// </summary>
	/// <param name="m1"></param>
	/// <param name="m2"></param>
	/// <returns></returns>
	Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2);


	/// <summary>
	/// 拡縮
	/// </summary>
	/// <param name="scale"></param>
	/// <returns></returns>
	Matrix4x4 MakeScaleMatrix(const Vector3& scale);

#pragma region 個別の回転行列
	/// <summary>
	/// X回転
	/// </summary>
	/// <param name="radian"></param>
	/// <returns></returns>
	Matrix4x4 MakeRotateXMatrix(const float& radian);

	/// <summary>
	/// Y回転
	/// </summary>
	/// <param name="radian"></param>
	/// <returns></returns>
	Matrix4x4 MakeRotateYMatrix(const float& radian);

	/// <summary>
	/// Z回転
	/// </summary>
	/// <param name="radian"></param>
	/// <returns></returns>
	Matrix4x4 MakeRotateZMatrix(const float& radian);

#pragma endregion

	/// <summary>
	/// 回転行列
	/// </summary>
	/// <param name="radianX"></param>
	/// <param name="radianY"></param>
	/// <param name="radianZ"></param>
	/// <returns></returns>
	Matrix4x4 MakeRotateXYZMatrix(const float& radianX, const float& radianY, const float& radianZ);


	/// <summary>
	/// 移動
	/// </summary>
	/// <param name="translate"></param>
	/// <returns></returns>
	Matrix4x4 MakeTranslateMatrix(const Vector3& translate);


	
	/// <summary>
	/// アフィン行列
	/// </summary>
	/// <param name="scale"></param>
	/// <param name="rotate"></param>
	/// <param name="translate"></param>
	/// <returns></returns>
	Matrix4x4 MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate);

	/// <summary>
	/// 逆行列
	/// </summary>
	/// <param name="m"></param>
	/// <returns></returns>
	Matrix4x4 Inverse(const Matrix4x4& m);


	/// <summary>
	/// 遠視投影行列
	/// </summary>
	/// <param name="fovY"></param>
	/// <param name="aspectRatio"></param>
	/// <param name="nearClip"></param>
	/// <param name="farClip"></param>
	/// <returns></returns>
	Matrix4x4 MakePerspectiveFovMatrix(const float& fovY, const float& aspectRatio, const float& nearClip, const float& farClip);
	
	/// <summary>
	/// 正射影行列
	/// </summary>
	/// <param name="left"></param>
	/// <param name="top"></param>
	/// <param name="right"></param>
	/// <param name="bottom"></param>
	/// <param name="neaClip"></param>
	/// <param name="farClip"></param>
	/// <returns></returns>
	Matrix4x4 MakeOrthographicMatrix(const float& left, const float& top, const float& right, const float& bottom, const float& neaClip, const float& farClip);

	/// <summary>
	/// 転置行列
	/// </summary>
	/// <param name="m"></param>
	/// <returns></returns>
	Matrix4x4 MakeTransposeMatrix(const Matrix4x4& m);
}











