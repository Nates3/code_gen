
#include "base_math.h"

/////////////////////////////////////
// NOTE(nates): Integer math functions


func_ S16
AbsValueS16(S16 Value)
{
  if(Value < 0)
  {
    Value *= -1;
  }
  S16 Result = Value;
  return(Result);
}

func_ U64
U64EncodeFromS64(S64 number)
{
  // NOTE(nates): rotleft(x, 1)
  U64 result = (((U64)number) << 1) | (((U64)number) >> 63);
  return(result);
}

func_ S64
S64DecodeFromU64(U64 number)
{
  S64 result = (S64)((number >> 1) | (number << 63));
  return(result);
}

///////////////////////////////////
// NOTE(nates): Real Math Functions


func_ S32
RoundF32ToS32(F32 Value)
{
  S32 Result = (S32)roundf(Value);
  return(Result);
}

func_ U32
RoundF32ToU32(F32 Value)
{
  U32 Result = (U32)roundf(Value);
  return(Result);
}

func_ S32
FloorF32ToS32(F32 Value)
{
  S32 Result = (S32)floorf(Value);
  return(Result);
}

func_ U32
FloorF32ToU32(F32 Value)
{
  U32 Result = (U32)floorf(Value);
  return(Result);
}

func_ S32
CeilF32ToS32(F32 Value)
{
  S32 Result = (S32)ceilf(Value);
  return(Result);
}

func_ U32
CeilF32Tou32(F32 Value)
{
  U32 Result = (U32)ceilf(Value);
  return(Result);
}

func_ F32
Square(F32 Value)
{
  F32 Result = Value*Value;
  return(Result);
}

func_ F32
SafeRatioN(F32 Numerator, F32 Divisor, F32 N)
{
  F32 Result = N;
  if(Divisor != 0.0f)
  {
    Result = Numerator / Divisor;
  }
  return(Result);
}

func_ F32
SafeRatio0(F32 Numerator, F32 Divisor)
{
  F32 Result = SafeRatioN(Numerator, Divisor, 0.0f);
  
  return(Result);
}

func_ F32
SafeRatio1(F32 Numerator, F32 Divisor)
{
  F32 Result = SafeRatioN(Numerator, Divisor, 1.0f);
  return(Result);
}

func_ F32
F32PosInf(void)
{
  typedef union { F32 f; U32 u; } un;
  
  un r = {0};
  r.u = 0x7f800000;
  return(r.f);
}

func_ F32
F32NegInf(void)
{
  typedef union { F32 f; U32 u; } un;
  
  un r = {0};
  r.u = 0xff800000;
  return(r.f);
}

func_ F32
F32NegZero(void)
{
  typedef union { F32 f; U32 u; } un;
  
  un r = {0};
  r.u = 0x80000000;
  return(r.f);
}


func_ F32 
AbsValue(F32 Value)
{
  typedef union { F32 f; U32 u; } un;
  un r;
  r.f = Value;
  r.u &= 0x7fffffff;
  return(r.f);
}

func_ F32 
Sqrt(F32 Value)
{
  F32 Result = sqrtf(Value);
  return(Result);
}

func_ F32
Sin(F32 Value)
{
  F32 Result = sinf(Value);
  return(Result);
}

func_ F32
Cos(F32 Value)
{
  F32 Result = cosf(Value);
  return(Result);
}

func_ F32
Tan(F32 Value)
{ 
  F32 Result = tanf(Value);
  return(Result);
}

func_ F32
Lerp(F32 A, F32 t, F32 B)
{
  F32 Result = (1.0f - t)*A + B;
  return(Result);
}

func_ F32
FindT(F32 Start, F32 Between, F32 End)
{
  F32 t = 0.0f;
  if(Start != End)
  {
    t = (Between - Start) / (End - Start);
  }
  
  return(t);
}

////////////////////
// NOTE(nates): F64 Math Functions

func_ S64
RoundF64ToS64(F64 Value)
{
  S64 Result = (S64)round(Value);
  return(Result);
}

func_ U64
RoundF64ToU64(F64 Value)
{
  U64 Result = (U64)round(Value);
  return(Result);
}

func_ S64
FloorF64ToS64(F64 Value)
{
  S64 Result = (S64)floor(Value);
  return(Result);
}

