/////////////////////////////////////////////////////////////////////////////
// CNEVecArith.h : Vector and matrix arithmetics
//
// (c) 2001 Ole Dittmann, CPP Studios


#pragma once

#define NENGINE_EXP
#define ASSERT_DBG
#define ulong unsigned long


// Some math defines
#define PI 3.141592654
#define PIf 3.141592654f
#define DEG2RAD 0.017453292
#define DEG2RADf 0.017453292f
#define RAD2DEG 57.29577951
#define RAD2DEGf 57.29577951f


// Predeclares
class CNEVector3;
class CNEEuler;
class CNEQuaternion;
class CNEMatrix3;
class CNEMatrix4;
class CNEPosition;
class CNEBoundBox;



/////////////////////////////////////////////////////////////////////////////
// CNEVector2 - 2D vectors

class NENGINE_EXP CNEVector2
	{
	public:
	union
		{
		float k[2];
		struct
			{
			float x;
			float y;
			};
		struct
			{
			float u;
			float v;
			};
		};
	
	// Constructors
	explicit CNEVector2(bool b)
		{}
	CNEVector2(float s=0)
		{x=s; y=s;}
	CNEVector2(float sx, float sy)
		{x=sx; y=sy;}
	CNEVector2(const float* p)
		{x=p[0]; y=p[1];}
	
	// Set functions from various kinds of src
	CNEVector2& Set(const CNEVector2& Vec)
		{*this=Vec;
		return *this;}
	CNEVector2& Set(float s=0)
		{x=s; y=s;
		return *this;}
	CNEVector2& Set(float sx, float sy)
		{x=sx; y=sy;
		return *this;}
	CNEVector2& Set(const float* p)
		{x=p[0]; y=p[1];
		return *this;}

	// Index operator for all integer types
	const float& operator [] (char i) const
		{ASSERT_DBG(i>=0 && i<=1);
		return k[i];}
	float& operator [] (char i) 
		{ASSERT_DBG(i>=0 && i<=1);
		return k[i];}
	const float& operator [] (unsigned char i) const
		{ASSERT_DBG(i>=0 && i<=1);
		return k[i];}
	float& operator [] (unsigned char i) 
		{ASSERT_DBG(i>=0 && i<=1);
		return k[i];}
	const float& operator [] (short i) const
		{ASSERT_DBG(i>=0 && i<=1);
		return k[i];}
	float& operator [] (short i) 
		{ASSERT_DBG(i>=0 && i<=1);
		return k[i];}
	const float& operator [] (unsigned short i) const
		{ASSERT_DBG(i>=0 && i<=1);
		return k[i];}
	float& operator [] (unsigned short i) 
		{ASSERT_DBG(i>=0 && i<=1);
		return k[i];}
	const float& operator [] (long i) const
		{ASSERT_DBG(i>=0 && i<=1);
		return k[i];}
	float& operator [] (long i) 
		{ASSERT_DBG(i>=0 && i<=1);
		return k[i];}
	const float& operator [] (unsigned long i) const
		{ASSERT_DBG(i>=0 && i<=1);
		return k[i];}
	float& operator [] (unsigned long i) 
		{ASSERT_DBG(i>=0 && i<=1);
		return k[i];}
	const float& operator [] (int i) const
		{ASSERT_DBG(i>=0 && i<=1);
		return k[i];}
	float& operator [] (int i) 
		{ASSERT_DBG(i>=0 && i<=1);
		return k[i];}
	const float& operator [] (unsigned int i) const
		{ASSERT_DBG(i>=0 && i<=1);
		return k[i];}
	float& operator [] (unsigned int i) 
		{ASSERT_DBG(i>=0 && i<=1);
		return k[i];}

	// Use as float array
	operator const float* () const
		{return k;}
	operator float* ()
		{return k;}

	// Component add/sub
	CNEVector2& Add(const CNEVector2& Vec)
		{x+=Vec.x; y+=Vec.y;
		return *this;}
	CNEVector2& Sub(const CNEVector2& Vec)
		{x-=Vec.x; y-=Vec.y;
		return *this;}
	CNEVector2& AddMul(const CNEVector2& Vec, float Scl)
		{x+=Vec.x*Scl; y+=Vec.y*Scl;
		return *this;}

	// Component mul/div
	CNEVector2& Mul(const CNEVector2& Vec)
		{x*=Vec.x; y*=Vec.y;
		return *this;}
	CNEVector2& Div(const CNEVector2& Vec)
		{x/=Vec.x; y/=Vec.y;
		return *this;}

	// Scalar mul/div
	CNEVector2& Mul(float Scl)
		{x=x*Scl; y=y*Scl;
		return *this;}
	CNEVector2& Div(float Scl)
		{x=x/Scl; y=y/Scl;
		return *this;}

	// Invert components regarding addition or multiplication
	CNEVector2& InvertAdd()
		{x=-x; y=-y;
		return *this;}
	CNEVector2& InvertMul()
		{x=1/x; y=1/y;
		return *this;}
	};



