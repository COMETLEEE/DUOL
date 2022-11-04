#pragma once
#include <cassert>
#include <cstddef>
#include <cstring>
#include <functional>

#include <DirectXMath.h>

namespace DUOLMath
{
    using namespace DirectX;

    struct Vector2;
    struct Vector4;
    struct Matrix;
    struct Quaternion;

    //------------------------------------------------------------------------------
    // Math Helper
    class MathHelper
    {
    public:
        static float DegreeToRadian(float degree);

        static float RadianToDegree(float radian);
    };

    //------------------------------------------------------------------------------
	// 2D vector
    struct Vector2 : public XMFLOAT2
    {
        Vector2() noexcept : XMFLOAT2(0.f, 0.f) {}
        constexpr explicit Vector2(float ix) noexcept : XMFLOAT2(ix, ix) {}
        constexpr Vector2(float ix, float iy) noexcept : XMFLOAT2(ix, iy) {}
        explicit Vector2(_In_reads_(2) const float* pArray) noexcept : XMFLOAT2(pArray) {}
        Vector2(FXMVECTOR V) noexcept { XMStoreFloat2(this, V); }
        Vector2(const XMFLOAT2& V) noexcept { this->x = V.x; this->y = V.y; }
        explicit Vector2(const XMVECTORF32& F) noexcept { this->x = F.f[0]; this->y = F.f[1]; }

        Vector2(const Vector2&) = default;
        Vector2& operator=(const Vector2&) = default;

        Vector2(Vector2&&) = default;
        Vector2& operator=(Vector2&&) = default;

        operator XMVECTOR() const noexcept { return XMLoadFloat2(this); }

        // Comparison operators
        bool operator == (const Vector2& V) const noexcept;
        bool operator != (const Vector2& V) const noexcept;

        // Assignment operators
        Vector2& operator= (const XMVECTORF32& F) noexcept { x = F.f[0]; y = F.f[1]; return *this; }
        Vector2& operator+= (const Vector2& V) noexcept;
        Vector2& operator-= (const Vector2& V) noexcept;
        Vector2& operator*= (const Vector2& V) noexcept;
        Vector2& operator*= (float S) noexcept;
        Vector2& operator/= (float S) noexcept;

        // Unary operators
        Vector2 operator+ () const noexcept { return *this; }
        Vector2 operator- () const noexcept { return Vector2(-x, -y); }

        // Vector operations
        bool InBounds(const Vector2& Bounds) const noexcept;

        float Length() const noexcept;
        float LengthSquared() const noexcept;

        float Dot(const Vector2& V) const noexcept;
        void Cross(const Vector2& V, Vector2& result) const noexcept;
        Vector2 Cross(const Vector2& V) const noexcept;

        void Normalize() noexcept;
        void Normalize(Vector2& result) const noexcept;

        void Clamp(const Vector2& vmin, const Vector2& vmax) noexcept;
        void Clamp(const Vector2& vmin, const Vector2& vmax, Vector2& result) const noexcept;

        // Static functions
        static float Distance(const Vector2& v1, const Vector2& v2) noexcept;
        static float DistanceSquared(const Vector2& v1, const Vector2& v2) noexcept;

        static void Min(const Vector2& v1, const Vector2& v2, Vector2& result) noexcept;
        static Vector2 Min(const Vector2& v1, const Vector2& v2) noexcept;

        static void Max(const Vector2& v1, const Vector2& v2, Vector2& result) noexcept;
        static Vector2 Max(const Vector2& v1, const Vector2& v2) noexcept;

        static void Lerp(const Vector2& v1, const Vector2& v2, float t, Vector2& result) noexcept;
        static Vector2 Lerp(const Vector2& v1, const Vector2& v2, float t) noexcept;

        static void SmoothStep(const Vector2& v1, const Vector2& v2, float t, Vector2& result) noexcept;
        static Vector2 SmoothStep(const Vector2& v1, const Vector2& v2, float t) noexcept;

        static void Barycentric(const Vector2& v1, const Vector2& v2, const Vector2& v3, float f, float g, Vector2& result) noexcept;
        static Vector2 Barycentric(const Vector2& v1, const Vector2& v2, const Vector2& v3, float f, float g) noexcept;

        static void CatmullRom(const Vector2& v1, const Vector2& v2, const Vector2& v3, const Vector2& v4, float t, Vector2& result) noexcept;
        static Vector2 CatmullRom(const Vector2& v1, const Vector2& v2, const Vector2& v3, const Vector2& v4, float t) noexcept;

        static void Hermite(const Vector2& v1, const Vector2& t1, const Vector2& v2, const Vector2& t2, float t, Vector2& result) noexcept;
        static Vector2 Hermite(const Vector2& v1, const Vector2& t1, const Vector2& v2, const Vector2& t2, float t) noexcept;