func_ U64
FloorF64ToU64(F64 Value)
{
  U64 Result = (U64)floor(Value);
  return(Result);
}

func_ S64
CeilF64ToS64(F64 Value)
{
  S64 Result = (S64)ceil(Value);
  return(Result);
}

func_ U64
CeilF64ToU64(F64 Value)
{
  U64 Result = (U64)ceil(Value);
  return(Result);
}

func_ F64
SquareF64(F64 Value)
{
  F64 Result = Value*Value;
  return(Result);
}

func_ F64
SafeRatioNF64(F64 Numerator, F64 Divisor, F64 N)
{
  F64 Result = N;
  if(Divisor != 0.0)
  {
    Result = Numerator / Divisor;
  }
  return(Result);
}

func_ F64
SafeRatio0F64(F64 Numerator, F64 Divisor)
{
  F64 Result = SafeRatioNR64(Numerator, Divisor, 0.0);
  
  return(Result);
}

func_ F64
SafeRatio1F64(F64 Numerator, F64 Divisor)
{
  F64 Result = SafeRatioNR64(Numerator, Divisor, 1.0);
  return(Result);
}

func_ F64
F64PosInf(void)
{
  typedef union { F64 f; U64 u; } un;
  
  un r = {0};
  r.u = 0x7f800000;
  return(r.f);
}

func_ F64
F64NegInf(void)
{
  typedef union { F64 f; U64 u; } un;
  
  un r = {0};
  r.u = 0xff800000;
  return(r.f);
}

func_ F64
F64NegZero(void)
{
  typedef union { F64 f; U64 u; } un;
  
  un r = {0};
  r.u = 0x80000000;
  return(r.f);
}

func_ F64 
AbsValueF64(F64 Value)
{
  typedef union { F64 f; U64 u; } un;
  un r;
  r.f = Value;
  r.u &= 0x7fffffff;
  return(r.f);
}

func_ F64 
SqrtF64(F64 Value)
{
  F64 Result = sqrt(Value);
  return(Result);
}

func_ F64
SinF64(F64 Value)
{
  F64 Result = sin(Value);
  return(Result);
}

func_ F64
CosF64(F64 Value)
{
  F64 Result = cos(Value);
  return(Result);
}

func_ F64
TanF64(F64 Value)
{ 
  F64 Result = tan(Value);
  return(Result);
}

func_ F64
LerpF64(F64 A, F64 t, F64 B)
{
  F64 Result = (1.0 - t)*A + B;
  return(Result);
}

func_ F64
FindTF64(F64 Start, F64 Between, F64 End)
{
  F64 t = 0.0;
  if(Start != End)
  {
    t = (Between - Start) / (End - Start);
  }
  
  return(t);
}

/////////////////////////////////////////
//~ NOTE(nates): Range functions

func_ Rng1F32 R1F32(F32 x, F32 y)
{
  Rng1F32 result = {x, y};
  return(result);
}

func_ Rng1S32 R1S32(S32 x, S32 y)
{
  Rng1S32 result = {x, y};
  return(result);
}
func_ Rng1U64 R1U64(U64 x, U64 y)
{
  Rng1U64 result = {x, y};
  return(result);
}

func_ Rng2F32 R2F32(F32 x0, F32 y0, F32 x1, F32 y1)
{
  Rng2F32 result = {x0, y0, x1, y1};
  return(result);
}

func_ Rng2S32 R2S32(S32 x0, S32 y0, S32 x1, S32 y1)
{
  Rng2S32 result = {x0, y0, x1, y1};
  return(result);
}
func_ Rng2F32 R2F32R(Rng1F32 a, Rng1F32 b)
{
  Rng2F32 result = {a.min, b.min, a.max, b.max};
  return(result);
}

func_ Rng2F32 R2F32V(V2F32 a, V2F32 b)
{
  Rng2F32 result = {a.x, b.x, a.y, b.y};
  return(result);
}

func_ Rng3F32 R3F32(F32 x0, F32 y0, F32 z0, F32 x1, F32 y1, F32 z1)
{
  Rng3F32 result = {x0, y0, z0, x1, y1, z1};
  return(result);
}

func_ Rng3F32 R3F32R(Rng1F32 a, Rng1F32 b, Rng1F32 c)
{
  Rng3F32 result = {a.min, b.min, c.min, a.max, b.max, c.max};
  return(result);
}
func_ Rng3F32 R3F32V(V3F32 a, V3F32 b, V3F32 c)
{
  Rng3F32 result = {a.x, b.x, c.x, a.y, b.y, c.y};
  return(result);
}



