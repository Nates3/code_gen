/* date = July 7th 2022 7:59 pm */

#ifndef BASE_MATH_H
#define BASE_MATH_H

#include "math.h"

typedef union V2F32
{
  F32 e[2];
  struct
  {
    F32 x, y;
  };
} V2F32;

// NOTE(nates): Maybe rework vector type
typedef union V2S32
{
  S32 e[2];
  struct
  {
    S32 x, y;
  };
} V2S32;

typedef union V3F32
{
  F32 E[3];
  struct
  {
    F32 x, y, z;
  };
  struct
  {
    F32 r, g, b;
  };
  struct
  {
    V2F32 xy;
    F32 IGNOREDZ_0;
  };
  struct
  {
    F32 IGNORED_X;
    F32 yz;
  };
}V3F32;

typedef union V4F32
{
  F32 e[4];
  union
  {
    struct
    {
      F32 x, y, z, w;
    };
    
    struct
    {
      V3F32 xyz;
      F32 IGNORED_0;
    };
  };
  
  union
  {
    struct
    {
      F32 r, g, b, a;
    };
    
    struct
    {
      V3F32 rgb;
      F32 IGNORED_1;
    };
  };
} V4F32;

typedef union Rng1F32
{
  F32 e[2];
  F32 min, max;
}Rng1F32;

typedef union Rng1S32
{
  S32 e[2];
  S32 min, max;
}Rng1S32;

typedef union Rng1U64
{
  U64 e[2];
  U64 min, max;
}Rng1U64;

typedef union Rng2F32
{
  F32 e[4];
  struct 
  {
    V2F32 p0;
    V2F32 p1;
  };
  struct 
  {
    V2F32 min;
    V2F32 max;
  };
  struct
  {
    F32 x0, y0;
    F32 x1, y1;
  };
} Rng2F32;

typedef union Rng2S32
{
  S32 e[4];
  struct
  {
    V2S32 p0;
    V2S32 p1;
  };
  struct
  {
    V2S32 min;
    V2S32 max;
  };
  struct
  {
    S32 x0, y0;
    S32 x1, y1;
  };
} Rng2S32;

typedef union Rng3F32
{
  F32 e[6];
  struct
  {
    V3F32 min;
    V3F32 max;
  };
  struct
  {
    F32 x0, y0, z0;
    F32 x1, y1, z1;
  };
} Rng3F32;

typedef struct M44
{
  F32 e[4][4];
} M44;

typedef union Qur
{
  struct
  {
    F32 x, y, z, w;
  };
  
  struct
  {
    V3F32 xyz;
    F32 IGNORED_0;
  };
} Qur;

///////////////////////////
// NOTE(nates): Integer math functions

func_ S16 AbsValueS16(S16 Value);
func_ U64 U64EncodeFromS64(S64 number);
func_ S64 S64DecodeFromU64(U64 number);

///////////////////////////
// NOTE(nates): Real math functions

func_ S32 RoundF32ToS32(F32 Value);
func_ U32 RoundF32ToU32(F32 Value);
func_ S32 FloorF32ToS32(F32 Value);
func_ U32 FloorF32ToU32(F32 Value);
func_ S32 CeilF32ToS32(F32 Value);
func_ U32 CeilF32Tou32(F32 Value);
func_ F32 Square(F32 Value);
func_ F32 SafeRatioN(F32 Numerator, F32 Divisor, F32 N);
func_ F32 SafeRatio0(F32 Numerator, F32 Divisor);
func_ F32 SafeRatio1(F32 Numerator, F32 Divisor);
func_ F32 F32PosInf(void);
func_ F32 F32NegInf(void);
func_ F32 F32NegZero(void);
func_ F32 AbsValue(F32 Value);
func_ F32 Sqrt(F32 Value);
func_ F32 Sin(F32 Value);
func_ F32 Cos(F32 Value);
func_ F32 Tan(F32 Value);
func_ F32 Lerp(F32 A, F32 t, F32 B);
func_ F32 FindT(F32 Start, F32 Between, F32 End);

// F64

func_ S64 RoundF64ToS64(F64 Value);
func_ U64 RoundF64ToU64(F64 Value);
func_ S64 FloorF64ToS64(F64 Value);
func_ U64 FloorF64ToU64(F64 Value);
func_ S64 CeilF64ToS64(F64 Value);
func_ U64 CeilF64ToU64(F64 Value);
func_ F64 SquareF64(F64 Value);
func_ F64 SafeRatioNF64(F64 Numerator, F64 Divisor, F64 N);
func_ F64 SafeRatio0F64(F64 Numerator, F64 Divisor);
func_ F64 SafeRatio1F64(F64 Numerator, F64 Divisor);
func_ F64 F64PosInf(void);
func_ F64 F64NegInf(void);
func_ F64 F64NegZero(void);
func_ F64 AbsValueF64(F64 Value);
func_ F64 SqrtF64(F64 Value);
func_ F64 SinF64(F64 Value);
func_ F64 CosF64(F64 Value);
func_ F64 TanF64(F64 Value);
func_ F64 LerpF64(F64 A, F64 t, F64 B);
func_ F64 FindTF64(F64 Start, F64 Between, F64 End);

/////////////////////////////
//~ NOTE(nates): Range functions

