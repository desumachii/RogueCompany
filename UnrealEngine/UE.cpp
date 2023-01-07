#pragma once
#include <UnrealEngine/UE.h>

#include<ZeroGui/ZeroInput.h>
#include<ZeroGui/ZeroGui.h>

FNamePool* NamePoolData = nullptr;
FUObjectArray* GUObjectArray = nullptr;
void* EngineVersionString = nullptr;
UWorld* GameWorld;


std::string GetEngineVersion() {
	if (EngineVersionString) {
		WIDECHAR Copy[64];
		memcpy(Copy, EngineVersionString, 64);
		std::wstring WideString(Copy, std::wcslen(Copy));
		return std::string(WideString.begin(), WideString.end());
	}
	else {
		return std::string("Unknown Engine Version");
	}
}

 template <typename T> inline T Read(const uintptr_t address)
{
	T Novalue = {};
	if (!IsBadReadPtr((const void*)address, sizeof(T)))
	{
		return *reinterpret_cast<T*>(address);
	}
	else
	{
		return Novalue;
	}
}

bool Initalize(HMODULE GameModule)
{
	const uint64 BaseAddress = reinterpret_cast<uint64>(GameModule);

	const IMAGE_DOS_HEADER* DOSHeader = reinterpret_cast<IMAGE_DOS_HEADER*>(GameModule);
	const IMAGE_NT_HEADERS* NtHeaders = reinterpret_cast<IMAGE_NT_HEADERS*>(reinterpret_cast<long long>(GameModule) + DOSHeader->e_lfanew);
	
	const DWORD SizeOfImage = NtHeaders->OptionalHeader.SizeOfImage;

	EngineVersionString = PatternScan<void*>("2B ? 2B ? 55 ? 45 ?", BaseAddress, SizeOfImage); 
	if (!EngineVersionString) {
		printf("[FAIL] Engine version was not found by string, you can proceed anyways.\n");
	}
		
	GUObjectArray = PatternScan<FUObjectArray*>("48 8D 0D ? ? ? ? E8 ? ? ? ? 48 8D 8D ? ? ? ? E8 ? ? ? ? 48 8D 8D", BaseAddress, SizeOfImage, true);
	if (!GUObjectArray) {
		printf("[FAIL] GUObjectArray not found.\n");
		return false;
	}

	
	NamePoolData = PatternScan<FNamePool*>("48 8D 0D ? ? ? ? E8 ? ? ? ? C6 05 ? ? ? ? 01 0F 10", BaseAddress, SizeOfImage, true);
	if (!NamePoolData) {
		printf("[FAIL] NamePoolData not found.\n");
		return false;
	}


	WIDECHAR GameFileDir[MAX_PATH];
	GetModuleFileNameW(NULL, GameFileDir, MAX_PATH);

	printf("[INFO] Game Directory -> %ws\n\n", GameFileDir);

	printf("[INFO] Game Module Base Address -> %llX \n\n", BaseAddress);

	printf("[INFO] Size Of Game Module -> 0x%X\n\n", SizeOfImage);

	printf("[INFO] Engine Version: %s\n\n", GetEngineVersion().c_str());

	printf("[INFO] FUObjectArray* GUObjectArray found -> Base Address + 0x%llX\n\n", reinterpret_cast<uint64>(GUObjectArray) - BaseAddress);

	printf("[INFO] FNamePool* NamePoolData found -> Base Address + 0x%llX\n\n", reinterpret_cast<uint64>(NamePoolData) - BaseAddress);

	GameWorld = Read<UWorld*>((uintptr_t)GameModule + 0x71bb918);
	
	WolrdtoScreen = GUObjectArray->ObjObjects.FindObject("Function Engine.PlayerController.ProjectWorldLocationToScreen");

	Font = GUObjectArray->ObjObjects.FindObject("Font Roboto.Roboto");//Font NotoSans.NotoSans Font Industry.Industry  Font Roboto.Roboto //Font Transient.DefaultRegularFont
	Draw_Text = GUObjectArray->ObjObjects.FindObject("Function Engine.Canvas.K2_DrawText");
	Draw_Line = GUObjectArray->ObjObjects.FindObject("Function Engine.Canvas.K2_DrawLine");
	Draw_Box = GUObjectArray->ObjObjects.FindObject("Function Engine.Canvas.K2_DrawBox");
	
	Actor_Location = GUObjectArray->ObjObjects.FindObject("Function Engine.Actor.K2_GetActorLocation");
	Actor_Bounds = GUObjectArray->ObjObjects.FindObject("Function Engine.Actor.GetActorBounds");
	Actor_IsEnemy = GUObjectArray->ObjObjects.FindObject("Function Engine.Actor.IsEnemy");
	Actor_GetVelocity = GUObjectArray->ObjObjects.FindObject("Function Engine.Actor.GetVelocity");

	Mesh_GetBoneName = GUObjectArray->ObjObjects.FindObject("Function Engine.SkinnedMeshComponent.GetBoneName");
	Mesh_GetBoneNums = GUObjectArray->ObjObjects.FindObject("Function Engine.SkinnedMeshComponent.GetNumBones");
	Mesh_GetBoneMatirx = (uintptr_t)GameModule + 0x3F2CBE0;

	_LineOfSightTo = GUObjectArray->ObjObjects.FindObject("Function Engine.Controller.LineOfSightTo");

	_GetControlRotation = GUObjectArray->ObjObjects.FindObject("Function Engine.Controller.GetControlRotation");
	_SetControlRotation = GUObjectArray->ObjObjects.FindObject("Function Engine.Controller.SetControlRotation");

	_GetViewportSize = GUObjectArray->ObjObjects.FindObject("Function Engine.PlayerController.GetViewportSize");
	
	CameraManager_GetCameraLocation = GUObjectArray->ObjObjects.FindObject("Function Engine.PlayerCameraManager.GetCameraLocation");
	CameraManager_GetCameraRotation = GUObjectArray->ObjObjects.FindObject("Function Engine.PlayerCameraManager.GetCameraRotation");


	SwapVmt(GameWorld->OwningGameInstance->LocalPlayers[0]->ViewportClient,97,
		reinterpret_cast<void*>(&PostRender),reinterpret_cast<void**>(&OPostRender));
	return true;
}