/////////////////////////////////////////////////////////////////////////////
// CNEVector3 - 3D vectors

class NENGINE_EXP CNEVector3
	{
	public:
	union
		{
		float k[3];
		struct
			{
			float x;
			float y;
			float z;
			};
		struct
			{
			float u;
			float v;
			float w;
			};
		struct
			{
			float r;
			float g;
			float b;
			};
		};
	
	// Constructors
	explicit CNEVector3(bool b)
		{}
	CNEVector3(float s=0)
		{x=s; y=s; z=s;}
	CNEVector3(float sx, float sy, float sz=0)
		{x=sx; y=sy; z=sz;}
	CNEVector3(const float* p)
		{x=p[0]; y=p[1]; z=p[2];}
	CNEVector3(const CNEVector2& Vec, float sz=0)
		{x=Vec.x; y=Vec.y; z=sz;}

	// Set content from various kind of src
	CNEVector3& Set(float s=0)
		{x=s; y=s; z=s;
		return *this;}
	CNEVector3& Set(float sx, float sy, float sz=0)
		{x=sx; y=sy; z=sz;
		return *this;}
	CNEVector3& Set(const float* p)
		{x=p[0]; y=p[1]; z=p[2];
		return *this;}
	CNEVector3& Set(const CNEVector2& Vec, float sz=0)
		{x=Vec.x; y=Vec.y; z=sz;
		return *this;}
	CNEVector3& Set(const CNEVector3& Vec)
		{*this=Vec;
		return *this;}

	// Index operator for all integer types
	const float& operator [] (char i) const
		{ASSERT_DBG(i>=0 && i<=2);
		return k[i];}
	float& operator [] (char i) 
		{ASSERT_DBG(i>=0 && i<=2);
		return k[i];}
	const float& operator [] (unsigned char i) const
		{ASSERT_DBG(i>=0 && i<=2);
		return k[i];}
	float& operator [] (unsigned char i) 
		{ASSERT_DBG(i>=0 && i<=2);
		return k[i];}
	const float& operator [] (short i) const
		{ASSERT_DBG(i>=0 && i<=2);
		return k[i];}
	float& operator [] (short i) 
		{ASSERT_DBG(i>=0 && i<=2);
		return k[i];}
	const float& operator [] (unsigned short i) const
		{ASSERT_DBG(i>=0 && i<=2);
		return k[i];}
	float& operator [] (unsigned short i) 
		{ASSERT_DBG(i>=0 && i<=2);
		return k[i];}
	const float& operator [] (long i) const
		{ASSERT_DBG(i>=0 && i<=2);
		return k[i];}
	float& operator [] (long i) 
		{ASSERT_DBG(i>=0 && i<=2);
		return k[i];}
	const float& operator [] (unsigned long i) const
		{ASSERT_DBG(i>=0 && i<=2);
		return k[i];}
	float& operator [] (unsigned long i) 
		{ASSERT_DBG(i>=0 && i<=2);
		return k[i];}
	const float& operator [] (int i) const
		{ASSERT_DBG(i>=0 && i<=2);
		return k[i];}
	float& operator [] (int i) 
		{ASSERT_DBG(i>=0 && i<=2);
		return k[i];}
	const float& operator [] (unsigned int i) const
		{ASSERT_DBG(i>=0 && i<=2);
		return k[i];}
	float& operator [] (unsigned int i) 
		{ASSERT_DBG(i>=0 && i<=2);
		return k[i];}

	// Use as float array
	operator const float* () const
		{return k;}
	operator float* ()
		{return k;}

	// Use as 2D vector
	operator const CNEVector2& () const
		{return *((CNEVector2*)this);}
	operator CNEVector2& ()
		{return *((CNEVector2*)this);}
/*
	// Use as D3D vector
	operator D3DXVECTOR3* ()
		{return (D3DXVECTOR3*)k;}
	operator const D3DXVECTOR3*  () const
		{return (D3DXVECTOR3*)k;}
*/
	// Component add/sub
	CNEVector3& Add(const CNEVector3& Vec)
		{x+=Vec.x; y+=Vec.y; z+=Vec.z;
		return *this;}
	CNEVector3& Sub(const CNEVector3& Vec)
		{x-=Vec.x; y-=Vec.y; z-=Vec.z;
		return *this;}
	CNEVector3& AddMul(const CNEVector3& Vec, float Scl)
		{x+=Vec.x*Scl; y+=Vec.y*Scl; z+=Vec.z*Scl;
		return *this;}


	// Component mul/div
	CNEVector3& Mul(const CNEVector3& Vec)
		{x*=Vec.x; y*=Vec.y; z*=Vec.z;
		return *this;}
	CNEVector3& Div(const CNEVector3& Vec)
		{x/=Vec.x; y/=Vec.y; z/=Vec.z;
		return *this;}

	// Scalar mul/div
	CNEVector3& Mul(float Scl)
		{x=x*Scl; y=y*Scl; z=z*Scl;
		return *this;}
	CNEVector3& Div(float Scl)
		{x=x/Scl; y=y/Scl; z=z/Scl;
		return *this;}

	// Invert components regarding addition or multiplication
	CNEVector3& InvertAdd()
		{x=-x; y=-y; z=-z;
		return *this;}
	CNEVector3& InvertMul()
		{x=1/x; y=1/y; z=1/z;
		return *this;}

	CNEVector3& Clamp(float From=0.0f, float To=1.0f); // Clamp components

	float Len(void) const;							// Return length
	CNEVector3& Norm(void);							// Normalize
	float NormLen(void);							// Normalize and return length

	CNEVector3& Rot(const CNEMatrix3& Mat);			// Rotate
	CNEVector3& RotInv(const CNEMatrix3& Mat);		// Rotate inverse
	CNEVector3& RotAround(const CNEVector3& Cent, const CNEMatrix3& Mat);
	CNEVector3& RotAroundInv(const CNEVector3& Cent, const CNEMatrix3& Mat);

	CNEVector3& Trans(const CNEPosition& Pos);			// Transform with position
	CNEVector3& TransInv(const CNEPosition& Pos);		// Transform with inverse position
	
	float Dot(const CNEVector3& Vec)					// Dot product
		{return (x*Vec.x + y*Vec.y + z*Vec.z);}
	CNEVector3& Cross(const CNEVector3& Vec);			// Cross product

	CNEVector3& Perpendic(const CNEVector3& p0,			// Perpendicular vector from 3 pts
		const CNEVector3& p1, const CNEVector3& p2); 
	CNEVector3& Reflect(const CNEVector3& Norm);			// Ray-reflection
	CNEVector3& Fract(const CNEVector3& Norm, float Idx);	// Ray-fraction

	CNEVector3& Lerp(const CNEVector3& Vec, float t);	// Linear interpolation
	};
	