func_ Rng1F32 R1F32(F32 x, F32 y);
func_ Rng1S32 R1S32(S32 x, S32 y);
func_ Rng1U64 R1U64(U64 x, U64 y);

func_ Rng2F32 R2F32(F32 x0, F32 y0, F32 x1, F32 y1);
func_ Rng2S32 R2S32(S32 x0, S32 y0, S32 x1, S32 y1);
func_ Rng2F32 R2F32R(Rng1F32 a, Rng1F32 b);
func_ Rng2F32 R2F32V(V2F32 a, V2F32 b);

func_ Rng3F32 R3F32(F32 x0, F32 y0, F32 z0, F32 x1, F32 y1, F32 z1);
func_ Rng3F32 R3F32R(Rng1F32 a, Rng1F32 b, Rng1F32 c);
func_ Rng3F32 R3F32V(V3F32 a, V3F32 b, V3F32 c);

/////////////////////////////
// NOTE(nates): V2F32 Math Functions

func_ V2F32 V2(F32 x, F32 y);
func_ V2F32 V2a(F32 S);
func_ V2F32 V2s(S32 X, S32 Y);
func_ V2F32 V2u(U32 X, U32 Y);
func_ V2F32 V2Neg(V2F32 A);
func_ V2F32 V2Add(V2F32 A, V2F32 B);
func_ V2F32 V2Sub(V2F32 A, V2F32 B);
func_ V2F32 V2MS(V2F32 A, F32 B);
func_ V2F32 V2DS(V2F32 A, F32 B);
func_ F32 V2Dot(V2F32 A, V2F32 B);
func_ V2F32 V2Had(V2F32 A, V2F32 B);
func_ F32 V2LenSq(V2F32 A);
func_ F32 V2Len(V2F32 A);
func_ V2F32 V2Norm(V2F32 A);
func_ V2F32 V2Perp(V2F32 A);
func_ V2F32 V2Lerp(V2F32 A, F32 t, V2F32 B);
func_ V2F32 V2TriProd(V2F32 A2, V2F32 B2);

///////////////////////////////
//NOTE(nates): V3F32 Math Functions

func_ V3F32 V3(F32 x, F32 y, F32 z);
func_ V3F32 V3a(F32 S);
func_ V3F32 V3xy(V2F32 XY, F32 z);
func_ V3F32 V3yz(F32 x, V2F32 YZ);
func_ V3F32 V3Neg(V3F32 A);
func_ V3F32 V3Add(V3F32 A, V3F32 B);
func_ V3F32 V3Sub(V3F32 A, V3F32 B);
func_ V3F32 V3MS(V3F32 A, F32 B);
func_ V3F32 V3DS(V3F32 A, F32 B);
func_ F32 V3Dot(V3F32 A, V3F32 B);
func_ V3F32 V3Had(V3F32 A, V3F32 B);
func_ F32 V3Len(V3F32 A);
func_ F32 V3LenSq(V3F32 A);
func_ V3F32 V3Norm(V3F32 A);
func_ V3F32 V3Cross(V3F32 A, V3F32 B);
func_ V3F32 V3Lerp(V3F32 A, F32 t, V3F32 B);
func_ F32 PlaneIntersectT(V3F32 PlaneP, V3F32 PlaneNormal, V3F32 LineStart, V3F32 LineEnd);
func_ V3F32 PlaneIntersection(V3F32 PlaneP, V3F32 PlaneNormal, V3F32 LineStart, V3F32 LineEnd);
func_ V3F32 V3AARot(V3F32 Point, V3F32 Axis, F32 Angle);


////////////////////////////////
// NOTE(nates): V4 Math Functions

func_ V4F32 V4(F32 x, F32 y, F32 z, F32 w);
func_ V4F32 V4a(F32 A);
func_ V4F32 V4xyz(V3F32 XYZ, F32 w);
func_ V4F32 V4Add(V4F32 A, V4F32 B);
func_ V4F32 V4Sub(V4F32 A, V4F32 B);
func_ V4F32 V4MS(V4F32 A, F32 B);
func_ V4F32 V4DS(V4F32 A, F32 B);
func_ F32 V4Dot(V4F32 A, V4F32 B);
func_ F32 V4Len(V4F32 A);
func_ F32 V4LenSq(V4F32 A);

///////////////////////////////
// NOTE(nates): M44 functions

func_ M44 M44Mul(M44 A, M44 B);
func_ M44 M44Transpose(M44 A);
func_ M44 M44Proj(F32 ScreenHeightInMeters, F32 ScreenWidthInMeters, F32 FocalLength);
func_ V4F32  M44MulV4F32(M44 A, V4F32 B);
func_ V3F32  M44MulV3F32(M44 A, V3F32 B);
func_ M44 M44Col3x3(V3F32 X, V3F32 Y, V3F32 Z);
func_ M44 M44Row3x3(V3F32 X, V3F32 Y, V3F32 Z);
func_ V3F32  M44Col3(M44 A, U32 ColIndex);
func_ V3F32  M44Row3(M44 A, U32 RowIndex);
func_ M44 M44Translation(M44 A, V3F32 T);
func_ M44 M44Identity(void);
func_ M44 M44XRotation(F32 Angle);
func_ M44 M44YRotation(F32 Angle);
func_ M44 M44ZRotation(F32 Angle);

#endif //BASE_MATH_H
