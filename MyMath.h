#pragma once

#include <cassert>
#include <cmath>

class MyMath {
public:
	static Vector3 VecAdd(Vector3& vector1, Vector3& vector2) {
		Vector3 result{};
		result.x = vector1.x + vector2.x;
		result.y = vector1.y + vector2.y;
		result.z = vector1.z + vector2.z;


		return result;
	}

	static Vector3 TransformNormal(const Vector3& v, const Matrix4x4& m) {
		Vector3 result{
		    v.x * m.m[0][0] + v.y * m.m[1][0] + v.z * m.m[2][0],
		    v.x * m.m[0][1] + v.y * m.m[1][1] + v.z * m.m[2][1],
		    v.x * m.m[0][2] + v.y * m.m[1][2] + v.z * m.m[2][2],
		};
		return result;
	}

	static Matrix4x4 Add(const Matrix4x4& m1, const Matrix4x4& m2) {
		Matrix4x4 result{};

		for (int row = 0; row < 4; row++) {
			for (int column = 0; column < 4; column++) {
				result.m[row][column] = m1.m[row][column] + m2.m[row][column];
			}
		}
		return result;
	}

	static Matrix4x4 Multiply(const Matrix4x4& m1, const Matrix4x4& m2) {
		Matrix4x4 result{};

		for (int row = 0; row < 4; row++) {
			for (int column = 0; column < 4; column++) {
				result.m[row][column] =
				    m1.m[row][0] * m2.m[0][column] + m1.m[row][1] * m2.m[1][column] +
				    m1.m[row][2] * m2.m[2][column] + m1.m[row][3] * m2.m[3][column];
			}
		}

		/*result.m[0][0] = m1.m[0][0] * m2.m[0][0] + m1.m[0][1] * m2.m[1][0] + m1.m[0][2] *
		m2.m[2][0] + m1.m[0][3] * m2.m[3][0]; result.m[0][1] = m1.m[0][0] * m2.m[0][1] + m1.m[0][1]
		* m2.m[1][1] + m1.m[0][2] * m2.m[2][1] + m1.m[0][3] * m2.m[3][1]; result.m[0][2] =
		m1.m[0][0] * m2.m[0][2] + m1.m[0][1] * m2.m[1][2] + m1.m[0][2] * m2.m[2][2] + m1.m[0][3] *
		m2.m[3][2]; result.m[0][3] = m1.m[0][0] * m2.m[0][3] + m1.m[0][1] * m2.m[1][3] + m1.m[0][2]
		* m2.m[2][3] + m1.m[0][3] * m2.m[3][3];

		result.m[1][0] = m1.m[1][0] * m2.m[0][0] + m1.m[1][1] * m2.m[1][0] + m1.m[1][2] * m2.m[2][0]
		+ m1.m[1][3] * m2.m[3][0]; result.m[1][1] = m1.m[1][0] * m2.m[0][1] + m1.m[1][1] *
		m2.m[1][1] + m1.m[1][2] * m2.m[2][1] + m1.m[1][3] * m2.m[3][1]; result.m[1][2] = m1.m[1][0]
		* m2.m[0][2] + m1.m[1][1] * m2.m[1][2] + m1.m[1][2] * m2.m[2][2] + m1.m[1][3] * m2.m[3][2];
		result.m[1][3] = m1.m[1][0] * m2.m[0][3] + m1.m[1][1] * m2.m[1][3] + m1.m[1][2] * m2.m[2][3]
		+ m1.m[1][3] * m2.m[3][3];

		result.m[2][0] = m1.m[2][0] * m2.m[0][0] + m1.m[2][1] * m2.m[1][0] + m1.m[2][2] * m2.m[2][0]
		+ m1.m[2][3] * m2.m[3][0]; result.m[2][1] = m1.m[2][0] * m2.m[0][1] + m1.m[2][1] *
		m2.m[1][1] + m1.m[2][2] * m2.m[2][1] + m1.m[2][3] * m2.m[3][1]; result.m[2][2] = m1.m[2][0]
		* m2.m[0][2] + m1.m[2][1] * m2.m[1][2] + m1.m[2][2] * m2.m[2][2] + m1.m[2][3] * m2.m[3][2];
		result.m[2][3] = m1.m[2][0] * m2.m[0][3] + m1.m[2][1] * m2.m[1][3] + m1.m[2][2] * m2.m[2][3]
		+ m1.m[2][3] * m2.m[3][3];

		result.m[3][0] = m1.m[3][0] * m2.m[0][0] + m1.m[3][1] * m2.m[1][0] + m1.m[3][2] * m2.m[2][0]
		+ m1.m[3][3] * m2.m[3][0]; result.m[3][1] = m1.m[3][0] * m2.m[0][1] + m1.m[3][1] *
		m2.m[1][1] + m1.m[3][2] * m2.m[2][1] + m1.m[3][3] * m2.m[3][1]; result.m[3][2] = m1.m[3][0]
		* m2.m[0][2] + m1.m[3][1] * m2.m[1][2] + m1.m[3][2] * m2.m[2][2] + m1.m[3][3] * m2.m[3][2];
		result.m[3][3] = m1.m[3][0] * m2.m[0][3] + m1.m[3][1] * m2.m[1][3] + m1.m[3][2] * m2.m[2][3]
		+ m1.m[3][3] * m2.m[3][3];*/

		return result;
	}

