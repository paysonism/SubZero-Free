#pragma once
#pragma warning(disable : 4996)



extern void preform_aimbot(double closest_distance, std::shared_ptr<player_data> closest_player);
inline camera_position_s get_camera_postion() {


	camera_position_s camera;

	auto location_pointer = driver.read<uintptr_t>((u)global->world + 0x110);
	auto rotation_pointer = driver.read<uintptr_t>((u)global->world + 0x120);

	struct FNRot
	{
		double a; //0x0000
		char pad_0008[24]; //0x0008
		double b; //0x0020
		char pad_0028[424]; //0x0028
		double c; //0x01D0
	}fnRot;

	fnRot.a = driver.read<double>(rotation_pointer);
	fnRot.b = driver.read<double>(rotation_pointer + 0x20);
	fnRot.c = driver.read<double>(rotation_pointer + 0x1d0);

	camera.location = driver.read<vector3>(location_pointer);
	camera.rotation.x = asin(fnRot.c) * (180.0 / M_PI);
	camera.rotation.y = ((atan2(fnRot.a * -1, fnRot.b) * (180.0 / M_PI)) * -1) * -1;
	camera.fov = driver.read<float>((uintptr_t)(u)global->player_controller + 0x394) * 90.f;

	return camera;

}
std::string string_To_UTF8(const std::string& str)
{
	int nwLen = ::MultiByteToWideChar(CP_ACP, 0, str.c_str(), -1, NULL, 0);
	wchar_t* pwBuf = new wchar_t[nwLen + 1];
	ZeroMemory(pwBuf, nwLen * 2 + 2);
	::MultiByteToWideChar(CP_ACP, 0, str.c_str(), str.length(), pwBuf, nwLen);
	int nLen = ::WideCharToMultiByte(CP_UTF8, 0, pwBuf, -1, NULL, NULL, NULL, NULL);
	char* pBuf = new char[nLen + 1];
	ZeroMemory(pBuf, nLen + 1);
	::WideCharToMultiByte(CP_UTF8, 0, pwBuf, nwLen, pBuf, nLen, NULL, NULL);
	std::string retStr(pBuf);
	delete[]pwBuf;
	delete[]pBuf;
	pwBuf = NULL;
	pBuf = NULL;
	return retStr;
}
void draw_outlined_text(int x, int y, int R, int G, int B, int A, const char* str)
{
	std::string utf_8_1 = std::string(str);
	std::string utf_8_2 = string_To_UTF8(utf_8_1);

	ImGui::GetBackgroundDrawList()->AddText(ImGui::GetFont(), 18.0f, ImVec2(x, y), ImGui::ColorConvertFloat4ToU32(ImVec4(R / 255.0f, G / 255.0f, B / 255.0f, A / 255.0f)), utf_8_2.c_str());
}
float GRD_TO_BOG(float GRD) {
	return (M_PI / 180) * GRD;
}
float BOG_TO_GRD(float BOG) {
	return (180 / M_PI) * BOG;
}
static ImColor linecolor;
static ImColor Skelcolor;
static ImColor BoxColor;
void actor_loop() {

	ImGui::PushFont(ESPFont);

	//cout << "actor_loop\n";
	/*if (debug) {
		printf(("\n [+] ActorLoop called"));
	}*/

	std::unique_lock<std::mutex> lock(global->data_mutex);

	camera_postion = get_camera_postion();

	ImDrawList* draw_list = ImGui::GetForegroundDrawList();
	/*char fpsinfo[64];
	sprintf(fpsinfo, ("fps: %03d"), get_fps());
	draw_list->AddText(ImGui::GetFont(), 14, ImVec2(20, 20), ImColor(0, 225, 0), fpsinfo);*/

	if (draw_fov) {
		draw_list->AddCircle(ImVec2(screen_width / 2, screen_height / 2), fov, ImColor(1, 0, 1), 60, 1);
		draw_list->AddCircle(ImVec2(screen_width / 2, screen_height / 2), fov + 1, ImColor(1, 0, 1), 60, 1);
	}
	if (draw_crosshair) {
		draw_list->AddLine(ImVec2(screen_width / 2 - 2, screen_height / 2), ImVec2(screen_width / 2 - 9, screen_height / 2), ImColor(0, 0, 0), 2);
		draw_list->AddLine(ImVec2(screen_width / 2 + 2, screen_height / 2), ImVec2(screen_width / 2 + 9, screen_height / 2), ImColor(0, 0, 0), 2);
		draw_list->AddLine(ImVec2(screen_width / 2, screen_height / 2 - 2), ImVec2(screen_width / 2, screen_height / 2 - 9), ImColor(0, 0, 0), 2);
		draw_list->AddLine(ImVec2(screen_width / 2, screen_height / 2 + 2), ImVec2(screen_width / 2, screen_height / 2 + 9), ImColor(0, 0, 0), 2);

		draw_list->AddLine(ImVec2(screen_width / 2 - 3, screen_height / 2), ImVec2(screen_width / 2 - 8, screen_height / 2), ImColor(255, 255, 255), 1);
		draw_list->AddLine(ImVec2(screen_width / 2 + 3, screen_height / 2), ImVec2(screen_width / 2 + 8, screen_height / 2), ImColor(255, 255, 255), 1);
		draw_list->AddLine(ImVec2(screen_width / 2, screen_height / 2 - 3), ImVec2(screen_width / 2, screen_height / 2 - 8), ImColor(255, 255, 255), 1);
		draw_list->AddLine(ImVec2(screen_width / 2, screen_height / 2 + 3), ImVec2(screen_width / 2, screen_height / 2 + 8), ImColor(255, 255, 255), 1);
	}
	/*if (!show_console) {
		ShowWindow(GetConsoleWindow(), SW_HIDE);
	}

	else if (show_console) {
		ShowWindow(GetConsoleWindow(), SW_SHOW);
	}*/


	if (obs_bypass) {
		SetWindowDisplayAffinity(Overlay::DXOverlay::pOverlay, WDA_EXCLUDEFROMCAPTURE);
	}
	if (!obs_bypass) {
		SetWindowDisplayAffinity(Overlay::DXOverlay::pOverlay, !WDA_EXCLUDEFROMCAPTURE);
	}
	ImGui::GetBackgroundDrawList()->AddText(ImVec2(35, 25), ImColor(0, 255, 255), "SubZero Free");

	float fps = ImGui::GetIO().Framerate;

	ImGui::GetBackgroundDrawList()->AddText(ImVec2(35, 45), ImColor(0, 255, 255), ("FPS: " + std::to_string(static_cast<int>(fps))).c_str());

	double closest_distance = DBL_MAX;
	std::shared_ptr<player_data> closest_player = std::make_shared<player_data>();
	for (auto player : global->players) {
		vector3 head_3d = player->mesh->get_head();
		vector2 head_2d = w2s(vector3(head_3d.x, head_3d.y, head_3d.z + 20));

		vector3 base_3d = player->mesh->get_base();
		vector2 base_2d = w2s(base_3d);

		float box_hight = abs(head_2d.y - base_2d.y);
		float box_width = box_hight * 0.45;

		if (!head_2d.valid_location())
			continue;

		ImGui::PushFont(ESPFont);
		if (esp) {

			ImColor visible_color;
			if (player->mesh->visible()) {
				visible_color = ImGui::GetColorU32({ visible_color_float[0], visible_color_float[1], visible_color_float[2],  1.0f });
			}
			else if (!player->mesh->visible()) {
				visible_color = ImGui::GetColorU32({ invisible_color_float[0], invisible_color_float[1], invisible_color_float[2],  1.0f });
			}
			if (player->player == global->local_pawn) continue;




			if (box_esp) {
				//BoxColor = ImGui::GetColorU32({ boxcolor[0], boxcolor[1], boxcolor[2],  1.0f });
				if (box_type == 0) {
					if (box_outline) {
						if (box_round)
							draw_list->AddRect(ImVec2(head_2d.x - box_width / 2, head_2d.y), ImVec2((head_2d.x - box_width / 2) + box_width, head_2d.y + box_hight), visible_color, 10, ImDrawCornerFlags_All, box_outline_thickness);
						else
							draw_list->AddRect(ImVec2(head_2d.x - box_width / 2, head_2d.y), ImVec2((head_2d.x - box_width / 2) + box_width, head_2d.y + box_hight), visible_color, 0, 0, box_thickness);

					}
					if (box_round)
						draw_list->AddRect(ImVec2(head_2d.x - box_width / 2, head_2d.y), ImVec2((head_2d.x - box_width / 2) + box_width, head_2d.y + box_hight), visible_color, 10, ImDrawCornerFlags_All, box_thickness);
					else
						draw_list->AddRect(ImVec2(head_2d.x - box_width / 2, head_2d.y), ImVec2((head_2d.x - box_width / 2) + box_width, head_2d.y + box_hight), visible_color, 0, 0, box_thickness);
				}
				else if (box_type == 1) {
					if (box_outline)
						DrawCornerBox(head_2d.x - box_width / 2, head_2d.y, box_width, box_hight, box_outline_thickness, ImColor(0, 0, 0), box_round);

					DrawCornerBox(head_2d.x - box_width / 2, head_2d.y, box_width, box_hight, box_thickness, visible_color, box_round);
				}
				else if (box_type == 2) {
					if (box_outline)
						DrawThreeDBox(base_3d, head_3d, 100, 100, box_outline_thickness, ImColor(0, 0, 0));
					DrawThreeDBox(base_3d, head_3d, 100, 100, box_thickness, visible_color);
				}
			}


			if (draw_username)
			{
				auto name = username((uintptr_t)player->player_state);
				ImVec2 textPosition(head_2d.x, head_2d.y - 13);
				draw_outlined_text(textPosition, ImColor(255, 255, 255), name.c_str());
			}
			auto CurrentWeapon = driver.read<uintptr_t>((uintptr_t)player->player + 0x9F8);
			if (draw_held_weapon) {
				auto ItemData = driver.read<DWORD_PTR>(CurrentWeapon + 0x498);
				auto DisplayName = driver.read<uint64_t>(ItemData + 0x98);
				auto WeaponLength = driver.read<uint32_t>(DisplayName + 0x40);
				wchar_t* WeaponName = new wchar_t[uint64_t(WeaponLength) + 1];
				driver.read_buffer((uintptr_t)driver.read<PVOID>(DisplayName + 0x38), (uint8_t*)WeaponName, WeaponLength * sizeof(wchar_t));

				BYTE tier;
				tier = driver.read<BYTE>(ItemData + 0x73);
				ImColor Color;

				if (tier == 2)
				{
					Color = IM_COL32(0, 255, 0, 255);
				}
				else if ((tier == 3))
				{
					Color = IM_COL32(0, 0, 255, 255);
				}
				else if ((tier == 4))
				{
					Color = IM_COL32(128, 0, 128, 255); // p
				}
				else if ((tier == 5))
				{
					Color = IM_COL32(255, 255, 0, 255); // p
				}
				else if ((tier == 6))
				{
					Color = IM_COL32(255, 255, 0, 255); // p
				}
				else if ((tier == 0) || (tier == 1))
				{
					Color = IM_COL32(255, 255, 255, 255); // p
				}
				if (!WeaponName || !tier)
				{
					delete[] WeaponName;
					WeaponName = new wchar_t[5];
					wcscpy(WeaponName, L"None");
				}

				std::string wep_str = wchar_to_char(WeaponName);
				//return wep_str;
				ImVec2 textPosition(head_2d.x, head_2d.y - 25);

				draw_outlined_text(textPosition, ImColor(255, 255, 255), wep_str.c_str());
			}

			if (draw_platform) {
				DWORD_PTR test_platform = driver.read<DWORD_PTR>((uintptr_t)player->player_state + 0x438);
				wchar_t platform[64];
				driver.read_buffer((uintptr_t)test_platform, (uint8_t*)platform, sizeof(platform));
				std::wstring platform_wstr(platform);
				std::string platform_str(platform_wstr.begin(), platform_wstr.end());
				ImVec2 textPosition(head_2d.x, head_2d.y - 35);

				draw_outlined_text(textPosition, ImColor(255, 255, 255), platform_str.c_str());
			}

			if (line) {
				//linecolor = ImGui::GetColorU32({ linecol[0], linecol[1], linecol[2],  1.0f });
				draw_list->AddLine(ImVec2(screen_width / 2, 0), ImVec2(head_2d.x, head_2d.y), visible_color, 2);
			}
			if (skeleton) {
				//Skelcolor = ImGui::GetColorU32({ skeletoncolor[0], skeletoncolor[1], skeletoncolor[2],  1.0f });
				drawskeleton(player->mesh, visible_color, skel_thickness);
			}


			if (draw_distance) {
				std::string dist = "[" + std::to_string(static_cast<int>(player->player_distance)) + ".0]";
				ImVec2 TextSize = ImGui::CalcTextSize(dist.c_str());
				DrawString(13, (base_2d.x) - (TextSize.x / 2), (base_2d.y + 5), &Col.white, false, true, dist.c_str());
				DrawString(13, (base_2d.x) - (TextSize.x / 2), (base_2d.y + 5), &Col.white, false, true, dist.c_str());
			}
		}

		auto dist = GetCrossDistance(head_2d.x, head_2d.y, screen_width / 2, screen_height / 2);
		if (dist < fov && dist < closest_distance && player->player_distance < 10000) {
			closest_distance = dist;
			closest_player = player;
		}

	}
	preform_aimbot(closest_distance, closest_player);
	lock.unlock();


}