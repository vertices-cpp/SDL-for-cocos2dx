#ifndef _OG_TYPE_HPP_
#define _OG_TYPE_HPP_

#include "OGTypes.h"
#include "Vec2.h"
#include <stdint.h>

OG_BEGIN

typedef struct V2F
{
	float x;
	float y;
} V2F;

typedef struct Color4C {
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
}Color4C;

typedef struct V2F_C4C_QUAD {
	V2F position;        /**< Vertex position, in SDL_Renderer coordinates  */
	Color4C  color;           /**< Vertex color */
	V2F tex_coord;       /**< Normalized texture coordinates, if needed */
}V2F_C4C_QUAD;

#define MATRIX_SIZE ( sizeof(float) * 12)

class   Mat3
{
public:
	 
#ifdef __SSE__
	union {
		__m128 col[3];
		float m[12];
	};
#else
	float m[12];
#endif

 
	Mat3() {
		*this = IDENTITY;
	}

	
	Mat3(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24,
		float m31) {
		set(m11, m12, m13, m14, m21, m22, m23, m24, m31);
	}
	Mat3(const float* mat) {
		set(mat);
	}
 
	Mat3(const Mat3& copy) 
	{
		memcpy(m, copy.m, MATRIX_SIZE);
	}

	 
	~Mat3() {}

	/*
	
	                  sx    0    0

[X, Y, 1] = [x, y, 1][ 0    sy    0  ] ;

					  0     0     1
	
	
	*/
	static void createScale(const Vec2& scale, Mat3* dst) {
	//	GP_ASSERT(dst);
		memcpy(dst, &IDENTITY, MATRIX_SIZE);
		dst->m[0] = scale.x;
		dst->m[4] = scale.y;
	}
	//ÁÐÊ½
	 
	static void createScale(float xScale, float yScale, Mat3* dst)
	{
		memcpy(dst, &IDENTITY, MATRIX_SIZE);

		dst->m[0] = xScale;
		dst->m[4] = yScale;
 
	}

	 
//	static void createRotation(const Quaternion& quat, Mat3* dst);
   /*
				       cosa   sina  0

 [X, Y, 1] = [x, y, 1][-sina  cosa  0  ]

		               0        0     1
   */
	static void createRotation(const Vec2& axis, float angle, Mat3* dst)
	{
		float x = axis.x;
		float y = axis.y;

		float n = x * x + y * y ;
		if (n != 1.0f)
		{
			// Not normalized.
			n = std::sqrt(n);
			// Prevent divide too close to zero.
			if (n > 0.000001f)
			{
				n = 1.0f / n;
				x *= n;
				y *= n;
			//	z *= n;
			}
		}

		float c = std::cos(angle);
		float s = std::sin(angle);

		float t = 1.0f - c;
		float tx = t * x;
		float ty = t * y;
	//	float tz = t * z;
		float txy = tx * y;
	//	float txz = tx * z;
	//	float tyz = ty * z;
		float sx = s * x;
		float sy = s * y;
	//	float sz = s * z;

		dst->m[0] = c + tx * x;
		dst->m[1] = txy + sz;
	//	dst->m[2] = txz - sy;
		dst->m[3] = 0.0f;

		dst->m[4] = txy - sz;
		dst->m[5] = c + ty * y;
	//	dst->m[6] = tyz + sx;
		dst->m[7] = 0.0f;

// 		dst->m[8] = txz + sy;
// 		dst->m[9] = tyz - sx;
// 		dst->m[10] = c + tz * z;
		dst->m[11] = 0.0f;

		dst->m[12] = 0.0f;
		dst->m[13] = 0.0f;
		dst->m[14] = 0.0f;
		dst->m[15] = 1.0f;

	}
 
	static void createRotationX(float angle, Mat3* dst);

	 
	static void createRotationY(float angle, Mat3* dst);

	 
	static void createRotationZ(float angle, Mat3* dst);

	 
	static void createTranslation(const Vec3& translation, Mat3* dst);

	 
	static void createTranslation(float xTranslation, float yTranslation, float zTranslation, Mat3* dst);

	 
	 
	void add(float scalar, Mat3* dst);

 
	void add(const Mat3& mat);

	 
	static void add(const Mat3& m1, const Mat3& m2, Mat3* dst);

	 
	bool decompose(Vec2* scale, Quaternion* rotation, Vec2* translation) const;
 
	float determinant() const;
 
	void getScale(Vec2* scale) const;

 
	bool getRotation(Quaternion* rotation) const;
 
	void getTranslation(Vec3* translation) const;
 
	void getUpVector(Vec3* dst) const;
 