////////////////////////
//~ NOTE(nates): Vector2!

func_ V2F32
V2(F32 x, F32 y)
{
  V2F32 Result = {x, y};
  return(Result);
}

func_ V2F32
V2a(F32 S)
{
  V2F32 Result = {
    .x = S,
    .y = S,
  };
  
  return(Result);
}

func_ V2F32
V2s(S32 X, S32 Y)
{
  V2F32 Result = V2((F32)X, (F32)Y);
  return(Result);
}

func_ V2F32
V2u(U32 X, U32 Y)
{
  V2F32 Result;
  Result = V2((F32)X, (F32)Y);
  return(Result);
}

func_ V2F32 
V2Neg(V2F32 A)
{
  V2F32 Result = {
    .x = -A.x,
    .y = -A.y,
  };
  return(Result);
}

func_ V2F32
V2Add(V2F32 A, V2F32 B)
{
  V2F32 Result;
  Result.x = A.x + B.x;
  Result.y = A.y + B.y;
  
  return(Result);
}

func_ V2F32
V2Sub(V2F32 A, V2F32 B)
{
  V2F32 Result;
  Result.x = A.x - B.x;
  Result.y = A.y - B.y;
  return(Result);
}

func_ V2F32
V2MS(V2F32 A, F32 B)
{
  V2F32 Result = {
    .x = A.x*B,
    .y = A.y*B,
  };
  return(Result);
}

func_ V2F32
V2DS(V2F32 A, F32 B)
{
  V2F32 Result = {
    .x = A.x/B,
    .y = A.y/B,
  };
  return(Result);
}

func_ F32
V2Dot(V2F32 A, V2F32 B)
{
  F32 Result = A.x*B.x + A.y*B.y;
  return(Result);
}

func_ V2F32
V2Had(V2F32 A, V2F32 B)
{
  V2F32 Result = {A.x*B.x, A.y*B.y};
  return(Result);
}

func_ F32
V2LenSq(V2F32 A)
{
  F32 Result = V2F32Dot(A, A);
  return(Result);
}

func_ F32
V2Len(V2F32 A)
{
  F32 Result = Sqrt(V2F32Dot(A, A));
  return(Result);
}

func_ V2F32
V2Norm(V2F32 A)
{
  V2F32 Result = {0};
  if(V2F32LenSq(A) != 0.0f)
  {
    Result = V2DS(A, V2Len(A));
  }
  
  return(Result);
}

func_ V2F32
V2Perp(V2F32 A)
{
  V2F32 Result;
  Result.x = -A.y;
  Result.y = A.x;
  return(Result);
}

func_ V2F32
V2Lerp(V2F32 A, F32 t, V2F32 B)
{
  V2F32 Result = V2Add(V2MS(A, (1.0f - t)),
                       V2MS(B, t));
  return(Result);
}

func_ V2F32
V2F32TriProd(V2F32 A2, V2F32 B2)
{
  V3F32 A = V3xy(A2, 0.0f);
  V3F32 B = V3xy(B2, 0.0f);
  V3F32 Cross = V3Cross(A, B);
  V2F32 Result = V3Cross(Cross, A).xy;
  return(Result);
}

/////////////////////////
// NOTE(nates): V2F32S32

func_ V2S32
V2S(S32 x, S32 y)
{
  V2S32 result = {x, y};
  return(result);
}

func_ V2S32
V2S32Add(V2S32 a, V2S32 b)
{
  V2S32 result = {
    a.x + b.x,
    a.y + b.y,
  };
  return(result);
}

func_ V2S32
V2S32Sub(V2S32 a, V2S32 b)
{
  V2S32 result = {
    a.x - b.x,
    a.y - b.y,
  };
  return(result);
}

///////////////////////
// NOTE(nates): Vtor3!

func_ V3F32
V3(F32 x, F32 y, F32 z)
{
  V3F32 Result = {x, y, z};
  return(Result);
}

func_ V3F32
V3a(F32 S)
{
  V3F32 Result = {S, S, S};
  return(Result);
}