        static void Reflect(const Vector2& ivec, const Vector2& nvec, Vector2& result) noexcept;
        static Vector2 Reflect(const Vector2& ivec, const Vector2& nvec) noexcept;

        static void Refract(const Vector2& ivec, const Vector2& nvec, float refractionIndex, Vector2& result) noexcept;
        static Vector2 Refract(const Vector2& ivec, const Vector2& nvec, float refractionIndex) noexcept;

        static void Transform(const Vector2& v, const Quaternion& quat, Vector2& result) noexcept;
        static Vector2 Transform(const Vector2& v, const Quaternion& quat) noexcept;

        static void Transform(const Vector2& v, const Matrix& m, Vector2& result) noexcept;
        static Vector2 Transform(const Vector2& v, const Matrix& m) noexcept;
        static void Transform(_In_reads_(count) const Vector2* varray, size_t count, const Matrix& m, _Out_writes_(count) Vector2* resultArray) noexcept;

        static void Transform(const Vector2& v, const Matrix& m, Vector4& result) noexcept;
        static void Transform(_In_reads_(count) const Vector2* varray, size_t count, const Matrix& m, _Out_writes_(count) Vector4* resultArray) noexcept;

        static void TransformNormal(const Vector2& v, const Matrix& m, Vector2& result) noexcept;
        static Vector2 TransformNormal(const Vector2& v, const Matrix& m) noexcept;
        static void TransformNormal(_In_reads_(count) const Vector2* varray, size_t count, const Matrix& m, _Out_writes_(count) Vector2* resultArray) noexcept;

        static Vector2 DegreeToRadian(const Vector2& degreeVec) noexcept;
        static Vector2 RadianToDegree(const Vector2& radianVec) noexcept;

        // Constants
        static const Vector2 Zero;
        static const Vector2 One;
        static const Vector2 UnitX;
        static const Vector2 UnitY;
    };

    //------------------------------------------------------------------------------
	// 3D vector
    struct Vector3 : public XMFLOAT3
    {
        Vector3() noexcept : XMFLOAT3(0.f, 0.f, 0.f) {}
        constexpr explicit Vector3(float ix) noexcept : XMFLOAT3(ix, ix, ix) {}
        constexpr Vector3(float ix, float iy, float iz) noexcept : XMFLOAT3(ix, iy, iz) {}
        explicit Vector3(_In_reads_(3) const float* pArray) noexcept : XMFLOAT3(pArray) {}
        Vector3(FXMVECTOR V) noexcept { XMStoreFloat3(this, V); }
        Vector3(const XMFLOAT3& V) noexcept { this->x = V.x; this->y = V.y; this->z = V.z; }
        explicit Vector3(const XMVECTORF32& F) noexcept { this->x = F.f[0]; this->y = F.f[1]; this->z = F.f[2]; }

        Vector3(const Vector3&) = default;
        Vector3& operator=(const Vector3&) = default;

        Vector3(Vector3&&) = default;
        Vector3& operator=(Vector3&&) = default;

        operator XMVECTOR() const noexcept { return XMLoadFloat3(this); }

        // Comparison operators
        bool operator == (const Vector3& V) const noexcept;
        bool operator != (const Vector3& V) const noexcept;

        // Assignment operators
        Vector3& operator= (const XMVECTORF32& F) noexcept { x = F.f[0]; y = F.f[1]; z = F.f[2]; return *this; }
        Vector3& operator+= (const Vector3& V) noexcept;
        Vector3& operator-= (const Vector3& V) noexcept;
        Vector3& operator*= (const Vector3& V) noexcept;
        Vector3& operator*= (float S) noexcept;
        Vector3& operator/= (float S) noexcept;

        // Unary operators
        Vector3 operator+ () const noexcept { return *this; }
        Vector3 operator- () const noexcept;

        // Vector operations
        bool InBounds(const Vector3& Bounds) const noexcept;

        float Length() const noexcept;
        float LengthSquared() const noexcept;

        float Dot(const Vector3& V) const noexcept;
        void Cross(const Vector3& V, Vector3& result) const noexcept;
        Vector3 Cross(const Vector3& V) const noexcept;

        void Normalize() noexcept;
        void Normalize(Vector3& result) const noexcept;

        void Clamp(const Vector3& vmin, const Vector3& vmax) noexcept;
        void Clamp(const Vector3& vmin, const Vector3& vmax, Vector3& result) const noexcept;

        // Static functions
        static float Distance(const Vector3& v1, const Vector3& v2) noexcept;
        static float DistanceSquared(const Vector3& v1, const Vector3& v2) noexcept;

        static void Min(const Vector3& v1, const Vector3& v2, Vector3& result) noexcept;
        static Vector3 Min(const Vector3& v1, const Vector3& v2) noexcept;

