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
    // 角度を -π ～ π に正規化
    currentAngle = NormalizeAngle(currentAngle);
    targetAngle = NormalizeAngle(targetAngle);

    // 角度の差を計算
    float delta = targetAngle - currentAngle;

    // 差が π を超える場合、逆方向に調整
    if (delta > 3.14159265359f) {
        delta -= 2.0f * 3.14159265359f;
    }
    else if (delta < -3.14159265359f) {
        delta += 2.0f * 3.14159265359f;
    }

    return currentAngle + delta;
}
