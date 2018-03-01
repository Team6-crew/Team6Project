#ifdef PSTATION4
#pragma once

#include <gnmx.h>
#include <graphics\api_gnm\toolkit\geommath\geommath.h>
#include <nclgl\Matrix4.h>
#include <nclgl\Vector2.h>
// Converts an nclgl matrix to a PS4 matrix. 
sce::Vectormath::Scalar::Aos::Matrix4 nclToPS4(const nclgl::Maths::Matrix4& mat);

// Converts  a PS4 matrix to an nclgl matrix
nclgl::Maths::Matrix4 PS4ToNcl(const sce::Vectormath::Scalar::Aos::Matrix4& mat);

// Converts an nclgl Vector4 to a PS4 Vector4. 
sce::Vectormath::Scalar::Aos::Vector4 nclToPS4(const nclgl::Maths::Vector4& vec);

// Converts  a PS4 Vector4 to an nclgl Vector4
nclgl::Maths::Vector4 PS4ToNcl(const sce::Vectormath::Scalar::Aos::Vector4& vec);

// Converts an nclgl Vector3 to a PS4 Vector3. 
sce::Vectormath::Scalar::Aos::Vector3 nclToPS4(const nclgl::Maths::Vector3& vec);

// Converts  a PS4 Vector3 to an nclgl Vector3
nclgl::Maths::Vector3 PS4ToNcl(const sce::Vectormath::Scalar::Aos::Vector3& mat);

// Converts an nclgl Vector2 to a PS4 Vector2. 
sce::Vectormath::Scalar::Aos::Vector2 nclToPS4(const nclgl::Maths::Vector2& vec);


sce::Vectormath::Scalar::Aos::Vector3 CrossPS4(const sce::Vectormath::Scalar::Aos::Vector3 &a, const sce::Vectormath::Scalar::Aos::Vector3 &b) {
	return sce::Vectormath::Scalar::Aos::Vector3((a.getY()*b.getZ()) - (a.getZ()*b.getY()), (a.getZ()*b.getX()) - (a.getX()*b.getZ()), (a.getX()*b.getY()) - (a.getY()*b.getX()));
}


const sce::Vectormath::Scalar::Aos::Vector3& NormalisePS4(sce::Vectormath::Scalar::Aos::Vector3 &vec) {
	float length = sqrt(vec.getX() * vec.getX() + vec.getY() * vec.getY() + vec.getZ() * vec.getZ());

	if (length != 0.0f) {
		length = 1.0f / length;
		vec.setX(vec.getX() * length);
		vec.setY(vec.getY() * length);
		vec.setZ(vec.getZ() * length);
	}
}
#endif