        static void Max(const Vector3& v1, const Vector3& v2, Vector3& result) noexcept;
        static Vector3 Max(const Vector3& v1, const Vector3& v2) noexcept;

        static void Lerp(const Vector3& v1, const Vector3& v2, float t, Vector3& result) noexcept;
        static Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t) noexcept;

        static void SmoothStep(const Vector3& v1, const Vector3& v2, float t, Vector3& result) noexcept;
        static Vector3 SmoothStep(const Vector3& v1, const Vector3& v2, float t) noexcept;

        static void Barycentric(const Vector3& v1, const Vector3& v2, const Vector3& v3, float f, float g, Vector3& result) noexcept;
        static Vector3 Barycentric(const Vector3& v1, const Vector3& v2, const Vector3& v3, float f, float g) noexcept;

        static void CatmullRom(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& v4, float t, Vector3& result) noexcept;
        static Vector3 CatmullRom(const Vector3& v1, const Vector3& v2, const Vector3& v3, const Vector3& v4, float t) noexcept;

        static void Hermite(const Vector3& v1, const Vector3& t1, const Vector3& v2, const Vector3& t2, float t, Vector3& result) noexcept;
        static Vector3 Hermite(const Vector3& v1, const Vector3& t1, const Vector3& v2, const Vector3& t2, float t) noexcept;

        static void Reflect(const Vector3& ivec, const Vector3& nvec, Vector3& result) noexcept;
        static Vector3 Reflect(const Vector3& ivec, const Vector3& nvec) noexcept;

        static void Refract(const Vector3& ivec, const Vector3& nvec, float refractionIndex, Vector3& result) noexcept;
        static Vector3 Refract(const Vector3& ivec, const Vector3& nvec, float refractionIndex) noexcept;

        static void Transform(const Vector3& v, const Quaternion& quat, Vector3& result) noexcept;
        static Vector3 Transform(const Vector3& v, const Quaternion& quat) noexcept;

        static void Transform(const Vector3& v, const Matrix& m, Vector3& result) noexcept;
        static Vector3 Transform(const Vector3& v, const Matrix& m) noexcept;
        static void Transform(_In_reads_(count) const Vector3* varray, size_t count, const Matrix& m, _Out_writes_(count) Vector3* resultArray) noexcept;

        static void Transform(const Vector3& v, const Matrix& m, Vector4& result) noexcept;
        static void Transform(_In_reads_(count) const Vector3* varray, size_t count, const Matrix& m, _Out_writes_(count) Vector4* resultArray) noexcept;

        static void TransformNormal(const Vector3& v, const Matrix& m, Vector3& result) noexcept;
        static Vector3 TransformNormal(const Vector3& v, const Matrix& m) noexcept;
        static void TransformNormal(_In_reads_(count) const Vector3* varray, size_t count, const Matrix& m, _Out_writes_(count) Vector3* resultArray) noexcept;

        static Vector3 DegreeToRadian(const Vector3& degreeVec) noexcept;
        static Vector3 RadianToDegree(const Vector3& radianVec) noexcept;