FVector2D pos;

static bool  bDrawBone, bDrawBox, bHealth, bAimBot, bDistance, bVisible, bDrawLine, bSlient, bDrawName;


void DrawBox(Canvas* can, FVector2D& topleft, FVector2D& downright, FLinearColor clr)
{
	auto h = downright.Y - topleft.Y;
	auto w = downright.X - topleft.X;

	auto downleft = FVector2D{ topleft.X, downright.Y };
	auto topright = FVector2D{ downright.X, topleft.Y };

	auto thicc = 1.0F;

	//FLinearColor xclr = RGBtoFLC(0, 0, 0);
	
	can->K2_DrawLine(topleft, { downright.X, topleft.Y }, thicc, clr);
	can->K2_DrawLine(topleft, { topleft.X , downright.Y }, thicc, clr);
	can->K2_DrawLine(downright, { topleft.X , downright.Y }, thicc, clr);
	can->K2_DrawLine(downright, { downright.X, topleft.Y }, thicc, clr);

}

void DrawBones(APlayerController* player_controller, USkeletalMeshComponent* mesh, FLinearColor color)
{
	std::vector<std::vector<bones>> _boneVector =
	{
		{ bones::C_Neck01, bones::C_Spine04,bones::C_Spine03,bones::C_Spine02,::C_Spine01, bones::C_Pelvis},// bones::PELVIS },
		{ bones::C_Neck01, bones::L_Clavicle,bones::L_UpperArm, bones::L_Forearm,bones::L_Hand},

	    /*{bones::L_Hand, bones::L_IndexFinger_00,bones::L_IndexFinger_01,bones::L_IndexFinger_02,bones::L_IndexFinger_03},
		{ bones::L_Hand, bones::L_MiddleFinger_00,bones::L_MiddleFinger_01,bones::L_MiddleFinger_02,bones::L_MiddleFinger_03},
		{ bones::L_Hand, bones::L_RingFinger_00,bones::L_RingFinger_01,bones::L_RingFinger_02,bones::L_RingFinger_03},
		{ bones::L_Hand, bones::L_PinkyFinger_00,bones::L_PinkyFinger_01,bones::L_PinkyFinger_02,bones::L_PinkyFinger_03},
		{ bones::L_Hand,bones::L_Thumb_01,bones::L_Thumb_02,bones::L_Thumb_03},*/


		{ bones::C_Neck01, bones::R_Clavicle,bones::R_UpperArm, bones::R_Forearm,bones::R_Hand},
		/*{bones::R_Hand, bones::R_IndexFinger_00,bones::R_IndexFinger_01,bones::R_IndexFinger_02,bones::R_IndexFinger_03},
		{ bones::R_Hand, bones::R_MiddleFinger_00,bones::R_MiddleFinger_01,bones::R_MiddleFinger_02,bones::R_MiddleFinger_03},
		{ bones::R_Hand, bones::R_RingFinger_00,bones::R_RingFinger_01,bones::R_RingFinger_02,bones::R_RingFinger_03},
		{ bones::R_Hand, bones::R_PinkyFinger_00,bones::R_PinkyFinger_01,bones::R_PinkyFinger_02,bones::R_PinkyFinger_03},
		{ bones::R_Hand,bones::R_Thumb_01,bones::R_Thumb_02,bones::R_Thumb_03},*/

		{ bones::C_Pelvis, bones::R_Thigh, bones::R_Calf,bones::R_Foot ,bones::R_Toe},
		{ bones::C_Pelvis, bones::L_Thigh, bones::L_Calf,bones::L_Foot ,bones::L_Toe},

	};

	for (const std::vector<bones>& part : _boneVector)
	{
		FVector2D previousBone{};
		for (const bones& bone : part)
		{
			FVector2D bonePos = {};

		    if(!player_controller->ProjectWorldLocationToScreen(mesh->GetBoneMatrix(bone), bonePos, false))continue;
			if (player_controller->LineOfSightTo(player_controller->AcknowledgedPawn, mesh->GetBoneMatrix(bone), false))color = { 1.f, 1.f, 0.f ,1.0f };
			else  color = { 1.f, 1.f, 1.f,1.0f };
			if (bonePos.X == 0.0f && bonePos.Y == 0.0f)
				break;
			if (previousBone.X == 0.0f) // first bone no draw happens
			{
				previousBone = bonePos;
				continue;
			}
			ZeroGUI::PostRenderer::Draw_Line(FVector2D{ previousBone.X, previousBone.Y }, FVector2D{ bonePos.X, bonePos.Y }, 2.0F, color);
			previousBone = bonePos;
		}
	}
}

