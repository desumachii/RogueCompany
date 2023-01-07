#pragma once

struct Vector3 {
	float x, y, z;
};

// Generated using ReClassEx

class GWorld;
class UWorld;
class ULevel;
class UGameInstance;
class ULocalPlayers;
class ULocalPlayer;
class UGameViewportClient;
class APlayerController;
class APawn;
class APlayerState;
class ACharacter;
class USkeletalMeshComponent;
class USceneComponent;
class APlayerController;
class APawn;
class USceneComponent;
class APlayerState;


struct FLinearColor
{
	float R;
	float G;
	float B;
	float A;
};


struct FVector {

	float X;
	float Y;
	float Z;
	FVector() : X(0.f), Y(0.f), Z(0.f) {}

	FVector(float x, float y, float z) : X(x), Y(y), Z(z) {}

	FVector(float InF) : X(InF), Y(InF), Z(InF) { }


	inline float Dot(const FVector& b) const { return (X * b.X) + (Y * b.Y) + (Z * b.Z); }

	inline float MagnitudeSqr() const { return Dot(*this); }

	inline float Magnitude() const { return std::sqrtf(MagnitudeSqr()); }

	float Size() const { return sqrtf(X * X + Y * Y + Z * Z); }

	float Sum() const { return X + Y + Z; }

	float Size2D() const { return sqrtf(X * X + Y * Y); }

	float SizeSquared() const { return X * X + Y * Y + Z * Z; }

	float DistTo(const FVector& V) const { return (*this - V).Size(); }

	FVector operator+(const FVector& other) const { return FVector(X + other.X, Y + other.Y, Z + other.Z); }

	FVector operator-(const FVector& other) const { return FVector(X - other.X, Y - other.Y, Z - other.Z); }

	FVector operator*(const FVector& V) const { return FVector(X * V.X, Y * V.Y, Z * V.Z); }

	FVector operator/(const FVector& V) const { return FVector(X / V.X, Y / V.Y, Z / V.Z); }

	bool operator==(const FVector& V) const { return X == V.X && Y == V.Y && Z == V.Z; }

	bool operator!=(const FVector& V) const { return X != V.X || Y != V.Y || Z != V.Z; }

	FVector operator-() const { return FVector(-X, -Y, -Z); }

	FVector operator+(float Bias) const { return FVector(X + Bias, Y + Bias, Z + Bias); }

	FVector operator-(float Bias) const { return FVector(X - Bias, Y - Bias, Z - Bias); }

	FVector operator*(float Scale) const { return FVector(X * Scale, Y * Scale, Z * Scale); } const

	FVector operator/(float Scale) const { const float RScale = 1.f / Scale; return FVector(X * RScale, Y * RScale, Z * RScale); }

	FVector operator=(const FVector& V) { X = V.X; Y = V.Y; Z = V.Z; return *this; }

	FVector operator+=(const FVector& V) { X += V.X; Y += V.Y; Z += V.Z; return *this; }

	FVector operator-=(const FVector& V) { X -= V.X; Y -= V.Y; Z -= V.Z; return *this; }

	FVector operator*=(const FVector& V) { X *= V.X; Y *= V.Y; Z *= V.Z; return *this; }

	FVector operator/=(const FVector& V) { X /= V.X; Y /= V.Y; Z /= V.Z; return *this; }

	FVector operator*=(float Scale) { X *= Scale; Y *= Scale; Z *= Scale; return *this; }

	FVector operator/=(float V) { const float RV = 1.f / V; X *= RV; Y *= RV; Z *= RV; return *this; }

	float operator|(const FVector& V) const { return X * V.X + Y * V.Y + Z * V.Z; }

	FVector operator^(const FVector& V) const { return FVector(Y * V.Z - Z * V.Y, Z * V.X - X * V.Z, X * V.Y - Y * V.X); }
	inline float Distance(FVector v) {
		return float(sqrtf(powf(v.X - X, 2.0) + powf(v.Y - Y, 2.0) + powf(v.Z - Z, 2.0)));
	}