func_ V3F32
V3xy(V2F32 XY, F32 z)
{
  V3F32 Result;
  Result.x = XY.x;
  Result.y = XY.y;
  Result.z = z;
  
  return(Result);
}

func_ V3F32
V3yz(F32 x, V2F32 YZ)
{
  V3F32 Result;
  Result.x = x;
  Result.y = YZ.x;
  Result.z = YZ.y;
  return(Result);
}

func_ V3F32 
V3Neg(V3F32 A)
{
  V3F32 Result = {
    .x = -A.x,
    .y = -A.y,
    .z = -A.z,
  };
  return(Result);
}

func_ V3F32
V3Add(V3F32 A, V3F32 B)
{
  V3F32 Result;
  Result.x = A.x + B.x;
  Result.y = A.y + B.y;
  Result.z = A.z + B.z;
  
  return(Result);
}

func_ V3F32
V3Sub(V3F32 A, V3F32 B)
{
  V3F32 Result;
  Result.x = A.x - B.x;
  Result.y = A.y - B.y;
  Result.z = A.z - B.z;
  
  return(Result);
}

func_ V3F32
V3MS(V3F32 A, F32 B)
{
  V3F32 Result = {
    .x = A.x*B,
    .y = A.y*B,
    .z = A.z*B,
  };
  
  return(Result);
}

func_ V3F32
V3DS(V3F32 A, F32 B)
{
  V3F32 Result = {
    .x = A.x/B,
    .y = A.y/B,
    .z = A.z/B,
  };
  return(Result);
}

func_ F32
V3Dot(V3F32 A, V3F32 B)
{
  F32 Result = A.x*B.x + A.y*B.y + A.z*B.z;
  return(Result);
}


func_ V3F32
V3Had(V3F32 A, V3F32 B)
{
  V3F32 Result = {A.x*B.x, A.y*B.y, A.z*B.z};
  return(Result);
}

func_ F32
V3Len(V3F32 A)
{
  F32 Result = Sqrt(V3F32Dot(A, A));
  return(Result);
}

func_ F32
V3F32LenSq(V3F32 A)
{
  F32 Result = V3F32Dot(A, A);
  return(Result);
}

func_ V3F32
V3Norm(V3F32 A)
{
  V3F32 Result = {0};
  if(V3F32LenSq(A) != 0.0f)
  {
    Result = V3DS(A, V3Len(A));
  }
  
  return(Result);
}

func_ V3F32
V3Cross(V3F32 A, V3F32 B)
{
  V3F32 Result;
  Result.x = A.y*B.z - A.z*B.y;
  Result.y = A.z*B.x - A.x*B.z;
  Result.z = A.x*B.y - A.y*B.x;
  return(Result);
}

func_ V3F32
V3Lerp(V3F32 A, F32 t, V3F32 B)
{
  V3F32 Result = V3Add(V3MS(A, (1.0f - t)),
                       V3MS(B, t));
  return(Result);
}

func_ F32
PlaneIntersectT(V3F32 PlaneP, V3F32 PlaneNormal, V3F32 LineStart, V3F32 LineEnd)
{
  Assert(V3F32Len(PlaneNormal) <= 1.01f);
  F32 PlaneProjection = V3Dot(PlaneNormal, PlaneP);
  F32 StartProjection = V3Dot(LineStart, PlaneNormal);
  F32 EndProjection = V3Dot(LineEnd, PlaneNormal);
  F32 t = (PlaneProjection - StartProjection) / (EndProjection - StartProjection);
  return(t);
}

func_ V3F32
PlaneIntersection(V3F32 PlaneP, V3F32 PlaneNormal, V3F32 LineStart, V3F32 LineEnd)
{
  // NOTE(nates): Finds intersection point between line and a plane
  Assert(V3F32Len(PlaneNormal) <= 1.01f);
  F32 PlaneProjection = V3Dot(PlaneNormal, PlaneP);
  F32 StartProjection = V3Dot(LineStart, PlaneNormal);
  F32 EndProjection = V3Dot(LineEnd, PlaneNormal);
  F32 t = (PlaneProjection - StartProjection) / (EndProjection - StartProjection);
  V3F32 Result = V3Add(LineStart, V3MS(V3Sub(LineEnd, LineStart), t));
  return(Result);
}

