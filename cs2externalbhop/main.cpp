#include "memory.h"
#include <thread>
#include <iostream>

namespace offsets {
	constexpr std::ptrdiff_t dwLocalPlayerPawn = 0x17D37F0;
	constexpr std::ptrdiff_t dwEntityList = 0x19684F8;
	constexpr std::ptrdiff_t m_fFlags = 0x3CC;
	constexpr std::ptrdiff_t jump = 0x17CC530;
	constexpr std::ptrdiff_t m_hPlayerPawn = 0x7EC;
	constexpr std::ptrdiff_t m_iHealth = 0x324;
	constexpr std::ptrdiff_t m_iTeamNum = 0x3C3;
	constexpr std::ptrdiff_t dwLocalPlayerController = 0x19B8158;
	DWORD64 m_Glow = 0xBA0;
	DWORD64 m_glowColorOverride = 0x40;
	DWORD64 m_bGlowing = 0x51;
}

int main()
{
	while (true) {
		const uintptr_t localplayer = VARS::memRead<uintptr_t>(VARS::baseAddress + offsets::dwLocalPlayerPawn);
		if (!localplayer)
			continue;
		uint32_t fflags = VARS::memRead<uint32_t>(localplayer + offsets::m_fFlags);
	
		if (GetAsyncKeyState(VK_SPACE) && fflags & (1 << 0))
		{
			VARS::memWrite<uintptr_t>(VARS::baseAddress + offsets::jump, 65537);
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
			VARS::memWrite<uintptr_t>(VARS::baseAddress + offsets::jump, 256);
		}

		for (int i = 1; i < 64; i++)
		{
			uintptr_t localplayercontroller = VARS::memRead<uintptr_t>(VARS::baseAddress + offsets::dwLocalPlayerController);
			if (!localplayercontroller)
				continue;

			int localteam = VARS::memRead<int>(localplayercontroller + offsets::m_iTeamNum);

			uintptr_t entity_list = VARS::memRead<uintptr_t>(VARS::baseAddress + offsets::dwEntityList);
			if (!entity_list)
				continue;

			uintptr_t list_entry = VARS::memRead<uintptr_t>(entity_list + (8 * (i & 0x7FFF) >> 9) + 16);
			if (!list_entry)
				continue;

			uintptr_t player = VARS::memRead<uintptr_t>(list_entry + 120 * (i & 0x1FF));
			if (!player)
				continue;

			int Pteam = VARS::memRead<int>(player + offsets::m_iTeamNum);
			if (Pteam == localteam)
				continue;

			uint32_t player_pawn = VARS::memRead<uint32_t>(player + offsets::m_hPlayerPawn);

			uintptr_t list_entry2 = VARS::memRead<uintptr_t>(entity_list + 0x8 * ((player_pawn & 0x7FFF) >> 9) + 16);
			if (!list_entry2)
				continue;

			uintptr_t p_cs_player_pawn = VARS::memRead<uintptr_t>(list_entry2 + 120 * (player_pawn & 0x1FF));
			if (!p_cs_player_pawn)
				continue;

			VARS::memWrite<DWORD64>(p_cs_player_pawn + offsets::m_Glow + offsets::m_glowColorOverride, 0x800000FF); //you can use any color you want (ARGB)
			VARS::memWrite<DWORD64>(p_cs_player_pawn + offsets::m_Glow + offsets::m_bGlowing, 1);

		}

	}
}