/////////////////////////////////////////////////////////////////////////////
// CNEVector4 - 4D vectors

class NENGINE_EXP CNEVector4
	{
	public:
	union
		{
		float k[4];
		struct
			{
			float x;
			float y;
			float z;
			float w;
			};
		struct
			{
			float r;
			float g;
			float b;
			float a;
			};
		};
	
	// Constructors
	explicit CNEVector4(bool b)
		{}
	CNEVector4(float s=0)
		{x=s; y=s; z=s; w=1;}
	CNEVector4(float sx, float sy, float sz=0, float sw=1)
		{x=sx; y=sy; z=sz; w=sw;}
	CNEVector4(const float* p)
		{x=p[0]; y=p[1]; z=p[2]; w=p[3];}
	CNEVector4(const CNEVector2& Vec, float sz=0, float sw=1)
		{x=Vec.x; y=Vec.y; z=sz; w=sw;}
	CNEVector4(const CNEVector3& Vec, float sw=1)
		{x=Vec.x; y=Vec.y; z=Vec.z; w=sw;}

	// Set content from various kind of src
	CNEVector4& Set(float s=0)
		{x=s; y=s; z=s; w=1;
		return *this;}
	CNEVector4& Set(float sx, float sy, float sz=0, float sw=1)
		{x=sx; y=sy; z=sz; w=sw;
		return *this;}
	CNEVector4& Set(const float* p)
		{x=p[0]; y=p[1]; z=p[2]; w=p[3];
		return *this;}
	CNEVector4& Set3(const float* p)
		{x=p[0]; y=p[1]; z=p[2]; w=1;
		return *this;}
	CNEVector4& Set(const CNEVector2& Vec, float sz=0, float sw=1)
		{x=Vec.x; y=Vec.y; z=sz; w=sw;
		return *this;}
	CNEVector4& Set(const CNEVector3& Vec, float sw=1)
		{x=Vec.x; y=Vec.y; z=Vec.z; w=sw;
		return *this;}
	CNEVector4& Set(const CNEVector4& Vec)
		{*this=Vec;
		return *this;}

	// Index operator for all integer types
	const float& operator [] (char i) const
		{ASSERT_DBG(i>=0 && i<=3);
		return k[i];}
	float& operator [] (char i) 
		{ASSERT_DBG(i>=0 && i<=3);
		return k[i];}
	const float& operator [] (unsigned char i) const
		{ASSERT_DBG(i>=0 && i<=3);
		return k[i];}
	float& operator [] (unsigned char i) 
		{ASSERT_DBG(i>=0 && i<=3);
		return k[i];}
	const float& operator [] (short i) const
		{ASSERT_DBG(i>=0 && i<=3);
		return k[i];}
	float& operator [] (short i) 
		{ASSERT_DBG(i>=0 && i<=3);
		return k[i];}
	const float& operator [] (unsigned short i) const
		{ASSERT_DBG(i>=0 && i<=3);
		return k[i];}
	float& operator [] (unsigned short i) 
		{ASSERT_DBG(i>=0 && i<=3);
		return k[i];}
	const float& operator [] (long i) const
		{ASSERT_DBG(i>=0 && i<=3);
		return k[i];}
	float& operator [] (long i) 
		{ASSERT_DBG(i>=0 && i<=3);
		return k[i];}
	const float& operator [] (unsigned long i) const
		{ASSERT_DBG(i>=0 && i<=3);
		return k[i];}
	float& operator [] (unsigned long i) 
		{ASSERT_DBG(i>=0 && i<=3);
		return k[i];}
	const float& operator [] (int i) const
		{ASSERT_DBG(i>=0 && i<=3);
		return k[i];}
	float& operator [] (int i) 
		{ASSERT_DBG(i>=0 && i<=3);
		return k[i];}
	const float& operator [] (unsigned int i) const
		{ASSERT_DBG(i>=0 && i<=3);
		return k[i];}
	float& operator [] (unsigned int i) 
		{ASSERT_DBG(i>=0 && i<=3);
		return k[i];}

	// Use as float array
	operator const float* () const
		{return k;}
	operator float* ()
		{return k;}

	// Use as 2D vector
	operator const CNEVector2& () const
		{return *((CNEVector2*)this);}
	operator CNEVector2& ()
		{return *((CNEVector2*)this);}

	// Use as 3D vector
	operator const CNEVector3& () const
		{return *((CNEVector3*)this);}
	operator CNEVector3& ()
		{return *((CNEVector3*)this);}
/*
	// Use as D3D vector
	operator D3DXVECTOR4* ()
		{return (D3DXVECTOR4*)k;}
	operator const D3DXVECTOR4*  () const
		{return (D3DXVECTOR4*)k;}
*/
	// Component add/sub
	CNEVector4& Add3(const CNEVector4& Vec)
		{x+=Vec.x; y+=Vec.y; z+=Vec.z;
		return *this;}
	CNEVector4& Sub3(const CNEVector4& Vec)
		{x-=Vec.x; y-=Vec.y; z-=Vec.z;
		return *this;}
	CNEVector4& AddMul3(const CNEVector4& Vec, float Scl)
		{x+=Vec.x*Scl; y+=Vec.y*Scl; z+=Vec.z*Scl;
		return *this;}

	// Component mul/div
	CNEVector4& Mul3(const CNEVector4& Vec)
		{x*=Vec.x; y*=Vec.y; z*=Vec.z;
		return *this;}
	CNEVector4& Div3(const CNEVector4& Vec)
		{x/=Vec.x; y/=Vec.y; z/=Vec.z;
		return *this;}

	// Scalar mul/div
	CNEVector4& Mul3(float Scl)
		{x=x*Scl; y=y*Scl; z=z*Scl;
		return *this;}
	CNEVector4& Div3(float Scl)
		{x=x/Scl; y=y/Scl; z=z/Scl;
		return *this;}

	// Invert components regarding addition or multiplication
	CNEVector4& InvertAdd3()
		{x=-x; y=-y; z=-z;
		return *this;}
	CNEVector4& InvertMul3()
		{x=1/x; y=1/y; z=1/z;
		return *this;}

	CNEVector4& Clamp(float From=0.0f, float To=1.0f);	// Clamp components
	CNEVector4& Lerp(const CNEVector4& Vec, float t);	// Linear interpolation
	};