	void getDownVector(Vec3* dst) const;
 
	void getLeftVector(Vec3* dst) const;

 
	void getRightVector(Vec3* dst) const;
 
	void getForwardVector(Vec3* dst) const;
 
	void getBackVector(Vec3* dst) const;

 
	bool inverse();
 
	Mat3 getInversed() const;
 
	bool isIdentity() const;
 
	void multiply(float scalar);
 
	void multiply(float scalar, Mat3* dst) const;
 
	static void multiply(const Mat3& mat, float scalar, Mat3* dst);

 
	void multiply(const Mat3& mat);

	 
	static void multiply(const Mat3& m1, const Mat3& m2, Mat3* dst);
 
	void negate();
 
	Mat3 getNegated() const;
 
	void rotate(const Quaternion& q);
 
	void rotate(const Quaternion& q, Mat3* dst) const;

 
	void rotate(const Vec3& axis, float angle);

 
	void rotate(const Vec3& axis, float angle, Mat3* dst) const;

	/**
	 * Post-multiplies this matrix by the matrix corresponding to the
	 * specified rotation around the x-axis.
	 *
	 * @param angle The angle (in radians).
	 */
	void rotateX(float angle);

 
	void rotateX(float angle, Mat3* dst) const;

 
	void rotateY(float angle);
 
	void rotateY(float angle, Mat3* dst) const;

 
	void rotateZ(float angle);
 
	void rotateZ(float angle, Mat3* dst) const;
 
	void scale(float value);
 
	void scale(float value, Mat3* dst) const;

 
	void scale(float xScale, float yScale, float zScale);

	 
	void scale(float xScale, float yScale, float zScale, Mat3* dst) const;
 
	void scale(const Vec3& s);
 
	void scale(const Vec3& s, Mat3* dst) const;
 
	void set(float m11, float m12, float m13, float m14, float m21, float m22, float m23, float m24,
		float m31, float m32, float m33, float m34, float m41, float m42, float m43, float m44);
 
	void set(const float* mat);
 
	void set(const Mat3& mat);
 
	void setIdentity();
 
	void setZero();
 
	void subtract(const Mat3& mat);
 
	static void subtract(const Mat3& m1, const Mat3& m2, Mat3* dst);

	/**
	 * Transforms the specified point by this matrix.
	 *
	 * The result of the transformation is stored directly into point.
	 *
	 * @param point The point to transform and also a vector to hold the result in.
	 */
	inline void transformPoint(Vec3* point) const { GP_ASSERT(point); transformVector(point->x, point->y, point->z, 1.0f, point); }

	/**
	 * Transforms the specified point by this matrix, and stores
	 * the result in dst.
	 *
	 * @param point The point to transform.
	 * @param dst A vector to store the transformed point in.
	 */
	inline void transformPoint(const Vec3& point, Vec3* dst) const { GP_ASSERT(dst); transformVector(point.x, point.y, point.z, 1.0f, dst); }

	/**
	 * Transforms the specified vector by this matrix by
	 * treating the fourth (w) coordinate as zero.
	 *
	 * The result of the transformation is stored directly into vector.
	 *
	 * @param vector The vector to transform and also a vector to hold the result in.
	 */
	void transformVector(Vec3* vector) const;

	/**
	 * Transforms the specified vector by this matrix by
	 * treating the fourth (w) coordinate as zero, and stores the
	 * result in dst.
	 *
	 * @param vector The vector to transform.
	 * @param dst A vector to store the transformed vector in.
	 */
	void transformVector(const Vec3& vector, Vec3* dst) const;

	/**
	 * Transforms the specified vector by this matrix.
	 *
	 * @param x The vector x-coordinate to transform by.
	 * @param y The vector y-coordinate to transform by.
	 * @param z The vector z-coordinate to transform by.
	 * @param w The vector w-coordinate to transform by.
	 * @param dst A vector to store the transformed point in.
	 */
	void transformVector(float x, float y, float z, float w, Vec3* dst) const;

	/**
	 * Transforms the specified vector by this matrix.
	 *
	 * The result of the transformation is stored directly into vector.
	 *
	 * @param vector The vector to transform.
	 */
	void transformVector(Vec4* vector) const;

	/**
	 * Transforms the specified vector by this matrix.
	 *
	 * @param vector The vector to transform.
	 * @param dst A vector to store the transformed point in.
	 */
	void transformVector(const Vec4& vector, Vec4* dst) const;

	/**
	 * Post-multiplies this matrix by the matrix corresponding to the
	 * specified translation.
	 *
	 * @param x The amount to translate along the x-axis.
	 * @param y The amount to translate along the y-axis.
	 * @param z The amount to translate along the z-axis.
	 */
	void translate(float x, float y, float z);

