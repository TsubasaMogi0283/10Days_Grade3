#pragma once

#include <iostream>
#include <cstdint>
#include <string>
#include <format>
#include <cassert>
#include <wrl.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <cassert>
#include "imgui.h"
#include <numbers>
#include <type_traits>
#include <filesystem>

#include "Vector2.h"
#include "Vector3.h"
#include "Vector4.h"
#include "Matrix3x3.h"
#include "Matrix4x4.h"


namespace pFunc {

	// 線形補間関数（Lerp）
	float Lerp(float a, float b, float t);

	// 角度を 0～2π の範囲に正規化
	float NormalizeAngle(float angle);

	// 角度の補正
	float ShortestAngle(float currentAngle, float targetAngle);

}

// Enumのサイズを取得するメタ関数
template<typename E, typename = void>
struct EnumSize;

// 通常のenumとenum classの両方に対応するための特殊化
template<typename E>
struct EnumSize<E, std::void_t<decltype(E::Count)>> {
	static constexpr std::size_t value = static_cast<std::size_t>(E::Count);
};

template<typename E>
constexpr auto enum_val(E e) noexcept {
	return static_cast<std::underlying_type_t<E>>(e);
}