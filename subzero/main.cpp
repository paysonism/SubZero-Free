#include "util.hpp"
#include "cheat/actors/esp.hpp"
#include "cheat/actors/aimbot.hpp"
#include "monkey.h"

Overlay::DXOverlay* pOverlay = nullptr;
HWND game_wndw;
int main() {
	SetConsoleTitleA("");
	system("color 5");
	SetConsoleTitleA("SubZero Free External - Made By Payson");
	printf("\n SubZero Free External - discord.gg/subz\n\n Made By Payson");
	Sleep(3000);
	system("cls");
	std::cout << ("\n [discord.gg/subz] Loading...");
	std::cout << ("\n [discord.gg/subz] Open Fortnite. \n");
	system("cls");
	Sleep(1000);
	s();
	system("cls");
	//downloadAndLoadDriver();
	if (!driver.init())
	{
		Sleep(3500);
		exit(0);
	}
	process_id = _GetProcessId(("FortniteClient-Win64-Shipping.exe"));
	driver.attach(process_id);
	virtualaddy = driver.get_process_base(process_id);
	std::cout << " [discord.gg/subz] Base > 0x" << virtualaddy << "\n";
	system(("cls"));

	printf("\n [discord.gg/subz] Closing this Window will Unload the Cheat. ");
	std::cout << "\n [discord.gg/subz] Base > 0x" << virtualaddy << "\n";
	std::cout << " [discord.gg/subz] Made By Payson" << "\n";

	game_wndw = FindWindow(0, ("Fortnite  "));
	while (!game_wndw) {
		game_wndw = FindWindow(0, ("Fortnite  "));
		Sleep(400);
	}

	Overlay::DXOverlay::GetWindow(FindWindow(0, ("Fortnite  ")));
	auto InitOverlay = pOverlay->InitOverlay();

	//std::cout << ("\n [i] Loading dependencies");
	std::thread t(cache_players);
	t.detach();

	//std::cout << ("\n [i] Initializing overlay");
	Sleep(1000);
	//printf("\n [+] Welcome to SubZero!");
	Sleep(3000);

	//::ShowWindow(::GetConsoleWindow(), SW_HIDE);
	switch (InitOverlay)
	{
	case 0: { break; }
	case 1:
	{
		MouseController::Init();
		while (pOverlay->MainLoop(actor_loop)) {
		}
		break;
	}
	}

}