	/**
	 * Post-multiplies this matrix by the matrix corresponding to the
	 * specified translation and stores the result in dst.
	 *
	 * @param x The amount to translate along the x-axis.
	 * @param y The amount to translate along the y-axis.
	 * @param z The amount to translate along the z-axis.
	 * @param dst A matrix to store the result in.
	 */
	void translate(float x, float y, float z, Mat3* dst) const;

	/**
	 * Post-multiplies this matrix by the matrix corresponding to the
	 * specified translation.
	 *
	 * @param t The translation values along the x, y and z axes.
	 */
	void translate(const Vec3& t);

	/**
	 * Post-multiplies this matrix by the matrix corresponding to the
	 * specified translation and stores the result in dst.
	 *
	 * @param t The translation values along the x, y and z axes.
	 * @param dst A matrix to store the result in.
	 */
	void translate(const Vec3& t, Mat3* dst) const;

	/**
	 * Transposes this matrix.
	 */
	void transpose();

	/**
	 * Get the Transposed matrix.
	 */
	Mat3 getTransposed() const;

	/**
	 * Calculates the sum of this matrix with the given matrix.
	 *
	 * Note: this does not modify this matrix.
	 *
	 * @param mat The matrix to add.
	 * @return The matrix sum.
	 */
	inline Mat3 operator+(const Mat3& mat) const;

	/**
	 * Adds the given matrix to this matrix.
	 *
	 * @param mat The matrix to add.
	 * @return This matrix, after the addition occurs.
	 */
	inline Mat3& operator+=(const Mat3& mat);

	/**
	 * Calculates the difference of this matrix with the given matrix.
	 *
	 * Note: this does not modify this matrix.
	 *
	 * @param mat The matrix to subtract.
	 * @return The matrix difference.
	 */
	inline Mat3 operator-(const Mat3& mat) const;

	/**
	 * Subtracts the given matrix from this matrix.
	 *
	 * @param mat The matrix to subtract.
	 * @return This matrix, after the subtraction occurs.
	 */
	inline Mat3& operator-=(const Mat3& mat);

	/**
	 * Calculates the negation of this matrix.
	 *
	 * Note: this does not modify this matrix.
	 *
	 * @return The negation of this matrix.
	 */
	inline Mat3 operator-() const;

	/**
	 * Calculates the matrix product of this matrix with the given matrix.
	 *
	 * Note: this does not modify this matrix.
	 *
	 * @param mat The matrix to multiply by.
	 * @return The matrix product.
	 */
	inline Mat3 operator*(const Mat3& mat) const;

	/**
	 * Right-multiplies this matrix by the given matrix.
	 *
	 * @param mat The matrix to multiply by.
	 * @return This matrix, after the multiplication occurs.
	 */
	inline Mat3& operator*=(const Mat3& mat);

	/** equals to a matrix full of zeros */
	static const Mat3 ZERO;
	/** equals to the identity matrix */
	static const Mat3 IDENTITY;

private:

	static void createBillboardHelper(const Vec3& objectPosition, const Vec3& cameraPosition,
		const Vec3& cameraUpVector, const Vec3* cameraForwardVector,
		Mat3* dst);
};

/**
 * Transforms the given vector by the given matrix.
 *
 * Note: this treats the given vector as a vector and not as a point.
 *
 * @param v The vector to transform.
 * @param m The matrix to transform by.
 * @return This vector, after the transformation occurs.
 */
inline Vec3& operator*=(Vec3& v, const Mat3& m);

/**
 * Transforms the given vector by the given matrix.
 *
 * Note: this treats the given vector as a vector and not as a point.
 *
 * @param m The matrix to transform by.
 * @param v The vector to transform.
 * @return The resulting transformed vector.
 */
inline Vec3 operator*(const Mat3& m, const Vec3& v);

/**
 * Transforms the given vector by the given matrix.
 *
 * Note: this treats the given vector as a vector and not as a point.
 *
 * @param v The vector to transform.
 * @param m The matrix to transform by.
 * @return This vector, after the transformation occurs.
 */
inline Vec4& operator*=(Vec4& v, const Mat3& m);

/**
 * Transforms the given vector by the given matrix.
 *
 * Note: this treats the given vector as a vector and not as a point.
 *
 * @param m The matrix to transform by.
 * @param v The vector to transform.
 * @return The resulting transformed vector.
 */
inline Vec4 operator*(const Mat3& m, const Vec4& v);

OG_END

#endif // 