	//__forceinline operator FVector2D() const { return { X, Y }; }
};


struct FVector2D
{
	float X;
	float Y;
	//FVector2D(float x, float y) { X = x; Y = y; };

	__forceinline void Normalize()
	{
		// pitch
		if (X > 90.f)
			X = 90.f;
		else if (X < -90.f)
			X = -90.f;

		// convert to Sword With Sauce's angle system
		if (X < 0.f)
			X += 360.f;
		Y = fmodf(Y + 360.f, 360.f);
	}
	__forceinline float Distance(const FVector2D b) const { return sqrtf((X - b.X) * (X - b.X) + (Y - b.Y) * (Y- b.Y)); }
	__forceinline operator FVector() const { return { X, Y, 0.f }; }

};

template<class T>

struct TArray
{
	friend struct FString;

public:
	inline int Num() const
	{
		return Count;
	};

	inline T& operator[](int i)
	{
		return Data[i];
	};

	inline bool IsValidIndex(int i) const
	{
		return i < Num();
	}

private:
	T* Data;
	int32_t Count;
	int32_t Max;
};

struct FString : private TArray<wchar_t>
{
	inline FString()
	{
	};

	FString(const wchar_t* other)
	{
		Max = Count = *other ? std::wcslen(other) + 1 : 0;

		if (Count)
		{
			Data = const_cast<wchar_t*>(other);
		}
	};

	inline bool IsValid() const
	{
		return Data != nullptr;
	}

	inline const wchar_t* c_str() const
	{
		return Data;
	}

	std::string ToString() const
	{
		auto length = std::wcslen(Data);

		std::string str(length, '\0');

		std::use_facet<std::ctype<wchar_t>>(std::locale()).narrow(Data, Data + length, '?', &str[0]);

		return str;
	}
};

class Matrix {
public:
	float m[4][4];
};
inline UObject* WolrdtoScreen;
inline UObject* Font;
inline UObject* Draw_Box;
inline UObject* Draw_Line;
inline UObject* Draw_Text;
inline UObject* Actor_Location;
inline UObject* Actor_Bounds;
inline UObject* Actor_IsEnemy;
inline UObject* Actor_GetVelocity;
inline UObject* _LineOfSightTo; //Function Engine.Controller.LineOfSightTo
inline UObject* Mesh_GetBoneName;
inline UObject* Mesh_GetBoneNums;
inline UObject* Mesh_GetBoneIndex;

inline UObject* _GetControlRotation;
inline UObject* _SetControlRotation;

inline UObject* _GetViewportSize;
inline UObject* CameraManager_GetCameraLocation;
inline UObject* CameraManager_GetCameraRotation;

inline uintptr_t Mesh_GetBoneMatirx;

struct Canvas : UObject {
	inline void K2_DrawLine(FVector2D ScreenPositionA, FVector2D ScreenPositionB, FLOAT Thickness, FLinearColor Color);
	inline void K2_DrawText(FString RenderText, FVector2D ScreenPosition, FVector2D Scale, FLinearColor RenderColor, float Kerning, FLinearColor ShadowColor, FVector2D ShadowOffset, bool bCentreX, bool bCentreY, bool bOutlined, FLinearColor OutlineColor);
};