__forceinline float SmoothOutYaw(float targetYaw, float currentYaw, float smoothness)
{
	if (targetYaw > 0.f && currentYaw < 0.f)
	{
		float dist = 180.f - targetYaw + 180.f + currentYaw;
		if (dist < 180.f)
			return currentYaw - dist / smoothness;
	}
	else if (currentYaw > 0.f && targetYaw < 0.f)
	{
		float dist = 180.f - currentYaw + 180.f + targetYaw;
		if (dist < 180.f)
			return currentYaw + dist / smoothness;
	}

	return currentYaw + (targetYaw - currentYaw) / smoothness;
}

#define M_PI   3.14159265358979323846264338327950288419716939937510


FVector2D CalcAngle(const FVector& src, const FVector& dst, const FVector2D currentAngles, float smoothness)
{
	FVector delta = dst - src;

	float dist = sqrtf(delta.X * delta.X + delta.Y * delta.Y + delta.Z * delta.Z);
	float pitch = (-((acosf((delta.Z / dist)) * 180.0f / M_PI) - 90.0f));
	float yaw = atan2f(delta.Y, delta.X) * (180.0f / M_PI);

	FVector2D ret{};
	ret.X = currentAngles.X + (pitch - currentAngles.X) / smoothness;
	ret.Y = SmoothOutYaw(yaw, currentAngles.Y, smoothness);
	ret.Normalize(); // makes the angles work for Sword With Sauce's angle system

	return ret;
}




static FLinearColor Color = { 1.f, 1.f, 1.f };
static int key;

float kekw = 4.0f;