/////////////////////////////////////////////////////////////////////////////
// CNEEuler - Represents a rotation using Euler degrees

class NENGINE_EXP CNEEuler : public CNEVector3
	{
	public:
	bool m_zyx;		// If true rotations are done reverse order
	
	// Constructors
	explicit CNEEuler(bool b) : CNEVector3(b)
		{m_zyx=b;}
	CNEEuler(float s=0, bool zyx=false) : CNEVector3(s)
		{m_zyx=zyx;}
	CNEEuler(float sx, float sy, float sz=0, bool zyx=false) : CNEVector3(sx, sy, sz)
		{m_zyx=zyx;}
	CNEEuler(const float* p, bool zyx=false) : CNEVector3(p)
		{m_zyx=zyx;}
	CNEEuler(const CNEVector3& Vec, bool zyx=false) : CNEVector3(Vec)
		{m_zyx=zyx;}

	// Set content from various kind of src
	CNEEuler& Set(float s=0)
		{x=s; y=s; z=s;
		return *this;}
	CNEEuler& Set(float sx, float sy, float sz=0)
		{x=sx; y=sy; z=sz;
		return *this;}
	CNEEuler& Set(const float* p)
		{x=p[0]; y=p[1]; z=p[2];
		return *this;}
	CNEEuler& Set(const CNEVector3& Vec)
		{x=Vec.x; y=Vec.y; z=Vec.z;
		return *this;}
	CNEEuler& Set(const CNEEuler& Src)
		{*this=Src;
		return *this;}
	};
	