func_ V3F32
V3AARot(V3F32 Point, V3F32 axis, F32 Angle)
{
  Angle = Angle*DegreesToRadians;
  
  F32 Proj = V3Dot(Point, axis);
  V3F32 p = V3MS(axis, Proj);
  V3F32 e = V3Sub(Point, p);
  V3F32 f = V3Cross(axis, Point);
  V3F32 ep = V3Add(V3MS(e,Cos(Angle)), V3MS(f, Sin(Angle)));
  
  V3F32 Result = V3Add(p,ep);
  return(Result);
}

//////////////////////////////
// NOTE(nates): Vtor 4!!

func_ V4F32
V4(F32 x, F32 y, F32 z, F32 w)
{
  V4F32 Result = {x, y, z, w};
  return(Result);
}

func_ V4F32
V4a(F32 S)
{
  V4F32 Result = 
  {
    .x = S, .y = S, .z = S, .w = S,
  };
  return(Result);
}

func_ V4F32
V4xyz(V3F32 XYZ, F32 w)
{
  V4F32 Result;
  Result.x = XYZ.x;
  Result.y = XYZ.y;
  Result.z = XYZ.z;
  Result.w = w;
  
  return(Result);
}


func_ V4F32
V4Add(V4F32 A, V4F32 B)
{
  V4F32 Result;
  Result.x = A.x + B.x;
  Result.y = A.y + B.y;
  Result.z = A.z + B.z;
  Result.w = A.w + B.w;
  
  return(Result);
}

func_ V4F32
V4Sub(V4F32 A, V4F32 B)
{
  V4F32 Result;
  Result.x = A.x - B.x;
  Result.y = A.y - B.y;
  Result.z = A.z - B.z;
  Result.w = A.w - B.w;
  
  return(Result);
}

func_ V4F32
V4MS(V4F32 A, F32 B)
{
  V4F32 Result = {
    .x = A.x*B,
    .y = A.y*B,
    .z = A.z*B,
    .w = A.w*B,
  };
  return(Result);
}

func_ V4F32
V4DS(V4F32 A, F32 B)
{
  V4F32 Result = {
    .x = A.x/B,
    .y = A.y/B,
    .z = A.z/B,
    .w = A.w/B,
  };
  return(Result);
}

func_ F32
V4Dot(V4F32 A, V4F32 B)
{
  F32 Result = A.x*B.x + A.y*B.y + A.z*B.z + A.w*B.w;
  return(Result);
}

func_ F32
V4Len(V4F32 A)
{
  F32 Result = Sqrt(V4F32Dot(A, A));
  return(Result);
}

func_ F32
V4LenSq(V4F32 A)
{
  F32 Result = V4F32Dot(A, A);
  return(Result);
}


//////////////////////////////////
// NOTE(nates): M44


func_ M44
M44Mul(M44 A, M44 B)
{
  M44 Result = {0};
  
  for(U32 r = 0;
      r < 4;
      ++r)
  {
    for(U32 c = 0;
        c < 4;
        ++c)
    {
      for(U32 i = 0;
          i < 4;
          ++i)
      {
        Result.e[r][c] += A.e[r][i]*B.e[i][c];
      }
    }
  }
  
  return(Result);
}

func_ M44
M44Transpose(M44 A)
{
  M44 Result;
  for(U32 r = 0;
      r < 4;
      ++r)
  {
    for(U32 c = 0;
        c < 4;
        ++c)
    {
      Result.e[r][c] = A.e[c][r];
    }
  }
  
  return(Result);
}

func_ M44
M44Proj(F32 ScreenHeightInMeters, F32 ScreenWidthInMeters, F32 FocalLength)
{
  // NOTE(nates): Aspect ratio is height / width
  
  F32 a = (1.0f / ScreenWidthInMeters*0.5f);
  F32 b = (1.0f / ScreenHeightInMeters*0.5f);
  F32 ar = (ScreenHeightInMeters / ScreenWidthInMeters);
  F32 c = -FocalLength;
  M44 Result = {
    {
      {a*c,   0.0f,  0.0f,  0.0f},
      {0.0f,  ar*ar*b*c,   0.0f,  0.0f},
      {0.0f,  0.0f,  1.0f,  0.0f},
      {0.0f,  0.0f,  0.0f,  0.0f},
    },
  };
  
  return(Result);
}

