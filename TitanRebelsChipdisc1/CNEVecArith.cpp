/////////////////////////////////////////////////////////////////////////////
// CNEVecArith.cpp : Vector and matrix arithmetics
//
// (c) 2001 Ole Dittmann

#include "stdafx.h"

/////////////////////////////////////////////////////////////////////////////
// CNEVector3 - 3D vectors

CNEVector3& CNEVector3::Clamp(float From, float To)
	{
	if (x<From) x=From;
	if (x>To) x=To;
	if (y<From) y=From;
	if (y>To) y=To;
	if (z<From) z=From;
	if (z>To) z=To;
	return *this;
	}


CNEVector3& CNEVector3::Rot(const CNEMatrix3& Mat)
	{
	float xn,yn,zn;

	xn=x; yn=y; zn=z;
	x=xn*Mat.m00+yn*Mat.m10+zn*Mat.m20;
	y=xn*Mat.m01+yn*Mat.m11+zn*Mat.m21;
	z=xn*Mat.m02+yn*Mat.m12+zn*Mat.m22;
	return *this;
	}


CNEVector3& CNEVector3::RotInv(const CNEMatrix3& Mat)
	{
	float xn,yn,zn;

	xn=x; yn=y; zn=z;
	x=xn*Mat.m00+yn*Mat.m01+zn*Mat.m02;
	y=xn*Mat.m10+yn*Mat.m11+zn*Mat.m12;
	z=xn*Mat.m20+yn*Mat.m21+zn*Mat.m22;
	return *this;
	}
	

CNEVector3& CNEVector3::RotAround(const CNEVector3& Cent, const CNEMatrix3& Mat)
	{
	float xn,yn,zn;

	xn=x-Cent.x; yn=y-Cent.y; zn=z-Cent.z;
	x=xn*Mat.m00+yn*Mat.m10+zn*Mat.m20+Cent.x;
	y=xn*Mat.m01+yn*Mat.m11+zn*Mat.m21+Cent.y;
	z=xn*Mat.m02+yn*Mat.m12+zn*Mat.m22+Cent.z;
	return *this;
	}


CNEVector3& CNEVector3::RotAroundInv(const CNEVector3& Cent, const CNEMatrix3& Mat)
	{
	float xn,yn,zn;

	xn=x-Cent.x; yn=y-Cent.y; zn=z-Cent.z;
	x=xn*Mat.m00+yn*Mat.m01+zn*Mat.m02+Cent.x;
	y=xn*Mat.m10+yn*Mat.m11+zn*Mat.m12+Cent.y;
	z=xn*Mat.m20+yn*Mat.m21+zn*Mat.m22+Cent.z;
	return *this;
	}


CNEVector3& CNEVector3::Cross(const CNEVector3& Vec)
	{
	float xn,yn,zn;
	
	xn=z*Vec.y - y*Vec.z;
	yn=x*Vec.z - z*Vec.x;
	zn=y*Vec.x - x*Vec.y;
	x=xn; y=yn; z=zn;
	return *this;
	}


CNEVector3& CNEVector3::Perpendic(const CNEVector3& p0, const CNEVector3& p1, const CNEVector3& p2)
	{
	float px,py,pz,qx,qy,qz;

	px=p2.x-p1.x;
	py=p2.y-p1.y;
	pz=p2.z-p1.z;
	qx=p0.x-p1.x;
	qy=p0.y-p1.y;
	qz=p0.z-p1.z;
	x=pz*qy-py*qz;
	y=px*qz-pz*qx;
	z=py*qx-px*qy;
	return *this;
	}
	
	
float CNEVector3::Len(void) const
	{
	return (float)sqrt(x*x+y*y+z*z);
	}


CNEVector3& CNEVector3::Norm(void)
	{
	float l;

	l=(float)sqrt(x*x+y*y+z*z);
	if (l==0.0f) return *this;
	x=x/l;
	y=y/l;
	z=z/l;
	return *this;
	}


