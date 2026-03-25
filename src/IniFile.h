#ifndef _IniFile_h_
#define _IniFile_h_
#else
#error reinclude IniFile.h
#endif

enum ConfigLineType {
	cfgEnd		= 0,
	cfgString	= 1,
	cfgBind		= 2,
};

enum DEFAULT_VALUE {
	DEF_COMPLEX,
	ON,
	OFF,
};

enum MENU_OPTION {
	MO_NO,
	MO_BOOL,
	MO_INT,
	MO_LIST,
};

struct ConfigLine {
	ConfigLineType type;
	int menu;
	char* name;
	char* nameMenu;
	void* value;
	DEFAULT_VALUE def;
};


extern char ZeroCfg[];
template<typename T> constexpr typename enable_if< is_array_v<T>, char*>::type Cfg( T& t ) { return t; }
constexpr char* Cfg( ... ) { return ZeroCfg; }
struct CfgParam { char* str; void* param; operator char*(){ return str; } };
template<auto& p> CfgParam cfg { Cfg( p ), &p };

void ConfigLoad(ConfigLine* configStructure, char* iniFileName = "config.ini");
void ConfigSave(ConfigLine* configStructure, char* iniFileName = "config.ini");

// some not yet used funcs from my input module from my project (Dragon) ------------
struct KeyMap {
	int key; 
	bool* control; 
};

void InputInit(KeyMap* defaultMapping=0, int size=0);
void Bind(int key, bool* control);
void UnbindAll();
void ProcessKeyboardInput(int key, bool press);
bool* GetControl(int key);