func_ V4F32
M44MulV4F32(M44 A, V4F32 B)
{
  V4F32 Result;
  Result.x = B.x*A.e[0][0] + B.y*A.e[0][1] + B.z*A.e[0][2] + B.w*A.e[0][3];
  Result.y = B.x*A.e[1][0] + B.y*A.e[1][1] + B.z*A.e[1][2] + B.w*A.e[1][3];
  Result.z = B.x*A.e[2][0] + B.y*A.e[2][1] + B.z*A.e[2][2] + B.w*A.e[2][3];
  Result.w = B.x*A.e[3][0] + B.y*A.e[3][1] + B.z*A.e[3][2] + B.w*A.e[3][3];
  
  return(Result);
}

func_ V3F32
M44MulV3F32(M44 A, V3F32 B)
{
  V4F32 HomogeneousCoord = V4xyz(B, 1.0f);
  V3F32 Result = M44MulV4F32(A, HomogeneousCoord).xyz;
  return(Result);
}

func_ M44
M44Col3x3(V3F32 X, V3F32 Y, V3F32 Z)
{
  M44 Result = {
    {
      {X.x,  Y.x,  Z.x,  0.0f},
      {X.y,  Y.y,  Z.y,  0.0f},
      {X.z,  Y.z,  Z.z,  0.0f},
      {0.0f, 0.0f, 0.0f, 1.0f},
    },
  };
  return(Result);
}

func_ M44
M44Row3x3(V3F32 X, V3F32 Y, V3F32 Z)
{
  M44 Result = {
    {
      {X.x,  X.y,  X.z,  0.0f},
      {Y.x,  Y.y,  Y.z,  0.0f},
      {Z.x,  Z.y,  Z.z,  0.0f},
      {0.0f, 0.0f, 0.0f, 1.0f},
    },
  };
  
  return(Result);
}


func_ V3F32  
M44Col3(M44 A, U32 ColIndex)
{
  V3F32 Result = {
    A.e[0][ColIndex],
    A.e[1][ColIndex],
    A.e[2][ColIndex],
  };
  
  return(Result);
}

func_ V3F32  
M44Row3(M44 A, U32 RowIndex)
{
  V3F32 Result = {
    A.e[RowIndex][0],
    A.e[RowIndex][1],
    A.e[RowIndex][2],
  };
  
  return(Result);
}

func_ M44
M44Translation(M44 A, V3F32 T)
{
  M44 R = A;
  R.e[0][3] += T.x;
  R.e[1][3] += T.y;
  R.e[2][3] += T.z;
  return(R);
}

func_ M44
M44Identity(void)
{
  M44 Result = {
    {
      {1.0f, 0.0f, 0.0f, 0.0f},
      {0.0f, 1.0f, 0.0f, 0.0f},
      {0.0f, 0.0f, 1.0f, 0.0f},
      {0.0f, 0.0f, 0.0f, 1.0f},
    },
  };
  
  return(Result);
}

func_ M44
M44XRotation(F32 Angle)
{
  Angle = Angle*DegreesToRadians;
  
  F32 s = Sin(Angle);
  F32 c = Cos(Angle);
  
  M44 Result = {
    {
      {1.0f, 0.0f, 0.0f, 0.0f},
      {0.0f,  c,   -s,   0.0f},
      {0.0f,  s,   c,    0.0f},
      {0.0f, 0.0f, 0.0f, 1.0f},
    },
  };
  
  return(Result);
}

func_ M44
M44YRotation(F32 Angle)
{
  Angle = Angle*DegreesToRadians;
  F32 s = Sin(Angle);
  F32 c = Cos(Angle);
  
  M44 Result = {
    {
      {c,    0.0f, s,    0.0f},
      {0.0f, 1.0f, 0.0f, 0.0f},
      {-s,   0.0f, c,    0.0f},
      {0.0f, 0.0f, 0.0f, 1.0f},
    },
  };
  
  return(Result);
}

func_ M44
M44ZRotation(F32 Angle)
{
  Angle = Angle*DegreesToRadians;
  F32 s = Sin(Angle);
  F32 c = Cos(Angle);
  
  M44 Result = {
    {
      {c,    -s,   0.0f, 0.0f},
      {s,     c,   0.0f, 0.0f},
      {0.0f, 0.0f, 1.0f, 0.0f},
      {0.0f, 0.0f, 0.0f, 1.0f},
    },
  };
  
  return(Result);
}