float CNEVector3::NormLen(void)
	{
	float l;

	l=(float)sqrt(x*x+y*y+z*z);
	if (l==0.0f) return l;
	x=x/l;
	y=y/l;
	z=z/l;
	return l;
	}


CNEVector3& CNEVector3::Reflect(const CNEVector3& Norm)
	{
    float c;

	c=Dot(Norm)*2;
    x=x - c*Norm.x;
    y=y - c*Norm.y;
    z=z - c*Norm.z;
	return *this;
	}


CNEVector3& CNEVector3::Fract(const CNEVector3& Norm, float Idx)
	{
	float c,r;

	c=-Dot(Norm);
	r=1.0f + Idx*Idx*(c*c - 1.0f);
	if (r<0.0f) return Reflect(Norm);
	c=Idx*c - (float)sqrt(r);
	x=Idx*x + c*Norm.x;
	y=Idx*y + c*Norm.y;
	z=Idx*z + c*Norm.z;
	return *this;
	}


CNEVector3& CNEVector3::Lerp(const CNEVector3& Vec, float t)
	{
	float tn=1-t;
	x=x*tn+Vec.x*t;
	y=y*tn+Vec.y*t;
	z=z*tn+Vec.z*t;
	return *this;
	}


/////////////////////////////////////////////////////////////////////////////
// CNEVector4 - 4D vectors

CNEVector4& CNEVector4::Clamp(float From, float To)
	{
	if (x<From) x=From;
	if (x>To) x=To;
	if (y<From) y=From;
	if (y>To) y=To;
	if (z<From) z=From;
	if (z>To) z=To;
	if (w<From) w=From;
	if (w>To) w=To;
	return *this;
	}


CNEVector4& CNEVector4::Lerp(const CNEVector4& Vec, float t)
	{
	float tn=1-t;
	x=x*tn+Vec.x*t;
	y=y*tn+Vec.y*t;
	z=z*tn+Vec.z*t;
	w=w*tn+Vec.w*t;
	return *this;
	}


/////////////////////////////////////////////////////////////////////////////
// CNEQuaternion - Quaternions representing rotations

CNEQuaternion& CNEQuaternion::Set(const CNEMatrix3& Mat)
	{
	float  tr, s;
	int    h, i, j;
	int nxt[3] = {1, 2, 0};

	// check the diagonal
	tr = Mat.m00 + Mat.m11 + Mat.m22;
	if (tr > 0.0)
		{
		s = (float)sqrt(tr + 1.0);
		w = s / 2;
		s = 0.5f / s;
		x = (Mat.m21 - Mat.m12) * s;
		y = (Mat.m02 - Mat.m20) * s;
		z = (Mat.m10 - Mat.m01) * s;
		}
	else
		{		
		h = 0;
		if (Mat.m11 > Mat.m00) h = 1;
		if (Mat.m22 > Mat.m[h][h]) h = 2;
		i = nxt[h];
		j = nxt[i];

		s = (float)sqrt((Mat.m[h][h] - (Mat.m[i][i] + Mat.m[j][j])) + 1);
		k[h] = s * 0.5f;
		if (s != 0.0) s = 0.5f / s;
		w = (Mat.m[j][i] - Mat.m[i][j]) * s;
		k[i] = (Mat.m[i][h] + Mat.m[h][i]) * s;
		k[j] = (Mat.m[j][h] + Mat.m[h][j]) * s;
		}
	return *this;
	}


CNEQuaternion& CNEQuaternion::Set(const CNEEuler& Degs)
	{
	float cr, cp, cy, sr, sp, sy, cpcy, spsy;

	cr = (float)cos(Degs.x/2);
	cp = (float)cos(Degs.y/2);
	cy = (float)cos(Degs.z/2);

	sr = (float)sin(Degs.x/2);
	sp = (float)sin(Degs.y/2);
	sy = (float)sin(Degs.z/2);
	
	cpcy = cp * cy;
	spsy = sp * sy;

	x = sr * cpcy - cr * spsy;
	y = cr * sp * cy + sr * cp * sy;
	z = cr * cp * sy - sr * sp * cy;
	w = cr * cpcy + sr * spsy;
	return *this;
	}