        // Constants
        static const Vector3 Zero;
        static const Vector3 One;
        static const Vector3 UnitX;
        static const Vector3 UnitY;
        static const Vector3 UnitZ;
        static const Vector3 Up;
        static const Vector3 Down;
        static const Vector3 Right;
        static const Vector3 Left;
        static const Vector3 Forward;
        static const Vector3 Backward;
    };

    // Binary operators
    Vector3 operator+ (const Vector3& V1, const Vector3& V2) noexcept;
    Vector3 operator- (const Vector3& V1, const Vector3& V2) noexcept;
    Vector3 operator* (const Vector3& V1, const Vector3& V2) noexcept;
    Vector3 operator* (const Vector3& V, float S) noexcept;
    Vector3 operator/ (const Vector3& V1, const Vector3& V2) noexcept;
    Vector3 operator/ (const Vector3& V, float S) noexcept;
    Vector3 operator* (float S, const Vector3& V) noexcept;

    //------------------------------------------------------------------------------
	// 4D vector
    struct Vector4 : public XMFLOAT4
    {
        Vector4() noexcept : XMFLOAT4(0.f, 0.f, 0.f, 0.f) {}
        constexpr explicit Vector4(float ix) noexcept : XMFLOAT4(ix, ix, ix, ix) {}
        constexpr Vector4(float ix, float iy, float iz, float iw) noexcept : XMFLOAT4(ix, iy, iz, iw) {}
        explicit Vector4(_In_reads_(4) const float* pArray) noexcept : XMFLOAT4(pArray) {}
        Vector4(FXMVECTOR V) noexcept { XMStoreFloat4(this, V); }
        Vector4(const XMFLOAT4& V) noexcept { this->x = V.x; this->y = V.y; this->z = V.z; this->w = V.w; }
        explicit Vector4(const XMVECTORF32& F) noexcept { this->x = F.f[0]; this->y = F.f[1]; this->z = F.f[2]; this->w = F.f[3]; }

        Vector4(const Vector4&) = default;
        Vector4& operator=(const Vector4&) = default;

        Vector4(Vector4&&) = default;
        Vector4& operator=(Vector4&&) = default;

        operator XMVECTOR() const  noexcept { return XMLoadFloat4(this); }

        // Comparison operators
        bool operator == (const Vector4& V) const noexcept;
        bool operator != (const Vector4& V) const noexcept;

        // Assignment operators
        Vector4& operator= (const XMVECTORF32& F) noexcept { x = F.f[0]; y = F.f[1]; z = F.f[2]; w = F.f[3]; return *this; }
        Vector4& operator+= (const Vector4& V) noexcept;
        Vector4& operator-= (const Vector4& V) noexcept;
        Vector4& operator*= (const Vector4& V) noexcept;
        Vector4& operator*= (float S) noexcept;
        Vector4& operator/= (float S) noexcept;
        Vector4& operator=(const Vector3& V) noexcept { x = V.x; y = V.y; z = V.z; w = 0.f; return *this; }

        // Unary operators
        Vector4 operator+ () const noexcept { return *this; }
        Vector4 operator- () const noexcept;

        // Vector operations
        bool InBounds(const Vector4& Bounds) const noexcept;

        float Length() const noexcept;
        float LengthSquared() const noexcept;

        float Dot(const Vector4& V) const noexcept;
        void Cross(const Vector4& v1, const Vector4& v2, Vector4& result) const noexcept;
        Vector4 Cross(const Vector4& v1, const Vector4& v2) const noexcept;

        void Normalize() noexcept;
        void Normalize(Vector4& result) const noexcept;

        void Clamp(const Vector4& vmin, const Vector4& vmax) noexcept;
        void Clamp(const Vector4& vmin, const Vector4& vmax, Vector4& result) const noexcept;

        // Static functions
        static float Distance(const Vector4& v1, const Vector4& v2) noexcept;
        static float DistanceSquared(const Vector4& v1, const Vector4& v2) noexcept;

        static void Min(const Vector4& v1, const Vector4& v2, Vector4& result) noexcept;
        static Vector4 Min(const Vector4& v1, const Vector4& v2) noexcept;

        static void Max(const Vector4& v1, const Vector4& v2, Vector4& result) noexcept;
        static Vector4 Max(const Vector4& v1, const Vector4& v2) noexcept;

        static void Lerp(const Vector4& v1, const Vector4& v2, float t, Vector4& result) noexcept;
        static Vector4 Lerp(const Vector4& v1, const Vector4& v2, float t) noexcept;

        static void SmoothStep(const Vector4& v1, const Vector4& v2, float t, Vector4& result) noexcept;
        static Vector4 SmoothStep(const Vector4& v1, const Vector4& v2, float t) noexcept;

        static void Barycentric(const Vector4& v1, const Vector4& v2, const Vector4& v3, float f, float g, Vector4& result) noexcept;
        static Vector4 Barycentric(const Vector4& v1, const Vector4& v2, const Vector4& v3, float f, float g) noexcept;

        static void CatmullRom(const Vector4& v1, const Vector4& v2, const Vector4& v3, const Vector4& v4, float t, Vector4& result) noexcept;
        static Vector4 CatmullRom(const Vector4& v1, const Vector4& v2, const Vector4& v3, const Vector4& v4, float t) noexcept;

        static void Hermite(const Vector4& v1, const Vector4& t1, const Vector4& v2, const Vector4& t2, float t, Vector4& result) noexcept;
        static Vector4 Hermite(const Vector4& v1, const Vector4& t1, const Vector4& v2, const Vector4& t2, float t) noexcept;

        static void Reflect(const Vector4& ivec, const Vector4& nvec, Vector4& result) noexcept;
        static Vector4 Reflect(const Vector4& ivec, const Vector4& nvec) noexcept;

        static void Refract(const Vector4& ivec, const Vector4& nvec, float refractionIndex, Vector4& result) noexcept;
        static Vector4 Refract(const Vector4& ivec, const Vector4& nvec, float refractionIndex) noexcept;

        static void Transform(const Vector2& v, const Quaternion& quat, Vector4& result) noexcept;
        static Vector4 Transform(const Vector2& v, const Quaternion& quat) noexcept;

        static void Transform(const Vector3& v, const Quaternion& quat, Vector4& result) noexcept;
        static Vector4 Transform(const Vector3& v, const Quaternion& quat) noexcept;

        static void Transform(const Vector4& v, const Quaternion& quat, Vector4& result) noexcept;
        static Vector4 Transform(const Vector4& v, const Quaternion& quat) noexcept;

        static void Transform(const Vector4& v, const Matrix& m, Vector4& result) noexcept;
        static Vector4 Transform(const Vector4& v, const Matrix& m) noexcept;
        static void Transform(_In_reads_(count) const Vector4* varray, size_t count, const Matrix& m, _Out_writes_(count) Vector4* resultArray) noexcept;

        static Vector4 DegreeToRadian(const Vector4& degreeVec) noexcept;
        static Vector4 RadianToDegree(const Vector4& radianVec) noexcept;

        // Constants
        static const Vector4 Zero;
        static const Vector4 One;
        static const Vector4 UnitX;
        static const Vector4 UnitY;
        static const Vector4 UnitZ;
        static const Vector4 UnitW;
    };

    // Binary operators
    Vector4 operator+ (const Vector4& V1, const Vector4& V2) noexcept;
    Vector4 operator- (const Vector4& V1, const Vector4& V2) noexcept;
    Vector4 operator* (const Vector4& V1, const Vector4& V2) noexcept;
    Vector4 operator* (const Vector4& V, float S) noexcept;
    Vector4 operator/ (const Vector4& V1, const Vector4& V2) noexcept;
    Vector4 operator/ (const Vector4& V, float S) noexcept;
    Vector4 operator* (float S, const Vector4& V) noexcept;

    //------------------------------------------------------------------------------
	// 4x4 Matrix (Assumes Left - Handed Coordinates)
    struct Matrix : public XMFLOAT4X4
    {
        Matrix() noexcept
            : XMFLOAT4X4(1.f, 0, 0, 0,
                0, 1.f, 0, 0,
                0, 0, 1.f, 0,
                0, 0, 0, 1.f) {}
        constexpr Matrix(float m00, float m01, float m02, float m03,
            float m10, float m11, float m12, float m13,
            float m20, float m21, float m22, float m23,
            float m30, float m31, float m32, float m33) noexcept
            : XMFLOAT4X4(m00, m01, m02, m03,
                m10, m11, m12, m13,
                m20, m21, m22, m23,
                m30, m31, m32, m33) {}
        explicit Matrix(const Vector3& r0, const Vector3& r1, const Vector3& r2) noexcept
            : XMFLOAT4X4(r0.x, r0.y, r0.z, 0,
                r1.x, r1.y, r1.z, 0,
                r2.x, r2.y, r2.z, 0,
                0, 0, 0, 1.f) {}
        explicit Matrix(const Vector4& r0, const Vector4& r1, const Vector4& r2, const Vector4& r3) noexcept
            : XMFLOAT4X4(r0.x, r0.y, r0.z, r0.w,
                r1.x, r1.y, r1.z, r1.w,
                r2.x, r2.y, r2.z, r2.w,
                r3.x, r3.y, r3.z, r3.w) {}
        Matrix(const XMFLOAT4X4& M) noexcept { memcpy(this, &M, sizeof(XMFLOAT4X4)); }
        Matrix(const XMFLOAT3X3& M) noexcept;
        Matrix(const XMFLOAT4X3& M) noexcept;

        explicit Matrix(_In_reads_(16) const float* pArray) noexcept : XMFLOAT4X4(pArray) {}
        Matrix(CXMMATRIX M) noexcept { XMStoreFloat4x4(this, M); }

        Matrix(const Matrix&) = default;
        Matrix& operator=(const Matrix&) = default;

        Matrix(Matrix&&) = default;
        Matrix& operator=(Matrix&&) = default;

        operator XMMATRIX() const noexcept { return XMLoadFloat4x4(this); }

        // Comparison operators
        bool operator == (const Matrix& M) const noexcept;
        bool operator != (const Matrix& M) const noexcept;

        // Assignment operators
        Matrix& operator= (const XMFLOAT3X3& M) noexcept;
        Matrix& operator= (const XMFLOAT4X3& M) noexcept;
        Matrix& operator+= (const Matrix& M) noexcept;
        Matrix& operator-= (const Matrix& M) noexcept;
        Matrix& operator*= (const Matrix& M) noexcept;
        Matrix& operator*= (float S) noexcept;
        Matrix& operator/= (float S) noexcept;

        Matrix& operator/= (const Matrix& M) noexcept;

        // Element-wise divide
    	// Unary operators
        Matrix operator+ () const noexcept { return *this; }
        Matrix operator- () const noexcept;

        // Properties
        Vector3 Up() const noexcept { return Vector3(_21, _22, _23); }
        void Up(const Vector3& v) noexcept { _21 = v.x; _22 = v.y; _23 = v.z; }

        Vector3 Down() const  noexcept { return Vector3(-_21, -_22, -_23); }
        void Down(const Vector3& v) noexcept { _21 = -v.x; _22 = -v.y; _23 = -v.z; }

        Vector3 Right() const noexcept { return Vector3(_11, _12, _13); }
        void Right(const Vector3& v) noexcept { _11 = v.x; _12 = v.y; _13 = v.z; }

        Vector3 Left() const noexcept { return Vector3(-_11, -_12, -_13); }
        void Left(const Vector3& v) noexcept { _11 = -v.x; _12 = -v.y; _13 = -v.z; }

        Vector3 Forward() const noexcept { return Vector3(-_31, -_32, -_33); }
        void Forward(const Vector3& v) noexcept { _31 = -v.x; _32 = -v.y; _33 = -v.z; }

        Vector3 Backward() const noexcept { return Vector3(_31, _32, _33); }
        void Backward(const Vector3& v) noexcept { _31 = v.x; _32 = v.y; _33 = v.z; }

        Vector3 Translation() const  noexcept { return Vector3(_41, _42, _43); }
        void Translation(const Vector3& v) noexcept { _41 = v.x; _42 = v.y; _43 = v.z; }

        // Matrix operations
        bool Decompose(Vector3& scale, Quaternion& rotation, Vector3& translation) noexcept;

        Matrix Transpose() const noexcept;
        void Transpose(Matrix& result) const noexcept;

        Matrix Invert() const noexcept;
        void Invert(Matrix& result) const noexcept;

        float Determinant() const noexcept;

        // Static functions
        static Matrix CreateBillboard(
            const Vector3& object, const Vector3& cameraPosition, const Vector3& cameraUp, _In_opt_ const Vector3* cameraForward = nullptr) noexcept;

        static Matrix CreateConstrainedBillboard(
            const Vector3& object, const Vector3& cameraPosition, const Vector3& rotateAxis,
            _In_opt_ const Vector3* cameraForward = nullptr, _In_opt_ const Vector3* objectForward = nullptr) noexcept;

        static Matrix CreateTranslation(const Vector3& position) noexcept;
        static Matrix CreateTranslation(float x, float y, float z) noexcept;

        static Matrix CreateScale(const Vector3& scales) noexcept;
        static Matrix CreateScale(float xs, float ys, float zs) noexcept;
        static Matrix CreateScale(float scale) noexcept;

        static Matrix CreateRotationX(float radians) noexcept;
        static Matrix CreateRotationY(float radians) noexcept;
        static Matrix CreateRotationZ(float radians) noexcept;

        static Matrix CreateFromAxisAngle(const Vector3& axis, float angle) noexcept;

        static Matrix CreatePerspectiveFieldOfView(float fov, float aspectRatio, float nearPlane, float farPlane) noexcept;
        static Matrix CreatePerspective(float width, float height, float nearPlane, float farPlane) noexcept;
        static Matrix CreatePerspectiveOffCenter(float left, float right, float bottom, float top, float nearPlane, float farPlane) noexcept;
        static Matrix CreateOrthographic(float width, float height, float zNearPlane, float zFarPlane) noexcept;
        static Matrix CreateOrthographicOffCenter(float left, float right, float bottom, float top, float zNearPlane, float zFarPlane) noexcept;

        static Matrix CreateLookAt(const Vector3& position, const Vector3& target, const Vector3& up) noexcept;
        static Matrix CreateWorld(const Vector3& position, const Vector3& forward, const Vector3& up) noexcept;

        static Matrix CreateFromQuaternion(const Quaternion& quat) noexcept;

        /**
         * \brief 변환 순서로 Z, X, Y 순서로 회전하는 행렬을 반환합니다.
         * \param yaw Y축 기준으로의 회전각 (라디안)
         * \param pitch X축 기준으로의 회전각 (라디안)
         * \param roll Z축 기준으로의 회전각 (라디안)
         * \return Transform Matrix
         */
        static Matrix CreateFromYawPitchRoll(float yaw, float pitch, float roll) noexcept;


        /**
         * \brief Look, Right, Up 벡터를 기저로하는 회전 변환 행렬을 반환합니다.
         * \param look Look Vector
         * \param right Right Vector
         * \param up Up Vector
         * \return Transform Matrix
         */
        static Matrix CreateFromLookRightUp(const Vector3& look, const Vector3& right, const Vector3& up) noexcept;

        static void Lerp(const Matrix& M1, const Matrix& M2, float t, Matrix& result) noexcept;
        static Matrix Lerp(const Matrix& M1, const Matrix& M2, float t) noexcept;

        static void Transform(const Matrix& M, const Quaternion& rotation, Matrix& result) noexcept;
        static Matrix Transform(const Matrix& M, const Quaternion& rotation) noexcept;

        // Constants
        static const Matrix Identity;
    };

    // Binary operators
    Matrix operator+ (const Matrix& M1, const Matrix& M2) noexcept;
    Matrix operator- (const Matrix& M1, const Matrix& M2) noexcept;
    Matrix operator* (const Matrix& M1, const Matrix& M2) noexcept;
    Matrix operator* (const Matrix& M, float S) noexcept;
    Matrix operator/ (const Matrix& M, float S) noexcept;
    Matrix operator/ (const Matrix& M1, const Matrix& M2) noexcept;
    // Element-wise divide
    Matrix operator* (float S, const Matrix& M) noexcept;

    //------------------------------------------------------------------------------
	// Quaternion
    struct Quaternion : public XMFLOAT4
    {
        Quaternion() noexcept : XMFLOAT4(0, 0, 0, 1.f) {}
        constexpr Quaternion(float ix, float iy, float iz, float iw) noexcept : XMFLOAT4(ix, iy, iz, iw) {}
        Quaternion(const Vector3& v, float scalar) noexcept : XMFLOAT4(v.x, v.y, v.z, scalar) {}
        explicit Quaternion(const Vector4& v) noexcept : XMFLOAT4(v.x, v.y, v.z, v.w) {}
        explicit Quaternion(_In_reads_(4) const float* pArray) noexcept : XMFLOAT4(pArray) {}
        Quaternion(FXMVECTOR V) noexcept { XMStoreFloat4(this, V); }
        Quaternion(const XMFLOAT4& q) noexcept { this->x = q.x; this->y = q.y; this->z = q.z; this->w = q.w; }
        explicit Quaternion(const XMVECTORF32& F) noexcept { this->x = F.f[0]; this->y = F.f[1]; this->z = F.f[2]; this->w = F.f[3]; }

        Quaternion(const Quaternion&) = default;
        Quaternion& operator=(const Quaternion&) = default;

        Quaternion(Quaternion&&) = default;
        Quaternion& operator=(Quaternion&&) = default;

        operator XMVECTOR() const noexcept { return XMLoadFloat4(this); }

        // Comparison operators
        bool operator == (const Quaternion& q) const noexcept;
        bool operator != (const Quaternion& q) const noexcept;

        // Assignment operators
        Quaternion& operator= (const XMVECTORF32& F) noexcept { x = F.f[0]; y = F.f[1]; z = F.f[2]; w = F.f[3]; return *this; }
        Quaternion& operator+= (const Quaternion& q) noexcept;
        Quaternion& operator-= (const Quaternion& q) noexcept;
        Quaternion& operator*= (const Quaternion& q) noexcept;
        Quaternion& operator*= (float S) noexcept;
        Quaternion& operator/= (const Quaternion& q) noexcept;

        // Unary operators
        Quaternion operator+ () const  noexcept { return *this; }
        Quaternion operator- () const noexcept;

        // Quaternion operations
        float Length() const noexcept;
        float LengthSquared() const noexcept;

        void Normalize() noexcept;
        void Normalize(Quaternion& result) const noexcept;

        void Conjugate() noexcept;
        void Conjugate(Quaternion& result) const noexcept;

        void Inverse(Quaternion& result) const noexcept;

        float Dot(const Quaternion& Q) const noexcept;

        // Static functions
        static Quaternion CreateFromAxisAngle(const Vector3& axis, float angle) noexcept;
        static Quaternion CreateFromYawPitchRoll(float yaw, float pitch, float roll) noexcept;
        static Quaternion CreateFromRotationMatrix(const Matrix& M) noexcept;

        /**
         * \brief Convert quaternion to euler angles. (using Z, X, Y rotation order)
         * \param quat Quaternion to extract euler angles.
         * \return euler angle that using Z X Y orders.
         */
        static Vector3 ConvertQuaternionToEuler(const Quaternion& quat) noexcept;

        static void Lerp(const Quaternion& q1, const Quaternion& q2, float t, Quaternion& result) noexcept;
        static Quaternion Lerp(const Quaternion& q1, const Quaternion& q2, float t) noexcept;

        static void Slerp(const Quaternion& q1, const Quaternion& q2, float t, Quaternion& result) noexcept;
        static Quaternion Slerp(const Quaternion& q1, const Quaternion& q2, float t) noexcept;

        static void Concatenate(const Quaternion& q1, const Quaternion& q2, Quaternion& result) noexcept;
        static Quaternion Concatenate(const Quaternion& q1, const Quaternion& q2) noexcept;

        // Constants
        static const Quaternion Identity;
    };

    // Binary operators
    Quaternion operator+ (const Quaternion& Q1, const Quaternion& Q2) noexcept;
    Quaternion operator- (const Quaternion& Q1, const Quaternion& Q2) noexcept;
    Quaternion operator* (const Quaternion& Q1, const Quaternion& Q2) noexcept;
    Quaternion operator* (const Quaternion& Q, float S) noexcept;
    Quaternion operator/ (const Quaternion& Q1, const Quaternion& Q2) noexcept;
    Quaternion operator* (float S, const Quaternion& Q) noexcept;

    /**
     * \brief 3D Ray (For PhysX Query)
     */
    class Ray
    {
    public:
        Vector3 origin;

        Vector3 direction;

    	float maxDistance;
    };

    /**
     * \brief 3D Plane (For PhysX Query)
     */
    class Plane
    {
    public:
	    /**
	     * \brief 평면의 법선 벡터
	     */
	    Vector3 normal;

	    /**
	     * \brief 원점으로부터의 거리
	     */
	    float distance;
    };

    /**
     * \brief 3D Box
     */
	class Box
    {
	public:
	    /**
	     * \brief X축 (Right) 방향 길이 절반
	     */
	    float halfX;

	    /**
	     * \brief Y축 (Up) 방향 길이 절반
	     */
	    float halfY;

	    /**
	     * \brief Z축 (Look) 방향 길이 절반
	     */
	    float halfZ;

        /**
         * \brief 박스의 Up Vector
         */
        Vector3 shapeUp;
    };

	/**
	 * \brief 3D Sphere
	 */
	class Sphere
    {
	public:
	    /**
	     * \brief 구의 중심
	     */
	    Vector3 origin;

	    /**
	     * \brief 구의 반지름
	     */
	    float radius;
    };

	/**
	 * \brief 3D Capsule
	 */
	class Capsule
    {
	public:
	    /**
	     * \brief 캡슐의 중앙
	     */
	    Vector3 origin;

	    /**
	     * \brief 캡슐의 Up Vector
	     */
	    Vector3 shapeUp;

	    /**
	     * \brief 캡슐 상, 하단 구의 반지름
	     */
	    float radius;

	    /**
	     * \brief 캡슐 원기둥의 높이의 절반
	     */
	    float halfHeight;
    };

	#include "DUOLMath.inl"
}