void Canvas::K2_DrawText(FString RenderText, FVector2D ScreenPosition, FVector2D Scale, FLinearColor RenderColor, float Kerning, FLinearColor ShadowColor, FVector2D ShadowOffset, bool bCentreX, bool bCentreY, bool bOutlined, FLinearColor OutlineColor)
{
	struct {
		UObject* RenderFont; //UFont* 
		FString RenderText;
		FVector2D ScreenPosition;
		FVector2D Scale;
		FLinearColor RenderColor;
		float Kerning;
		FLinearColor ShadowColor;
		FVector2D ShadowOffset;
		bool bCentreX;
		bool bCentreY;
		bool bOutlined;
		FLinearColor OutlineColor;
	} Parameters;
	Parameters.RenderFont = Font;
	Parameters.RenderText = RenderText;
	Parameters.ScreenPosition = ScreenPosition;
	Parameters.Scale = Scale;
	Parameters.RenderColor = RenderColor;
	Parameters.Kerning = Kerning;
	Parameters.ShadowColor = ShadowColor;
	Parameters.ShadowOffset = ShadowOffset;
	Parameters.bCentreX = bCentreX;
	Parameters.bCentreY = bCentreY;
	Parameters.bOutlined = bOutlined;

	ProcessEvent(this, Draw_Text, &Parameters);
}

void Canvas::K2_DrawLine(FVector2D ScreenPositionA, FVector2D ScreenPositionB, FLOAT Thickness, FLinearColor Color)
{
	struct {
		FVector2D ScreenPositionA;
		FVector2D ScreenPositionB;
		FLOAT Thickness;
		FLinearColor Color;
	} Parameters{};

	Parameters.ScreenPositionA = ScreenPositionA;
	Parameters.ScreenPositionB = ScreenPositionB;
	Parameters.Thickness = Thickness;
	Parameters.Color = Color;

	ProcessEvent(this, Draw_Line, &Parameters);
}

enum bones :int32 {

	C_Head = 104,
	C_Neck01 = 102,
	C_Neck02 = 103,


	C_Spine01 = 30,
	C_Spine02 =31,
	C_Spine03 = 32,
	C_Spine04 = 33,


	C_Pelvis = 1,
	R_Thigh = 3,
	R_Knee = 9,
	R_Calf = 4,
	R_Foot = 5,
	R_Toe = 6,
	
	L_Thigh = 14,
	L_Knee = 20,
	L_Calf = 15,
	L_Foot = 16,
	L_Toe = 17,

	L_Clavicle = 34,
	L_UpperArm = 35,
	L_Forearm = 36,
	L_Hand = 37,

	R_Clavicle = 68,
	R_UpperArm = 69,
	R_Forearm = 70,
	R_Hand = 71,
	R_IndexFinger_00 = 74, 
	R_IndexFinger_01 = 75, 
	R_IndexFinger_02 = 76, 
	R_IndexFinger_03 = 77, 
	R_MiddleFinger_00 = 78,
	R_MiddleFinger_01 = 79,
	R_MiddleFinger_02 = 80, 
	R_MiddleFinger_03 = 81, 
	R_RingFinger_00 = 82, 
	R_RingFinger_01 = 83,
	R_RingFinger_02 = 84, 
	R_RingFinger_03 = 85,
	R_PinkyFinger_00 = 86,
	R_PinkyFinger_01 = 87,
	R_PinkyFinger_02 = 88, 
	R_PinkyFinger_03 = 89,
	R_Thumb_01 = 90, 
	R_Thumb_02 = 91, 
	R_Thumb_03 = 92, 
	L_IndexFinger_00 = 40, 
	L_IndexFinger_01 = 41,
	L_IndexFinger_02 = 42, 
	L_IndexFinger_03 = 43,
	L_MiddleFinger_00 = 44, 
	L_MiddleFinger_01 = 45,
	L_MiddleFinger_02 = 46, 
	L_MiddleFinger_03 = 47,
	L_RingFinger_00 = 48, 
	L_RingFinger_01 = 49,
	L_RingFinger_02 = 50, 
	L_RingFinger_03 = 51, 
	L_PinkyFinger_00 = 52,
	L_PinkyFinger_01 = 53,
	L_PinkyFinger_02 = 54, 
	L_PinkyFinger_03 = 55,
	L_Thumb_01 = 56, 
	L_Thumb_02 = 57, 
	L_Thumb_03 = 58

};


