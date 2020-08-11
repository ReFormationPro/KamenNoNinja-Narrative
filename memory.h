void load_addresses();
bool open_process();
void debug();
bool close_process();
bool write_byte(LPCVOID addr, int val);
int read_byte(LPCVOID addr);
int get_enemy_state(enum MOB_TYPE type);
void set_enemy_state(enum MOB_TYPE type, int val);
