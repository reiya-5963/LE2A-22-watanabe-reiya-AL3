#pragma once

/// <summary>
/// 3次元ベクトル
/// </summary>
struct Vector3 final {
	float x;
	float y;
	float z;

	const Vector3 operator+(const Vector3& v1) {
		Vector3 result{};
		result.x = x + v1.x;
		result.y = y + v1.y;
		result.z = z + v1.z;
		return result;
	}

	const Vector3 operator+=(const Vector3& v1) {
		Vector3 result{};
		result.x += v1.x;
		result.y += v1.y;
		result.z += v1.z;
		return result;
	}


	const Vector3 operator-(const Vector3& v1) {
		Vector3 result{};
		result.x = x - v1.x;
		result.y = y - v1.y;
		result.z = z - v1.z;
		return result;
	}

	const Vector3 operator-=(const Vector3& v1) {
		Vector3 result{};
		result.x -= v1.x;
		result.y -= v1.y;
		result.z -= v1.z;
		return result;
	}


	const bool operator<=(const Vector3& v1) {
		if (x <= v1.x && y <= v1.y && z <= v1.z) {
			return true;
		}

		return false;
	};

	const bool operator<(const Vector3& v1) {
		if (x < v1.x && y < v1.y && z < v1.z) {
			return true;
		}

		return false;
	};

	const bool operator>=(const Vector3& v1) {
		if (x >= v1.x && y >= v1.y && z >= v1.z) {
			return true;
		}

		return false;
	};

	const bool operator>(const Vector3& v1) {
		if (x > v1.x && y > v1.y && z > v1.z) {
			return true;
		}

		return false;
	};

	const Vector3 operator*(const float& f1) {
		Vector3 result{};
		result.x = x * f1;
		result.y = y * f1;
		result.z = z * f1;
		return result;
	}
};