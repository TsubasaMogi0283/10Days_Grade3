#include "PlayerFunc.h"



// 線形補間関数（Lerp）
float pFunc::Lerp(float a, float b, float t)
{
    return a + t * (b - a);
}


// 角度を 0～2π の範囲に正規化
float pFunc::NormalizeAngle(float angle)
{
    const float TWO_PI = 2.0f * 3.14159265359f;
    const float PI = 3.14159265359f;

    while (angle < -PI) angle += TWO_PI;
    while (angle > PI) angle -= TWO_PI;
    return angle;
}


// 角度の補正
float pFunc::ShortestAngle(float currentAngle, float targetAngle)
{
    // 角度の差を計算
    float angleDifference = targetAngle - currentAngle;

    // 角度を -π から +π の範囲に正規化する
    while (angleDifference > 3.14159265f) angleDifference -= 2.0f * 3.14159265f;
    while (angleDifference < -3.14159265f) angleDifference += 2.0f * 3.14159265f;

    return angleDifference; // 最短回転角度を返す
}