/////////////////////////////////////////////////////////////////////////////
// CNEQuaternion - Quaternions representing rotations

class NENGINE_EXP CNEQuaternion : public CNEVector4
	{
	public:
	
	// Constructors
	explicit CNEQuaternion(bool b) : CNEVector4(b)
		{}
	CNEQuaternion(float s=0) : CNEVector4(s)
		{}
	CNEQuaternion(float sx, float sy, float sz=0, float sw=1)
		: CNEVector4(sx, sy, sz, sw)
		{}
	CNEQuaternion(const float* p) : CNEVector4(p)
		{}
	CNEQuaternion(const CNEMatrix3& Mat)
		: CNEVector4(false)
		{Set(Mat);}
	CNEQuaternion(const CNEEuler& Degs)
		: CNEVector4(false)
		{Set(Degs);}
	CNEQuaternion(const CNEVector3& Axis, float Deg)
		: CNEVector4(false)
		{Set(Axis, Deg);}
	CNEQuaternion(const CNEVector4& Vec)
		: CNEVector4(Vec)
		{}		

	// Set content from various kind of src
	CNEQuaternion& Set(float s=0)
		{x=s; y=s; z=s; w=1;
		return *this;}
	CNEQuaternion& Set(float sx, float sy, float sz=0, float sw=1)
		{x=sx; y=sy; z=sz; w=sw;
		return *this;}
	CNEQuaternion& Set(const float* p)
		{x=p[0]; y=p[1]; z=p[2]; w=p[3];
		return *this;}
	CNEQuaternion& Set(const CNEMatrix3& Mat);
	CNEQuaternion& Set(const CNEEuler& Degs);
	CNEQuaternion& Set(const CNEVector3& Axis, float Deg);
	CNEQuaternion& Set(const CNEVector4& Vec)
		{x=Vec.x; y=Vec.y; z=Vec.z; w=Vec.w;
		return *this;}
	CNEQuaternion& Set(const CNEQuaternion& Src)
		{*this=Src;
		return *this;}
/*
	operator D3DXQUATERNION* ()						// Use as D3D quaternion
		{return (D3DXQUATERNION*)k;}
	operator const D3DXQUATERNION*  () const
		{return (D3DXQUATERNION*)k;}
*/
	CNEQuaternion& Norm();							// Normalize
	CNEQuaternion& Invert();						// Invert (normalized quaternions only)
	CNEQuaternion& MulR(CNEQuaternion& Quat);		// Multiply (rotation Quat after this)
	CNEQuaternion& MulL(CNEQuaternion& Quat);		// Multiply (rotation this after Quat)
	CNEQuaternion& Slerp(CNEQuaternion& Q1, CNEQuaternion& Q2, float t); // Spherical linear interpolation
	};



/////////////////////////////////////////////////////////////////////////////
// CNEMatrix3 - 3D (rotational) Matrix