//------------------------------------------------------------------------------
// Support for DUOLMath and Standard C++ Library containers
namespace std
{
    template<> struct less<DUOLMath::Vector2>
    {
        bool operator()(const DUOLMath::Vector2& V1, const DUOLMath::Vector2& V2) const noexcept
        {
            return ((V1.x < V2.x) || ((V1.x == V2.x) && (V1.y < V2.y)));
        }
    };

    template<> struct less<DUOLMath::Vector3>
    {
        bool operator()(const DUOLMath::Vector3& V1, const DUOLMath::Vector3& V2) const noexcept
        {
            return ((V1.x < V2.x)
                || ((V1.x == V2.x) && (V1.y < V2.y))
                || ((V1.x == V2.x) && (V1.y == V2.y) && (V1.z < V2.z)));
        }
    };

    template<> struct less<DUOLMath::Vector4>
    {
        bool operator()(const DUOLMath::Vector4& V1, const DUOLMath::Vector4& V2) const noexcept
        {
            return ((V1.x < V2.x)
                || ((V1.x == V2.x) && (V1.y < V2.y))
                || ((V1.x == V2.x) && (V1.y == V2.y) && (V1.z < V2.z))
                || ((V1.x == V2.x) && (V1.y == V2.y) && (V1.z == V2.z) && (V1.w < V2.w)));
        }
    };