CNEQuaternion& CNEQuaternion::Set(const CNEVector3& Axis, float Deg)
	{
	float si=(float)sin(Deg/2);
	x=Axis.x*si;
	y=Axis.y*si;
	z=Axis.z*si;
	w=(float)cos(Deg/2);
	return *this;
	}


CNEQuaternion& CNEQuaternion::Norm()
	{
	float l;

	l=x*x+y*y+z*z+w*w;
	if (l==0.0f) return *this;
	x=x/l;
	y=y/l;
	z=z/l;
	w=w/l;
	return *this;
	}


CNEQuaternion& CNEQuaternion::Invert()
	{
	x=-x;
	y=-y;
	z=-z;
	return *this;
	}


CNEQuaternion& CNEQuaternion::MulR(CNEQuaternion& Quat)
	{
	float A, B, C, D, E, F, G, H;

	A=(w + x)*(Quat.w + Quat.x);
	B=(z - y)*(Quat.y - Quat.z);
	C=(x - w)*(Quat.y - Quat.z);
	D=(y + z)*(Quat.x - Quat.w);
	E=(x + z)*(Quat.x + Quat.y);
	F=(x - z)*(Quat.x - Quat.y);
	G=(w + y)*(Quat.w - Quat.z);
	H=(w - y)*(Quat.w + Quat.z);

	w = B + (-E - F + G + H)/2;
	x = A - (E + F + G + H)/2; 
	y = -C + (E - F + G - H)/2;
	z = -D + (E - F - G + H)/2;
	return *this;
	}


CNEQuaternion& CNEQuaternion::Slerp(CNEQuaternion& Q1, CNEQuaternion& Q2, float t)
	{	
	float Q2a[4];
	float omega, cosom, sinom, scale0, scale1;

	// calc cosine
	cosom = Q1.x * Q2.x + Q1.y * Q2.y + Q1.z * Q2.z + Q1.w * Q2.w;

	// adjust signs (if necessary)
	if (cosom <0)
		{
		cosom = -cosom;
		Q2a[0] = - Q2.x;
		Q2a[1] = - Q2.y;
		Q2a[2] = - Q2.z;
		Q2a[3] = - Q2.w;
		}
	else
		{
		Q2a[0] = Q2.x;
		Q2a[1] = Q2.y;
		Q2a[2] = Q2.z;
		Q2a[3] = Q2.w;
		}

	if ((1.0f - cosom) > 0.0001f)	// Delta below only linear interpolation is done
		{
		// standard case (slerp)
		omega = (float)acos(cosom);
		sinom = (float)sin(omega);
		scale0 = (float)sin((1.0f - t) * omega) / sinom;
		scale1 = (float)sin(t * omega) / sinom;
		}
	else
		{        
		// "from" and "to" quaternions are very close 
		//  ... so we can do a linear interpolation
		scale0 = 1.0f - t;
		scale1 = t;
		}

	// calculate final values
	x = scale0 * Q1.x + scale1 * Q2a[0];
	y = scale0 * Q1.y + scale1 * Q2a[1];
	z = scale0 * Q1.z + scale1 * Q2a[2];
	w = scale0 * Q1.w + scale1 * Q2a[3];
	return *this;
	}



/////////////////////////////////////////////////////////////////////////////
// CNEMatrix3 - 3D matrix

CNEMatrix3::CNEMatrix3(
	float sm00, float sm10, float sm20,
	float sm01, float sm11, float sm21,
	float sm02, float sm12, float sm22)
	{
	m00=sm00; m10=sm10; m20=sm20;
	m01=sm01; m11=sm11; m21=sm21;
	m02=sm02; m12=sm12; m22=sm22;
	}