class GWorld
{
public:
	UWorld* Wolrd; //0x0000 
	char pad_0x0008[0x38]; //0x0008

}; //Size=0x0040


class UWorld
{
public:
	char pad_0x0000[0x30]; //0x0000
	ULevel* PersistentLevel; //0x0030 
	char pad_0x0038[0x150]; //0x0038
	UGameInstance* OwningGameInstance; //0x0188 
	char pad_0x0190[0x338]; //0x0190

}; //Size=0x04C8

class ULevel
{
public:
	char pad_0x0000[0x98]; //0x0000
	TArray<APawn*> Actors;
	char pad_0x00A8[0x20]; //0x00A8

}; //Size=0x00C8



class UGameInstance
{
public:
	char pad_0x0000[0x38]; //0x0000
	TArray<ULocalPlayer*> LocalPlayers; //0x0038 
	__int32 N00000142; //0x0040 
	__int32 N00000144; //0x0044 

}; //Size=0x0048


class ULocalPlayer
{
public:
	char pad_0x0000[0x30]; //0x0000
	APlayerController* PlayerController; //0x0030 
	char pad_0x0038[0x38]; //0x0038
	UGameViewportClient* ViewportClient; //0x0070 
	char pad_0x0078[0x48]; //0x0078


}; //Size=0x00C0

class 	UGameViewportClient
{
public:
	char pad_0x0000[0x8]; //0x0000

}; //Size=0x0008




class ACharacter
{
public:
	char pad_0x0000[0x280]; //0x0000
	USkeletalMeshComponent* Mesh; //0x0280 
	char pad_0x0288[0x180]; //0x0288

}; //Size=0x0408

class USkeletalMeshComponent
{
public:
	inline FName GetBoneName(int32 index) {
		struct {
			int index;
			FName ret;
		}params{};
		params.index = index;
		ProcessEvent(this, Mesh_GetBoneName, &params);
		return params.ret;
	};

	inline FVector GetBoneMatrix(int32 index) {
		const auto Fn = reinterpret_cast<Matrix * (*)(USkeletalMeshComponent*, Matrix*, int)>(Mesh_GetBoneMatirx);
		Matrix matrix{};
		Fn(this, &matrix, index);
		return FVector{ matrix.m[3][0], matrix.m[3][1], matrix.m[3][2] };
	};

	inline int32 GetBoneNums() {
		struct {
			int32 ret;
		}params{};
		ProcessEvent(this, Mesh_GetBoneNums, &params);
		return params.ret;
	};
	inline int32 GetBoneIndex() {};
}; //Size=0x0588


class APlayerCameraManager
{
public:

 inline	FVector GetCameraLocation() {
	 struct {
		 FVector ret;
	 }params{};
	 ProcessEvent(this, CameraManager_GetCameraLocation, &params);
	 return params.ret;
  }

 inline	FVector GetCameraRotation() {
	 struct {
		 FVector ret;
	 }params{};
	 ProcessEvent(this, CameraManager_GetCameraRotation, &params);
	 return params.ret;
 }
};


class APlayerController
{
public:
	char pad_0x0000[0x2A0]; //0x0000
	APawn* AcknowledgedPawn; //0x02A0 
	char pad_0x02A8[0x10]; //0x02A8
	APlayerCameraManager* PlayerCameraManager;
	inline bool ProjectWorldLocationToScreen(FVector WorldLocation, FVector2D& ScreenLocation, bool bPlayerViewportRelative);

	inline bool LineOfSightTo(APawn* actor, FVector ViewPoint, bool bAlternateChecks) {
		struct
		{
			APawn* Actor;
			FVector ViewPoint;
			bool bAlternateChecks;
			bool Out;
		}Parameters{};
		Parameters.Actor = actor;
		Parameters.ViewPoint = ViewPoint;
		Parameters.bAlternateChecks = bAlternateChecks;
		ProcessEvent(this, _LineOfSightTo, &Parameters);
		return Parameters.Out;
	}