int32 ScreenWidth, ScreenHeight;
/*
APawn* GetClosestEnemy(FVector& outLocation, int& outId)
{

	APawn* ret = nullptr;
	float closestDist = INFINITY;

	for (USHORT i = 0; i < EntityList->count; i++)
	{
		auto& ent = EntityList->items[i];
		if (!ent->IsPlayer() || (cfg->aimbot->bVisibleOnly && !ent->IsVisible()))
			continue;

		FVector target = ent->GetBonePos(BONE_HEAD_ID);

		FVector2D screenPos;
		//if (!WorldToScreen(target, screenPos))
		//	continue;

		//float dist = winCenter.Distance(screenPos);
		//if (cfg->aimbot->bFOV && dist > cfg->aimbot->FOV)
		//	continue;

		if (dist < closestDist)
		{
			closestDist = dist;
			outLocation = target;
			outId = ent->ObjectIndex;
			ret = ent;
		}
	}

	return ret;
}

*/

FVector2D winCenter{};
static int FOV = 150;

//static APawn* TargetEnemy = nullptr;


static APawn* TargetEnemy = nullptr;

void tick(Canvas* canvas) {
	if(!GameWorld->OwningGameInstance)return;
	if (!GameWorld->OwningGameInstance->LocalPlayers[0])return;
	auto playercontroller = GameWorld->OwningGameInstance->LocalPlayers[0]->PlayerController;
	if (!playercontroller)return;
	auto localplayer = playercontroller->AcknowledgedPawn;
	if (!localplayer)return;
	auto actors = GameWorld->PersistentLevel->Actors;
	static float closestDistance = INFINITY;
	TargetEnemy = nullptr;
	auto mesh = localplayer->Mesh;
	playercontroller->GetViewportSize(ScreenWidth, ScreenHeight);
	winCenter = { ScreenWidth / 2.f, ScreenHeight / 2.f };
	ZeroGUI::DrawCircle(winCenter, FOV, 100, { 1.0,1.0,1.0,1.0 });

	auto AimAtPos = [&](const FVector& worldLocation)
	{
		// implementing my velocity, distance to enemy as well as the smoothness in the calculations for a more accurate aimbot
		FVector src = playercontroller->PlayerCameraManager->GetCameraLocation()
			+ (localplayer->GetVelocity()
				* playercontroller->PlayerCameraManager->GetCameraLocation().Distance(worldLocation)
				/ 49650.f
				* 1.f);

		FVector angle = CalcAngle(
			src,							// source world location
			worldLocation,					// target world location
			{ playercontroller->PlayerCameraManager->GetCameraRotation().X,
			playercontroller->PlayerCameraManager->GetCameraRotation().Y },// current angles/rotation
			1.0F);		// smoothness
		playercontroller->SetControlRotation(angle);
	};

	for (int i = 0; i < actors.Num(); i++) {
		auto player = actors[i];
		if (!player)continue;
		if (player == localplayer)continue;
		if (player->IsPlayer())
		{
			if (localplayer->IsEnemy(player))Color = { 0,1,0,1 };

			if (bDrawBox) {
				FVector Origin, Extend;
				player->GetBounds(1, Origin, Extend);
				auto location = player->GetLocation(); FVector2D footPos;
				if (!playercontroller->ProjectWorldLocationToScreen({ location.X, location.Y, location.Z + (Extend.Z / 2) }, footPos, 0)) continue;
				footPos.Y -= 10.0f;
				FVector2D headPos;
				if (!playercontroller->ProjectWorldLocationToScreen({ location.X, location.Y, location.Z - (Extend.Z / 2) }, headPos, 0)) continue;
				headPos.Y += 10.0f;
				const float height = abs(footPos.Y - headPos.Y);
				const float width = height * 0.4f;
				FVector2D top = { headPos.X - width * 0.5f, headPos.Y };
				FVector2D bottom = { headPos.X + width * 0.5f, footPos.Y };
				DrawBox(canvas, top, bottom, Color);
			}
			FVector target = player->GetLocation();
			FVector2D screenPos;
			if(!playercontroller->ProjectWorldLocationToScreen(target, screenPos, false))continue;
			float dist = winCenter.Distance(screenPos);
			if (dist < closestDistance)
			{
				closestDistance = dist;
				TargetEnemy = player;
			}

			if (bDrawBone)DrawBones(playercontroller, player->Mesh, Color);
		}

		if (bAimBot)
		{
			if (TargetEnemy)
			{
				FVector TargetLocation = TargetEnemy->Mesh->GetBoneMatrix(bones::C_Head);
				if (bVisible)
				{
					if (playercontroller->LineOfSightTo(TargetEnemy, { 0,0,0 }, false))
					{

						if (GetAsyncKeyState(key))
					    	AimAtPos(TargetLocation);
					}
				}
				else
				{

					if (GetAsyncKeyState(key))
						AimAtPos(TargetLocation);
				}
			}
		}
	}
}