CNEMatrix3::CNEMatrix3(const CNEEuler& Degs)
	{
	float xsi,xco,ysi,yco,zsi,zco;

	xsi=(float)sin(Degs.x); xco=(float)cos(Degs.x);
	ysi=(float)sin(Degs.y); yco=(float)cos(Degs.y);
	zsi=(float)sin(Degs.z); zco=(float)cos(Degs.z);

	if (Degs.m_zyx)
		{
		m00=yco*zco;
		m10=yco*zsi;
		m20=-ysi;
		m01=xsi*ysi*zco-xco*zsi;
		m11=xsi*ysi*zsi+xco*zco;
		m21=xsi*yco;
		m02=xco*ysi*zco+xsi*zsi;
		m12=xco*ysi*zsi-xsi*zco;
		m22=xco*yco;
		}
	else
		{
		m00=yco*zco;
		m10=xco*zsi+xsi*ysi*zco;
		m20=xsi*zsi-xco*ysi*zco;
		m01=-yco*zsi;
		m11=xco*zco-xsi*ysi*zsi;
		m21=xsi*zco+xco*ysi*zsi;
		m02=ysi;
		m12=-xsi*yco;
		m22=xco*yco;
		}
	}


CNEMatrix3::CNEMatrix3(const CNEVector3& Axis, float Deg)
	{	
	float s,c;
	float t;
	float txy,txz,tyz,sx,sy,sz;

	s=(float)sin(Deg);
	c=(float)cos(Deg);
	t=1.0f-c;
	txy=t*Axis.x*Axis.y;
	txz=t*Axis.x*Axis.z;
	tyz=t*Axis.y*Axis.z;
	sx=s*Axis.x;
	sy=s*Axis.y;
	sz=s*Axis.z;
	m00=t*Axis.x*Axis.x+c;
	m10=txy+sz;
	m20=txz-sy;
	m01=txy-sz;
	m11=t*Axis.y*Axis.y+c;
	m21=tyz+sx;
	m02=txz+sy;
	m12=tyz-sx;
	m22=t*Axis.z*Axis.z+c;
	}


CNEMatrix3::CNEMatrix3(const CNEQuaternion& Quat)
	{
	float wx, wy, wz, xx, yy, yz, xy, xz, zz, x2, y2, z2;

	x2 = Quat.x + Quat.x;
	y2 = Quat.y + Quat.y; 
	z2 = Quat.z + Quat.z;
	xx = Quat.x * x2;   xy = Quat.x * y2;   xz = Quat.x * z2;
	yy = Quat.y * y2;   yz = Quat.y * z2;   zz = Quat.z * z2;
	wx = Quat.w * x2;   wy = Quat.w * y2;   wz = Quat.w * z2;

	m00 = 1.0f - (yy + zz);
	m01 = xy - wz;
	m02 = xz + wy;
 
	m10 = xy + wz;
	m11 = 1.0f - (xx + zz);
	m12 = yz - wx;

	m20 = xz - wy;
	m21 = yz + wx;
	m22 = 1.0f - (xx + yy);
	}


CNEMatrix3& CNEMatrix3::Norm(void)
	{
	CNEMatrix3 x;

	x.m00=m00; x.m10=m01; x.m20=m02;
	x.m01=m10; x.m11=m11; x.m21=m12;
	x.m02=m20; x.m12=m21; x.m22=m22;
	x.MulL(*this);

	x.m00=(3.0f-x.m00)/2.0f;
	x.m10=-x.m10/2.0f;
	x.m20=-x.m20/2.0f;
	x.m01=-x.m01/2.0f;
	x.m11=(3.0f-x.m11)/2.0f;
	x.m21=-x.m21/2.0f;
	x.m02=-x.m02/2.0f;
	x.m12=-x.m12/2.0f;
	x.m22=(3.0f-x.m22)/2.0f;
	MulL(x);
	return *this;
	}


CNEMatrix3& CNEMatrix3::Invert(void)
	{
    float buff;

    buff=m01; m01=m10; m10=buff;
    buff=m02; m02=m20; m20=buff;
    buff=m12; m12=m21; m21=buff;
	return *this;
	}