	inline FVector GetControlRotation()
	{
		struct
		{
			FVector Rot;
		}Parameters{};
		ProcessEvent(this, _GetControlRotation, &Parameters);
		return Parameters.Rot;
	}

	inline void SetControlRotation(FVector NewRot)
	{
		struct
		{
			FVector Rot;
		}Parameters{};
		Parameters.Rot = NewRot;
		ProcessEvent(this, _SetControlRotation, &Parameters);
		return;
	}

	inline void GetViewportSize(int32 & ScreenWidth,int32& ScreenHeight) {
		struct 
		{
			int32  ScreenWidth;
			int32  ScreenHeight;
		}Parameters{};
		ProcessEvent(this, _GetViewportSize, &Parameters);
		ScreenHeight = Parameters.ScreenHeight;
		ScreenWidth = Parameters.ScreenWidth;
	}


}; //Size=0x0408

bool APlayerController::ProjectWorldLocationToScreen(FVector WorldLocation, FVector2D& ScreenLocation, bool bPlayerViewportRelative) {

	struct {
		FVector WorldLocation_;
		FVector2D ScreenLocation_;
		bool _bPlayerViewportRelative;
		bool ret;
	} Parameters{};
	Parameters.WorldLocation_ = WorldLocation;
	Parameters.ScreenLocation_ = ScreenLocation;
	Parameters._bPlayerViewportRelative = bPlayerViewportRelative;
	ProcessEvent(this, WolrdtoScreen, &Parameters);
	ScreenLocation = Parameters.ScreenLocation_;
	return Parameters.ret;
}


class APawn : public UObjectBaseUtility
{
public:
	char pad_0x0000[0x108]; //0x0000
	USceneComponent* RootComponent; //0x0130 
	char pad_0x0138[0x108]; //0x0138
	APlayerState* PlayerState; //0x0240 
	char pad_0x0248[0x38]; //0x0248
	USkeletalMeshComponent* Mesh;


	inline FVector GetLocation() {
		struct {
			FVector ReturnValue;
		} params{};

		ProcessEvent(this, Actor_Location, &params);

		return params.ReturnValue;
	}

	inline bool IsPlayer() {
		return GetName().find("Character") != std::string::npos;
	}

	inline bool IsEnemy(APawn* pOther) {
		struct {
			APawn* Other;
			bool ret;
		}params{};
		params.Other = pOther;
		ProcessEvent(this, Actor_IsEnemy, &params);
		return params.ret;
	}

	inline void GetBounds(bool bOnlyCollidingComponents, FVector& Origin, FVector& BoxExtent)
	{
		struct {
			bool bOnlyCollidingComponents;
			FVector Origin;
			FVector BoxExtent;
		}params{};
		params.bOnlyCollidingComponents = bOnlyCollidingComponents;
		ProcessEvent(this, Actor_Bounds,&params);
		Origin = params.Origin;
		BoxExtent = params.BoxExtent;
	}

	inline FVector GetVelocity() {
		struct {
			FVector ret;
		}params{};
		ProcessEvent(this, Actor_GetVelocity, &params);
		return params.ret;
	}
}; //Size=0x0408

class USceneComponent
{
public:
	char pad_0x0000[0x118]; //0x0000
	__int32 N0000070E; //0x0118 
	FVector RelativeLocation; //0x011C 
	char pad_0x0128[0x20]; //0x0128

}; //Size=0x0148

class APlayerState
{
public:
	char pad_0x0000[0x18]; //0x0000
	__int32 N0000071C; //0x0018 
	char pad_0x001C[0x208]; //0x001C
	__int32 PlayerId; //0x0224 
	char pad_0x0228[0xD8]; //0x0228
	wchar_t* PlayerNamePrivate; //0x0300 
	char pad_0x0308[0x180]; //0x0308

}; //Size=0x0488