	static Matrix4x4 MakeTranslateMatrix(const Vector3& translate) {
		Matrix4x4 result{};

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				result.m[i][j] = 0;
			}
		}

		for (int i = 0; i < 4; i++) {
			result.m[i][i] = 1;
		}

		result.m[3][0] = translate.x;
		result.m[3][1] = translate.y;
		result.m[3][2] = translate.z;

		return result;
	};

	static Matrix4x4 MakeScaleMatrix(const Vector3& scale) {
		Matrix4x4 result{};

		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				result.m[i][j] = 0;
			}
		}
		result.m[0][0] = scale.x;
		result.m[1][1] = scale.y;
		result.m[2][2] = scale.z;
		result.m[3][3] = 1;

		return result;
	}
	
	static Vector3 TransformCoord(Vector3& vector, Matrix4x4& matrix) {
		Vector3 result{};
		result.x = vector.x * matrix.m[0][0] + vector.y * matrix.m[1][0] +
		           vector.z * matrix.m[2][0] + 1.0f * matrix.m[3][0];
		result.y = vector.x * matrix.m[0][1] + vector.y * matrix.m[1][1] +
		           vector.z * matrix.m[2][1] + 1.0f * matrix.m[3][1];
		result.z = vector.x * matrix.m[0][2] + vector.y * matrix.m[1][2] +
		           vector.z * matrix.m[2][2] + 1.0f * matrix.m[3][2];

		float w = vector.x * matrix.m[0][3] + vector.y * matrix.m[1][3] +
		          vector.z * matrix.m[2][3] + 1.0f * matrix.m[3][3];
		assert(w != 0.0f);
		result.x /= w;
		result.y /= w;
		result.z /= w;

		return result;
	}

	static Matrix4x4 MakeRotateXMatrix(float radian) {
		Matrix4x4 result{};

		result.m[0][0] = 1;
		result.m[1][1] = std::cos(radian);
		result.m[1][2] = std::sin(radian);
		result.m[2][1] = std::sin(-radian);
		result.m[2][2] = std::cos(radian);
		result.m[3][3] = 1;

		return result;
	}
	
	static Matrix4x4 MakeRotateYMatrix(float radian) {
		Matrix4x4 result{};

		result.m[0][0] = std::cos(radian);
		result.m[0][2] = std::sin(-radian);
		result.m[1][1] = 1;
		result.m[2][0] = std::sin(radian);
		result.m[2][2] = std::cos(radian);
		result.m[3][3] = 1;

		return result;
	}

	static Matrix4x4 MakeRotateZMatrix(float radian) {
		Matrix4x4 result{};

		result.m[0][0] = std::cos(radian);
		result.m[1][0] = std::sin(-radian);
		result.m[0][1] = std::sin(radian);
		result.m[1][1] = std::cos(radian);
		result.m[2][2] = 1;
		result.m[3][3] = 1;

		return result;
	}

	static Matrix4x4
	    MakeAffineMatrix(const Vector3& scale, const Vector3& rotate, const Vector3& translate) {
		/*Matrix4x4 result{};

		for (int i = 0; i < 4; i++) {
		    for (int j = 0; j < 4; j++) {
		        result.m[i][j] = scale.m[i][j] * rotate.m[i][j] * translate.m[i][j];
		    }
		}
		return result;*/

		Matrix4x4 result{};
		Matrix4x4 translateMatrix = MakeTranslateMatrix(translate);

		Matrix4x4 scaleMatrix = MakeScaleMatrix(scale);
		Matrix4x4 rotateXYZMatrix = Multiply(
		    MakeRotateXMatrix(rotate.x),
		    Multiply(MakeRotateYMatrix(rotate.y), MakeRotateZMatrix(rotate.z)));

		result.m[0][0] = rotateXYZMatrix.m[0][0] * scaleMatrix.m[0][0];
		result.m[0][1] = rotateXYZMatrix.m[0][1] * scaleMatrix.m[0][0];
		result.m[0][2] = rotateXYZMatrix.m[0][2] * scaleMatrix.m[0][0];
		result.m[0][3] = 0;

		result.m[1][0] = rotateXYZMatrix.m[1][0] * scaleMatrix.m[1][1];
		result.m[1][1] = rotateXYZMatrix.m[1][1] * scaleMatrix.m[1][1];
		result.m[1][2] = rotateXYZMatrix.m[1][2] * scaleMatrix.m[1][1];
		result.m[1][3] = 0;

		result.m[2][0] = rotateXYZMatrix.m[2][0] * scaleMatrix.m[3][3];
		result.m[2][1] = rotateXYZMatrix.m[2][1] * scaleMatrix.m[3][3];
		result.m[2][2] = rotateXYZMatrix.m[2][2] * scaleMatrix.m[3][3];
		result.m[2][3] = 0;

		result.m[3][0] = translateMatrix.m[3][0];
		result.m[3][1] = translateMatrix.m[3][1];
		result.m[3][2] = translateMatrix.m[3][2];
		result.m[3][3] = 1;

		return result;
	}

	/*static Vector3 operator+(const Vector3 v1, const Vector3 v2) {
		Vector3 result{};
		result.x = v2.x + v1.x;
		result.y = v2.x + v1.y;
		result.z = v2.x + v1.z;
		return result;
	}*/

	
 private: 

};
