CNEMatrix3& CNEMatrix3::MulR(const CNEMatrix3& Mat)
	{
	float z11,z21,z31,z12,z22,z32,z13,z23,z33;

	z11=Mat.m00*m00 + Mat.m01*m10 + Mat.m02*m20;
	z21=Mat.m10*m00 + Mat.m11*m10 + Mat.m12*m20;
	z31=Mat.m20*m00 + Mat.m21*m10 + Mat.m22*m20;
	z12=Mat.m00*m01 + Mat.m01*m11 + Mat.m02*m21;
	z22=Mat.m10*m01 + Mat.m11*m11 + Mat.m12*m21;
	z32=Mat.m20*m01 + Mat.m21*m11 + Mat.m22*m21;
	z13=Mat.m00*m02 + Mat.m01*m12 + Mat.m02*m22;
	z23=Mat.m10*m02 + Mat.m11*m12 + Mat.m12*m22;
	z33=Mat.m20*m02 + Mat.m21*m12 + Mat.m22*m22;
	m00=z11; m10=z21; m20=z31;
	m01=z12; m11=z22; m21=z32;
	m02=z13; m12=z23; m22=z33;
	return *this;
	}


CNEMatrix3& CNEMatrix3::MulInvR(const CNEMatrix3& Mat)
	{
	float z11,z21,z31,z12,z22,z32,z13,z23,z33;

	z11=Mat.m00*m00 + Mat.m10*m10 + Mat.m20*m20;
	z21=Mat.m01*m00 + Mat.m11*m10 + Mat.m21*m20;
	z31=Mat.m02*m00 + Mat.m12*m10 + Mat.m22*m20;
	z12=Mat.m00*m01 + Mat.m10*m11 + Mat.m20*m21;
	z22=Mat.m01*m01 + Mat.m11*m11 + Mat.m21*m21;
	z32=Mat.m02*m01 + Mat.m12*m11 + Mat.m22*m21;
	z13=Mat.m00*m02 + Mat.m10*m12 + Mat.m20*m22;
	z23=Mat.m01*m02 + Mat.m11*m12 + Mat.m21*m22;
	z33=Mat.m02*m02 + Mat.m12*m12 + Mat.m22*m22;
	m00=z11; m10=z21; m20=z31;
	m01=z12; m11=z22; m21=z32;
	m02=z13; m12=z23; m22=z33;
	return *this;
	}


CNEMatrix3& CNEMatrix3::MulL(const CNEMatrix3& Mat)
	{
	float z11,z21,z31,z12,z22,z32,z13,z23,z33;

	z11=m00*Mat.m00 + m01*Mat.m10 + m02*Mat.m20;
	z21=m10*Mat.m00 + m11*Mat.m10 + m12*Mat.m20;
	z31=m20*Mat.m00 + m21*Mat.m10 + m22*Mat.m20;
	z12=m00*Mat.m01 + m01*Mat.m11 + m02*Mat.m21;
	z22=m10*Mat.m01 + m11*Mat.m11 + m12*Mat.m21;
	z32=m20*Mat.m01 + m21*Mat.m11 + m22*Mat.m21;
	z13=m00*Mat.m02 + m01*Mat.m12 + m02*Mat.m22;
	z23=m10*Mat.m02 + m11*Mat.m12 + m12*Mat.m22;
	z33=m20*Mat.m02 + m21*Mat.m12 + m22*Mat.m22;
	m00=z11; m10=z21; m20=z31;
	m01=z12; m11=z22; m21=z32;
	m02=z13; m12=z23; m22=z33;
	return *this;
	}


CNEMatrix3& CNEMatrix3::MulInvL(const CNEMatrix3& Mat)
	{
	float z11,z21,z31,z12,z22,z32,z13,z23,z33;

	z11=m00*Mat.m00 + m01*Mat.m01 + m02*Mat.m02;
	z21=m10*Mat.m00 + m11*Mat.m01 + m12*Mat.m02;
	z31=m20*Mat.m00 + m21*Mat.m01 + m22*Mat.m02;
	z12=m00*Mat.m10 + m01*Mat.m11 + m02*Mat.m12;
	z22=m10*Mat.m10 + m11*Mat.m11 + m12*Mat.m12;
	z32=m20*Mat.m10 + m21*Mat.m11 + m22*Mat.m12;
	z13=m00*Mat.m20 + m01*Mat.m21 + m02*Mat.m22;
	z23=m10*Mat.m20 + m11*Mat.m21 + m12*Mat.m22;
	z33=m20*Mat.m20 + m21*Mat.m21 + m22*Mat.m22;
	m00=z11; m10=z21; m20=z31;
	m01=z12; m11=z22; m21=z32;
	m02=z13; m12=z23; m22=z33;
	return *this;
	}