    template<> struct less<DUOLMath::Matrix>
    {
        bool operator()(const DUOLMath::Matrix& M1, const DUOLMath::Matrix& M2) const noexcept
        {
            if (M1._11 != M2._11) return M1._11 < M2._11;
            if (M1._12 != M2._12) return M1._12 < M2._12;
            if (M1._13 != M2._13) return M1._13 < M2._13;
            if (M1._14 != M2._14) return M1._14 < M2._14;
            if (M1._21 != M2._21) return M1._21 < M2._21;
            if (M1._22 != M2._22) return M1._22 < M2._22;
            if (M1._23 != M2._23) return M1._23 < M2._23;
            if (M1._24 != M2._24) return M1._24 < M2._24;
            if (M1._31 != M2._31) return M1._31 < M2._31;
            if (M1._32 != M2._32) return M1._32 < M2._32;
            if (M1._33 != M2._33) return M1._33 < M2._33;
            if (M1._34 != M2._34) return M1._34 < M2._34;
            if (M1._41 != M2._41) return M1._41 < M2._41;
            if (M1._42 != M2._42) return M1._42 < M2._42;
            if (M1._43 != M2._43) return M1._43 < M2._43;
            if (M1._44 != M2._44) return M1._44 < M2._44;

            return false;
        }
    };

    template<> struct less<DUOLMath::Quaternion>
    {
        bool operator()(const DUOLMath::Quaternion& Q1, const DUOLMath::Quaternion& Q2) const noexcept
        {
            return ((Q1.x < Q2.x)
                || ((Q1.x == Q2.x) && (Q1.y < Q2.y))
                || ((Q1.x == Q2.x) && (Q1.y == Q2.y) && (Q1.z < Q2.z))
                || ((Q1.x == Q2.x) && (Q1.y == Q2.y) && (Q1.z == Q2.z) && (Q1.w < Q2.w)));
        }
    };
}