class NENGINE_EXP CNEMatrix3
	{
	public:
	union
		{
		float m[3][3];
		struct
			{
			float m00;
			float m01;
			float m02;
			float m10;
			float m11;
			float m12;
			float m20;
			float m21;
			float m22;
			};
		};
	
	// Constructors
	explicit CNEMatrix3(bool b)
		{}
	CNEMatrix3(	
		float sm00=1, float sm10=0, float sm20=0,
		float sm01=0, float sm11=1, float sm21=0,
		float sm02=0, float sm12=0, float sm22=1);
	CNEMatrix3(const CNEEuler& Degs);
	CNEMatrix3(const CNEVector3& Axis, float Deg);
	CNEMatrix3(const CNEQuaternion& Quat);

	CNEMatrix3& Set(const CNEMatrix3& Src)				// Set content
		{*this=Src;
		return *this;}
	CNEMatrix3& Set(
		float sm00=1, float sm10=0, float sm20=0,
		float sm01=0, float sm11=1, float sm21=0,
		float sm02=0, float sm12=0, float sm22=1)
		{this->CNEMatrix3::CNEMatrix3(sm00, sm10, sm20, sm01, sm11, sm21, sm02, sm12, sm22);
		return *this;}
	CNEMatrix3& Set(const CNEEuler& Degs)
		{this->CNEMatrix3::CNEMatrix3(Degs);
		return *this;}
	CNEMatrix3& Set(const CNEVector3& Axis, float Deg)
		{this->CNEMatrix3::CNEMatrix3(Axis, Deg);
		return *this;}
	CNEMatrix3& Set(const CNEQuaternion& Quat)
		{this->CNEMatrix3::CNEMatrix3(Quat);
		return *this;}

	// Index operator for all integer types returns column vector
	CNEVector3& operator [] (char i)
		{ASSERT_DBG(i>=0 && i<=2);
		return (CNEVector3&)m[i];}
	const CNEVector3& operator [] (char i) const
		{ASSERT_DBG(i>=0 && i<=2);
		return (CNEVector3&)m[i];}
	CNEVector3& operator [] (unsigned char i)
		{ASSERT_DBG(i>=0 && i<=2);
		return (CNEVector3&)m[i];}
	const CNEVector3& operator [] (unsigned char i) const
		{ASSERT_DBG(i>=0 && i<=2);
		return (CNEVector3&)m[i];}
	CNEVector3& operator [] (short i)
		{ASSERT_DBG(i>=0 && i<=2);
		return (CNEVector3&)m[i];}
	const CNEVector3& operator [] (short i) const
		{ASSERT_DBG(i>=0 && i<=2);
		return (CNEVector3&)m[i];}
	CNEVector3& operator [] (unsigned short i)
		{ASSERT_DBG(i>=0 && i<=2);
		return (CNEVector3&)m[i];}
	const CNEVector3& operator [] (unsigned short i) const
		{ASSERT_DBG(i>=0 && i<=2);
		return (CNEVector3&)m[i];}
	CNEVector3& operator [] (long i)
		{ASSERT_DBG(i>=0 && i<=2);
		return (CNEVector3&)m[i];}
	const CNEVector3& operator [] (long i) const
		{ASSERT_DBG(i>=0 && i<=2);
		return (CNEVector3&)m[i];}
	CNEVector3& operator [] (unsigned long i)
		{ASSERT_DBG(i>=0 && i<=2);
		return (CNEVector3&)m[i];}
	const CNEVector3& operator [] (unsigned long i) const
		{ASSERT_DBG(i>=0 && i<=2);
		return (CNEVector3&)m[i];}
	CNEVector3& operator [] (int i)
		{ASSERT_DBG(i>=0 && i<=2);
		return (CNEVector3&)m[i];}
	const CNEVector3& operator [] (int i) const
		{ASSERT_DBG(i>=0 && i<=2);
		return (CNEVector3&)m[i];}
	CNEVector3& operator [] (unsigned int i)
		{ASSERT_DBG(i>=0 && i<=2);
		return (CNEVector3&)m[i];}
	const CNEVector3& operator [] (unsigned int i) const
		{ASSERT_DBG(i>=0 && i<=2);
		return (CNEVector3&)m[i];}

	// Use as float array
	operator const float* () const
		{return m[0];}
	operator float* ()
		{return m[0];}

	CNEMatrix3& Norm(void);								// Normalize
	CNEMatrix3& Invert(void);							// Invert (normalized matrices only)
	CNEMatrix3& MulR(const CNEMatrix3& Mat);			// Multiply (rotation Mat after this)
	CNEMatrix3& MulInvR(const CNEMatrix3& Mat);
	CNEMatrix3& MulL(const CNEMatrix3& Mat);			// Multiply (rotation this after mat)
	CNEMatrix3& MulInvL(const CNEMatrix3& Mat);

	// Construct matrix that points towards a direction and an up-vector
	CNEMatrix3& LookAt(const CNEVector3& Dir, const CNEVector3& UpVec);
	// Point towards a direction while automatically selecting a suitable up-vector
	CNEMatrix3& LookAt(const CNEVector3& Dir);
	};



/////////////////////////////////////////////////////////////////////////////
// CNEMatrix4 - 4D Matrix