// Construct matrix that points towards a direction and an up-vector
CNEMatrix3& CNEMatrix3::LookAt(const CNEVector3& Dir, const CNEVector3& UpVec)
	{
	CNEVector3& Right=operator [](0);
	CNEVector3& Up=operator [](1);
	CNEVector3& Forw=operator [](2);

	Forw.Set(Dir).Norm();
	Right.Set(Forw).Cross(UpVec).Norm();
	Up.Set(Right).Cross(Forw).Norm();
	return *this;
	}


// Point towards a direction while automatically selecting a suitable up-vector
CNEMatrix3& CNEMatrix3::LookAt(const CNEVector3& Dir)
	{
	CNEVector3& Right=operator [](0);
	CNEVector3& Up=operator [](1);
	CNEVector3& Forw=operator [](2);

	Forw.Set(Dir).Norm();

 	// Select positive y axis or positive z alternatively as up-vector
	CNEVector3 UpVec(false);
	if (fabs(Forw.y) > 0.8f)
		UpVec.Set(0.0f, 0.0f, 1.0f);
	else
		UpVec.Set(0.0f, 1.0f, 0.0f);

	Right.Set(Forw).Cross(UpVec).Norm();
	Up.Set(Right).Cross(Forw).Norm();
	return *this;
	}
	

/////////////////////////////////////////////////////////////////////////////
// CNEMatrix3 - 4D matrix

CNEMatrix4::CNEMatrix4(
	float sm00, float sm10, float sm20, float sm30,
	float sm01, float sm11, float sm21, float sm31,
	float sm02, float sm12, float sm22, float sm32,
	float sm03, float sm13, float sm23, float sm33)
	{
	m00=sm00; m10=sm10; m20=sm20; m30=sm30;
	m01=sm01; m11=sm11; m21=sm21; m31=sm31;
	m02=sm02; m12=sm12; m22=sm22; m32=sm32;
	m03=sm03; m13=sm13; m23=sm23; m33=sm33;
	}


CNEMatrix4::CNEMatrix4(const CNEPosition& Pos)
	{
	m00=Pos.m_Rot.m00*Pos.m_Scale.x;
	m10=Pos.m_Rot.m10*Pos.m_Scale.y;
	m20=Pos.m_Rot.m20*Pos.m_Scale.z;
	m30=Pos.m_Center.x;

	m01=Pos.m_Rot.m01*Pos.m_Scale.x;
	m11=Pos.m_Rot.m11*Pos.m_Scale.y;
	m21=Pos.m_Rot.m21*Pos.m_Scale.z;
	m31=Pos.m_Center.y;

	m02=Pos.m_Rot.m02*Pos.m_Scale.x;
	m12=Pos.m_Rot.m12*Pos.m_Scale.y;
	m22=Pos.m_Rot.m22*Pos.m_Scale.z;
	m32=Pos.m_Center.z;

	m03=0;
	m13=0;
	m23=0;
	m33=1;
	}



/////////////////////////////////////////////////////////////////////////////
// CNEPosition - 3D position containing center and rotation

CNEPosition& CNEPosition::Invert()
	{
	m_Scale.InvertMul();
	m_Rot.Invert();
	m_Center.Mul(m_Scale).Rot(m_Rot).InvertAdd();
	return *this;
	}


CNEPosition& CNEPosition::AccumulateR(const CNEPosition& Pos)
	{
	CNEVector3 v=Pos.m_Center;
	m_Center.Add(v.Mul(m_Scale).Rot(m_Rot));
	m_Rot.MulR(Pos.m_Rot);
	m_Scale.Mul(Pos.m_Scale);
	return *this;
	}


