//ENEMY_FLAGS_BASE_OFF + enemy type = is enemy killed
enum MOB_TYPE {
	NINJA_KIDS,
	NINJA_MEN,
	SKINWALKERS,
	HUMAN_SISHKEBAB,
	FINAL_BOSS,
	ROMANTIC_MAN = 5,
	BALOON_MAN,
	POSIDONS_DAUGHTER,	//3CD
	ELECMAN = 8,
	TOADMAN,	//OFFSET 3CF
	REDSKINDUDE,
	FIRE_OGRE,
	BALDIE = 12,//3D2
	BAD_ALAADDIN = 13,
	IRON_MAN,
	LOCHNESS
};

enum MOB_TYPE get_mob_type();
void set_mob_type(enum MOB_TYPE type);
void set_on_access_breakpoint(LPVOID addr);
void load_addresses();
bool open_process();
void debug();
bool close_process();
void write_byte(LPCVOID addr, int val);
int read_byte(LPCVOID addr);
int get_enemy_state(enum MOB_TYPE type);
void set_enemy_state(enum MOB_TYPE type, int val);