class NENGINE_EXP CNEMatrix4
	{
	public:
	union
		{
		float m[4][4];
		struct
			{
			float m00;
			float m01;
			float m02;
			float m03;
			float m10;
			float m11;
			float m12;
			float m13;
			float m20;
			float m21;
			float m22;
			float m23;
			float m30;
			float m31;
			float m32;
			float m33;
			};
		};
	
	// Constructors
	explicit CNEMatrix4(bool b)
		{}
	CNEMatrix4(
		float sm00=1, float sm10=0, float sm20=0, float sm30=0,
		float sm01=0, float sm11=1, float sm21=0, float sm31=0,
		float sm02=0, float sm12=0, float sm22=1, float sm32=0,
		float sm03=0, float sm13=0, float sm23=0, float sm33=1);
	CNEMatrix4(const CNEPosition& Pos);

	// Set content from various kind of src
	CNEMatrix4& Set(const CNEMatrix4& Src)
		{(*this)=Src;
		return *this;}
	CNEMatrix4& Set(
		float sm00=1, float sm10=0, float sm20=0, float sm30=0,
		float sm01=0, float sm11=1, float sm21=0, float sm31=0,
		float sm02=0, float sm12=0, float sm22=1, float sm32=0,
		float sm03=0, float sm13=0, float sm23=0, float sm33=1)
		{this->CNEMatrix4::CNEMatrix4(sm00, sm10, sm20, sm30, sm01, sm11, sm21, sm31,
			sm02, sm12, sm22, sm32, sm03, sm13, sm23, sm33);
		return *this;}
	CNEMatrix4& Set(CNEPosition& Pos)
		{this->CNEMatrix4::CNEMatrix4(Pos);
		return *this;}
		
	// Index operator for all integer types returns column vector
	CNEVector4& operator [] (char i)
		{ASSERT_DBG(i>=0 && i<=3);
		return (CNEVector4&)m[i];}
	const CNEVector4& operator [] (char i) const
		{ASSERT_DBG(i>=0 && i<=3);
		return (CNEVector4&)m[i];}
	CNEVector4& operator [] (unsigned char i)
		{ASSERT_DBG(i>=0 && i<=3);
		return (CNEVector4&)m[i];}
	const CNEVector4& operator [] (unsigned char i) const
		{ASSERT_DBG(i>=0 && i<=3);
		return (CNEVector4&)m[i];}
	CNEVector4& operator [] (short i)
		{ASSERT_DBG(i>=0 && i<=3);
		return (CNEVector4&)m[i];}
	const CNEVector4& operator [] (short i) const
		{ASSERT_DBG(i>=0 && i<=3);
		return (CNEVector4&)m[i];}
	CNEVector4& operator [] (unsigned short i)
		{ASSERT_DBG(i>=0 && i<=3);
		return (CNEVector4&)m[i];}
	const CNEVector4& operator [] (unsigned short i) const
		{ASSERT_DBG(i>=0 && i<=3);
		return (CNEVector4&)m[i];}
	CNEVector4& operator [] (long i)
		{ASSERT_DBG(i>=0 && i<=3);
		return (CNEVector4&)m[i];}
	const CNEVector4& operator [] (long i) const
		{ASSERT_DBG(i>=0 && i<=3);
		return (CNEVector4&)m[i];}
	CNEVector4& operator [] (unsigned long i)
		{ASSERT_DBG(i>=0 && i<=3);
		return (CNEVector4&)m[i];}
	const CNEVector4& operator [] (unsigned long i) const
		{ASSERT_DBG(i>=0 && i<=3);
		return (CNEVector4&)m[i];}
	CNEVector4& operator [] (int i)
		{ASSERT_DBG(i>=0 && i<=3);
		return (CNEVector4&)m[i];}
	const CNEVector4& operator [] (int i) const
		{ASSERT_DBG(i>=0 && i<=3);
		return (CNEVector4&)m[i];}
	CNEVector4& operator [] (unsigned int i)
		{ASSERT_DBG(i>=0 && i<=3);
		return (CNEVector4&)m[i];}
	const CNEVector4& operator [] (unsigned int i) const
		{ASSERT_DBG(i>=0 && i<=3);
		return (CNEVector4&)m[i];}

	// Use as float array
	operator const float* () const
		{return m[0];}
	operator float* ()
		{return m[0];}

/*
    operator D3DMATRIX* ()								// Use as D3D matrix
		{return (D3DMATRIX*)m;}
	operator const D3DMATRIX*  () const
		{return (D3DMATRIX*)m;}
	operator D3DXMATRIX* ()
		{return (D3DXMATRIX*)m;}
	operator const D3DXMATRIX*  () const
		{return (D3DXMATRIX*)m;}
*/
	};



/////////////////////////////////////////////////////////////////////////////
// CNEPosition - 3D Position containing center, rotation and size