CNEPosition& CNEPosition::AccumulateInvR(const CNEPosition& Pos)
	{
	m_Scale.Div(Pos.m_Scale);
	m_Rot.MulInvR(Pos.m_Rot);
	CNEVector3 v=Pos.m_Center;
	m_Center.Sub(v.Mul(m_Scale).Rot(m_Rot));
	return *this;
	}


CNEPosition& CNEPosition::AccumulateL(const CNEPosition& Pos)
	{
	m_Center.Mul(Pos.m_Scale).Rot(Pos.m_Rot).Add(Pos.m_Center);
	m_Rot.MulL(Pos.m_Rot);
	m_Scale.Mul(Pos.m_Scale);
	return *this;
	}


CNEPosition& CNEPosition::AccumulateInvL(const CNEPosition& Pos)
	{
	m_Scale.Div(Pos.m_Scale);
	m_Rot.MulInvL(Pos.m_Rot);
	m_Center.Sub(Pos.m_Center).RotInv(Pos.m_Rot).Div(Pos.m_Scale);
	return *this;
	}


CNEPosition& CNEPosition::AccumulateR(const CNEVector3& Vec)
	{
	CNEVector3 v=Vec;
	m_Center.Add(v.Mul(m_Scale).Rot(m_Rot));
	return *this;
	}


CNEPosition& CNEPosition::AccumulateInvR(const CNEVector3& Vec)
	{
	CNEVector3 v=Vec;
	m_Center.Sub(v.Mul(m_Scale).Rot(m_Rot));
	return *this;
	}


CNEPosition& CNEPosition::AccumulateL(const CNEVector3& Vec)
	{
	m_Center.Add(Vec);
	return *this;
	}

CNEPosition& CNEPosition::AccumulateInvL(const CNEVector3& Vec)
	{
	m_Center.Sub(Vec);
	return *this;
	}



/////////////////////////////////////////////////////////////////////////////
// CNEBoundBox - 3D bounding box

CNEBoundBox& CNEBoundBox::Grow(const CNEVector3& Pt)
	{
	if (Pt.x>m_Max.x) m_Max.x=Pt.x;
	if (Pt.y>m_Max.y) m_Max.y=Pt.y;
	if (Pt.z>m_Max.z) m_Max.z=Pt.z;
	if (Pt.x<m_Min.x) m_Min.x=Pt.x;
	if (Pt.y<m_Min.y) m_Min.y=Pt.y;
	if (Pt.z<m_Min.z) m_Min.z=Pt.z;
	return *this;
	}


CNEBoundBox& CNEBoundBox::Inflate(float Err)
	{
	if (IsEmpty()) return *this;
	m_Min.x-=Err;
	m_Min.y-=Err;
	m_Min.z-=Err;
	m_Max.x+=Err;
	m_Max.y+=Err;
	m_Max.z+=Err;
	return *this;
	}



/////////////////////////////////////////////////////////////////////////////
// CNEColor - RGBA color

// Load with ABGR color stored to an ulong value
CNEColor& CNEColor::SetARGB(ulong argb)
	{
	b=(argb & 0x000000ff)/255.0f;
	g=(argb & 0x0000ff00)/65280.0f;
	r=(argb & 0x00ff0000)/16711680.0f;
	a=(argb & 0xff000000)/4278190080.0f;
	return *this;
	}


// Calculate ABGR color to an ulong value
ulong CNEColor::GetARGB() const
	{
	long _r=(long)(r*255);
	if (_r>255) _r=255;
	if (_r<0) _r=0;
	long _g=(long)(g*255);
	if (_g>255) _g=255;
	if (_g<0) _g=0;
	long _b=(long)(b*255);
	if (_b>255) _b=255;
	if (_b<0) _b=0;
	long _a=(long)(a*255);
	if (_a>255) _a=255;
	if (_a<0) _a=0;
	return ((ulong)_a<<24)|((ulong)_r<<16)|((ulong)_g<<8)|((ulong)_b<<0);
	}



