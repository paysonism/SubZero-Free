#pragma once
#include "../util.hpp"
#include "structs.hpp"
#include "kernel/driver.h"

#define u uintptr_t
template <typename T>

extern std::string read_string(uintptr_t Address, void* Buffer, SIZE_T Size);
class aactor;
class aplayerstate;
// IF YOU SELL THIS ILL EXPOSE U HARD
// payson1337
class aplayercameramanager {
public:
	camera_position_s get_camera_cache() const {
		if (!(uintptr_t)this) return {};
		return driver.read<camera_position_s>((uintptr_t)this + 0x2A80 + 0x10);
	}
};		// IF YOU SELL THIS ILL EXPOSE U HARD
// payson1337
class uschenecomponent {
public:
	vector3 get_relative_location() const {
		if (!(u)this) return {};
		return driver.read<vector3>((u)this + 0x128);
	}
	vector3 get_relative_rotation() const {
		if (!(u)this) return {};
		return driver.read<vector3>((u)this + 0x140);
	}
};
class uskeletalmeshcomponent {
public:
	vector3 get_bone_3d(int id) const {
		if (!(uintptr_t)this) return {};

		int is_cached = driver.read<int>((u)this + 0x658);
		auto bone_transform = driver.read<FTransform>(driver.read<u>((u)this + 0x10 * is_cached + 0x608) + 0x60 * id);

		FTransform ComponentToWorld = driver.read<FTransform>((u)this + 0x230);

		D3DMATRIX Matrix = {};
		Matrix = MatrixMultiplication(bone_transform.ToMatrixWithScale(), ComponentToWorld.ToMatrixWithScale());

		return vector3(Matrix._41, Matrix._42, Matrix._43);
	}
	vector3 get_head() const {
		if (!(uintptr_t)this) return {};
		return get_bone_3d(109);
	}
	vector3 get_base() const {
		if (!(uintptr_t)this) return {};
		return get_bone_3d(0);
	}
	float get_last_submit_time() const {
		if (!(uintptr_t)this) return 0;
		return driver.read<float>((u)this + 0x368);
	}
	float get_last_render_time() const {
		if (!(uintptr_t)this) return 0;
		return driver.read<float>((u)this + 0x370);
	}
	bool test_visible() const {
		return get_last_submit_time() == get_last_render_time();

	}
	bool visible() const {
		float fLastSubmitTime = get_last_submit_time();
		float fLastRenderTimeOnScreen = get_last_render_time();
		const float fVisionTick = 0.06f;
		bool bVisible = fLastRenderTimeOnScreen + fVisionTick >= fLastSubmitTime;
		return bVisible;
	}

};		// IF YOU SELL THIS ILL EXPOSE U HARD
// payson1337
class afortweapon {
public:
	std::string get_weapon_name() const {
		if (!(u)this) return "";

		auto itemdef = driver.read<uint64_t>((u)this + 0x450); //WeaponData		
		if (!itemdef) return "";
		//std::cout << itemdef << std::endl;
		auto DisplayName = driver.read<uint64_t>(itemdef + 0x98); // DisplayName (FText
		if (!DisplayName) return "";
		auto WeaponLength = driver.read<uint32_t>(DisplayName + 0x40); // FText -> Length
		wchar_t* WeaponName = new wchar_t[uint64_t(WeaponLength) + 1];

		//driver->ReadProcessMemory(read<uint64_t>(DisplayName + 0x38), WeaponName, WeaponLength * sizeof(wchar_t));

		return wchar_to_char(WeaponName);
	}
};
class aactor {
public:

	uschenecomponent* get_root_component() const {
		if (!(u)this) return 0;
		return driver.read<uschenecomponent*>((u)this + 0x198);
	}
	aplayerstate* get_player_state() {
		if (!(u)this) return 0;
		return driver.read<aplayerstate*>((u)this + 0x2b0);
	}
	uskeletalmeshcomponent* get_mesh() const {
		if (!(u)this) return 0;
		return driver.read<uskeletalmeshcomponent*>((u)this + 0x318);
	}
	afortweapon* get_current_weapon() const {
		if (!(u)this) return 0;
		return driver.read<afortweapon*>((u)this + 0x948);
	}

};
class aplayerstate {
public:
	aactor* get_pawn_private() const {
		if (!(u)this) return 0;
		return driver.read<aactor*>((u)this + 0x308);
	}
	int get_player_id() const {
		if (!(u)this) return 0;
		return driver.read<int>((u)this + 0x10a0);
	}
};
class aplayercontroller {
public:
	aactor* get_acknowledged_pawn() const {
		if (!(u)this) return 0;
		return driver.read<aactor*>((u)this + 0x338);
	}

	float get_fov() const {
		if (!(u)this) return 0;
		return driver.read<float>((u)this + 0x38C) * 90.f;
	}
	aplayercameramanager* get_camera_manager() const {
		if (!(uintptr_t)this) return 0;
		return (aplayercameramanager*)driver.read<uintptr_t>((uintptr_t)this + 0x348);
	}
};
class ulocalplayer {
public:
	aplayercontroller* get_player_controller() const {
		if (!(u)this) return 0;
		return driver.read<aplayercontroller*>((u)this + 0x30);
	}

};
class ugameinstance {
public:
	ulocalplayer* get_local_player() const {
		if (!(u)this) return 0;
		return driver.read<ulocalplayer*>((u)driver.read<ulocalplayer*>((u)this + 0x38));
	}

};
// IF YOU SELL THIS ILL EXPOSE U HARD
// payson1337
class agamestate {
public:
	tarray<aplayerstate*> get_player_array() const {
		if (!(u)this) return {};
		return driver.read<tarray<aplayerstate*>>((u)this + 0x2A8);
	}

};
class uworld {
public:

	ugameinstance* get_game_instance() const {
		if (!(u)this) return 0;
		return driver.read<ugameinstance*>((u)this + 0x1d0);
	}
	agamestate* get_game_state() const {
		if (!(u)this) return 0;
		return driver.read<agamestate*>((u)this + 0x158);
	}
};