class NENGINE_EXP CNEPosition
	{
	public:
	CNEMatrix3 m_Rot;
	CNEVector3 m_Center;
	CNEVector3 m_Scale;

	// Constructors
	explicit CNEPosition(bool b)
		:m_Rot(b), m_Center(b), m_Scale(b)
		{}
	CNEPosition(float x=0, float y=0, float z=0, float Size=1)
		:m_Center(x,y,z), m_Scale(Size)
		{}
	CNEPosition(const CNEVector3& Vec, float Size=1)
		:m_Center(Vec), m_Scale(Size)
		{}
	CNEPosition(const CNEMatrix3& Mat, const CNEVector3& Vec, float Size=1)
		:m_Rot(Mat), m_Center(Vec), m_Scale(Size)
		{}
	CNEPosition(const CNEMatrix3& Mat, const CNEVector3& Vec, const CNEVector3& Size)
		:m_Rot(Mat), m_Center(Vec), m_Scale(Size)
		{}
	CNEPosition(const CNEEuler& Deg, const CNEVector3& Vec, float Size=1)
		:m_Rot(Deg), m_Center(Vec), m_Scale(Size)
		{}
	CNEPosition(const CNEEuler& Deg, const CNEVector3& Vec, const CNEVector3& Size)
		:m_Rot(Deg), m_Center(Vec), m_Scale(Size)
		{}
	CNEPosition(const CNEQuaternion& Quat, const CNEVector3& Vec, float Size=1)
		:m_Rot(Quat), m_Center(Vec), m_Scale(Size)
		{}
	CNEPosition(const CNEQuaternion& Quat, const CNEVector3& Vec, const CNEVector3& Size)
		:m_Rot(Quat), m_Center(Vec), m_Scale(Size)
		{}

	// Set content from various kind of src
	CNEPosition& Set(CNEPosition& Src)
		{*this=Src;
		return *this;}
	CNEPosition& Set(float x=0, float y=0, float z=0, float Size=1)
		{this->CNEPosition::CNEPosition(x, y, z, Size);
		return *this;}
	CNEPosition& Set(const CNEVector3& Vec, float Size=1)
		{this->CNEPosition::CNEPosition(Vec, Size);
		return *this;}
	CNEPosition& Set(const CNEMatrix3& Mat, const CNEVector3& Vec, float Size=1)
		{this->CNEPosition::CNEPosition(Mat, Vec, Size);
		return *this;}
	CNEPosition& Set(const CNEMatrix3& Mat, const CNEVector3& Vec, const CNEVector3& Size)
		{this->CNEPosition::CNEPosition(Mat, Vec, Size);
		return *this;}
	CNEPosition& Set(const CNEEuler& Deg, const CNEVector3& Vec, float Size=1)
		{this->CNEPosition::CNEPosition(Deg, Vec, Size);
		return *this;}
	CNEPosition& Set(const CNEEuler& Deg, const CNEVector3& Vec, const CNEVector3& Size)
		{this->CNEPosition::CNEPosition(Deg, Vec, Size);
		return *this;}
	CNEPosition& Set(const CNEQuaternion& Quat, const CNEVector3& Vec, float Size=1)
		{this->CNEPosition::CNEPosition(Quat, Vec, Size);
		return *this;}
	CNEPosition& Set(const CNEQuaternion& Quat, const CNEVector3& Vec, const CNEVector3& Size)
		{this->CNEPosition::CNEPosition(Quat, Vec, Size);
		return *this;}
		

	CNEPosition& Invert();									// Invert position
	CNEPosition& AccumulateR(const CNEPosition& Pos);		// Pos after this
	CNEPosition& AccumulateInvR(const CNEPosition& Pos);
	CNEPosition& AccumulateL(const CNEPosition& Pos);		// this after Pos
	CNEPosition& AccumulateInvL(const CNEPosition& Pos);

	CNEPosition& AccumulateR(const CNEVector3& Vec);		// Vec after this
	CNEPosition& AccumulateInvR(const CNEVector3& Vec);
	CNEPosition& AccumulateL(const CNEVector3& Vec);		// this after Vec
	CNEPosition& AccumulateInvL(const CNEVector3& Vec);
	};



/////////////////////////////////////////////////////////////////////////////
// CNEBoundBox - 3D bounding box


class NENGINE_EXP CNEBoundBox
	{
	public:
	CNEVector3 m_Min;
	CNEVector3 m_Max;
	
	// Constructors
	explicit CNEBoundBox(bool b)
		: m_Min(b), m_Max(b)
		{}
	CNEBoundBox()
		: m_Min(1e+35f), m_Max(-1e+35f)
		{}

	// Set content
	CNEBoundBox& Set(CNEBoundBox& Src)
		{*this=Src;
		return *this;}
	CNEBoundBox& Set(void)
		{this->CNEBoundBox::CNEBoundBox();
		return *this;}

	CNEBoundBox& Grow(const CNEVector3& Pt);	// "Add" point to bounding box
	CNEBoundBox& Inflate(float Err);			// Inflate the box (all axes)
	bool IsEmpty()								// Is box empty?
		{
		return m_Max[0]<m_Min[0];
		}
	};	
	


/////////////////////////////////////////////////////////////////////////////
// CNEColor - RGBA color


class NENGINE_EXP CNEColor : public CNEVector4
	{
	public:

	// Constructors
	explicit CNEColor(bool b) : CNEVector4(b)
		{}
	CNEColor(float s=0) : CNEVector4(s)
		{}
	CNEColor(float sr, float sg, float sb=0, float sa=1)
		: CNEVector4(sr, sg, sb, sa)
		{}
	CNEColor(const float* p) : CNEVector4(p)
		{}
	CNEColor(const CNEVector3& Vec, float sa=1)
		: CNEVector4(Vec, sa)
		{}
	CNEColor(ulong argb) : CNEVector4(false)
		{
		SetARGB(argb);
		}

	// Load with ARGB color from an ulong value
	CNEColor& SetARGB(ulong argb);
	// Calculate ARGB color to an ulong value
	ulong GetARGB() const;
	};



/////////////////////////////////////////////////////////////////////////////
// Missing inline functions implementations


inline CNEVector3& CNEVector3::Trans(const CNEPosition& Pos)
	{return Mul(Pos.m_Scale).Rot(Pos.m_Rot).Add(Pos.m_Center);}

inline CNEVector3& CNEVector3::TransInv(const CNEPosition& Pos)
	{return Sub(Pos.m_Center).RotInv(Pos.m_Rot).Div(Pos.m_Scale);}