inline void PostRender(UGameViewportClient* GameViewportClient, Canvas* canvas)
{
	if(!canvas)	OPostRender(GameViewportClient, canvas);

	ZeroGUI::SetupCanvas(canvas);
	ZeroGUI::Input::Handle();

	static bool menu_opened = true;
	static float m_fov = 1.f;
	if (GetAsyncKeyState(VK_INSERT) & 1) menu_opened = !menu_opened; //Our menu key
	if (ZeroGUI::Window((char*)"Rogue Company", &pos, FVector2D{ 500.0f, 380.0f }, menu_opened)) {

		static int tab = 0;

		if (ZeroGUI::ButtonTab((char*)"Visuals", FVector2D{ 100.0f, 30.0f }, tab == 0))tab = 0;
		if (ZeroGUI::ButtonTab((char*)"Aimbot", FVector2D{ 100.0f, 30.0f }, tab == 1))tab = 1;
		if (ZeroGUI::ButtonTab((char*)"Exploits", FVector2D{ 100.0f, 30.0f }, tab == 2))tab =2;
		if (ZeroGUI::ButtonTab((char*)"Misc", FVector2D{ 100.0f, 30.0f }, tab == 3))tab = 3;

		ZeroGUI::NextColumn(140.f);

		if (tab == 0)
		{
			ZeroGUI::Checkbox((char*)"Skeleton", &bDrawBone);
		//	ZeroGUI::SameLine();
			ZeroGUI::Checkbox((char*)"BOX", &bDrawBox);
			ZeroGUI::Checkbox((char*)"Distance", &bDistance);
			ZeroGUI::NextColumn(260.f);
			ZeroGUI::Checkbox((char*)"Line", &bDrawLine);
			ZeroGUI::Checkbox((char*)"PlayerName", &bDrawName);
			ZeroGUI::Checkbox((char*)"Health", &bHealth);
		}

		if (tab == 1)
		{
			ZeroGUI::Checkbox((char*)"Enable", &bAimBot);
			ZeroGUI::Checkbox((char*)"Visible", &bVisible);
			ZeroGUI::Checkbox((char*)"Silent", &bSlient);
			ZeroGUI::Hotkey((char*)"Keybind", { 132,32 }, &key);
			ZeroGUI::SliderInt((char*)"FOV", &FOV, 4, ScreenWidth);
		}
		if (tab == 2)
		{
			if (ZeroGUI::Button((char*)"Dump Bones", { 100,100 }))
			{

				Logger LOG("\\DumpBone.txt");
				auto mesh = GameWorld->OwningGameInstance->LocalPlayers[0]->
					PlayerController->AcknowledgedPawn->Mesh;
				for (int index = 0; index < mesh->GetBoneNums(); index++) {
					char i = index + '0';
					std::string logs("bone name: " + mesh->GetBoneName(index).GetName()+"    index:" +std::to_string(index));
					LOG.LogToFile(logs);
				}
				LOG.CloseLog();
			}

			if (ZeroGUI::Button((char*)"Uninstall", { 100,100 }))
			{

			}
		}

		if (tab == 3) 
		{
			ZeroGUI::ColorPicker((char*)"Skeleton Color", &Color);
			ZeroGUI::ColorPicker((char*)"Box Color", &Color);
			ZeroGUI::SliderFloat((char*)"Box Thickness", &m_fov, 1, 3);
			ZeroGUI::SliderFloat((char*)"Skeleton Thickness", &m_fov, 1, 3);
		}
	}
	ZeroGUI::Render();
	ZeroGUI::Draw_Cursor(menu_opened);
	tick(canvas);
	OPostRender(GameViewportClient, canvas);
}