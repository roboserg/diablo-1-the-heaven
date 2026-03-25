#include "stdafx.h"

// some not yet used funcs from my input module from my project (Dragon) ------------
bool* controls[256]; // Array used for the keyboard routine
void UnbindAll()
{
	for( int i=0; i<256; i++ ){
		controls[i] = 0;
	}
}

void Bind(int key, bool* control)
{
	controls[key] = control;
}

void InputInit(KeyMap* defaultMapping/*=0*/, int size/*=0*/)
{
	UnbindAll();
	for( int i=0; i<size; i++ ){
		Bind(defaultMapping[i].key, defaultMapping[i].control);
	}
}

void ProcessKeyboardInput(int key, bool press)
{
	if( controls[key] ){
		*controls[key] = press;
	}
}

bool* GetControl(int key)
{
	return controls[key];
}

// ---------------------------------------------------------------------
// Таблица соответствия имен и номеров клавиш
struct { 
	char* name;
	int number;
} Keys[] = {	
	{"", 0},
	{"0", '0'},	{"1", '1'},	{"2", '2'},	{"3", '3'},	{"4", '4'},	{"5", '5'},
	{"6", '6'},	{"7", '7'},	{"8", '8'},	{"9", '9'},	{"q", 'Q'},	{"w", 'W'},
	{"e", 'E'},	{"r", 'R'},	{"t", 'T'},	{"y", 'Y'},	{"u", 'U'},	{"i", 'I'},
	{"o", 'O'},	{"p", 'P'},	{"a", 'A'},	{"s", 'S'},	{"d", 'D'},	{"f", 'F'},
	{"g", 'G'},	{"h", 'H'},	{"j", 'J'},	{"k", 'K'},	{"l", 'L'},	{"z", 'Z'},
	{"x", 'X'},	{"c", 'C'},	{"v", 'V'},	{"b", 'B'},	{"n", 'N'},	{"m", 'M'},
	{"f1", VK_F1}, {"f2",  VK_F2},  {"f3",  VK_F3},  {"f4",  VK_F4},
	{"f5", VK_F5}, {"f6",  VK_F6},  {"f7",  VK_F7},  {"f8",  VK_F8},
	{"f9", VK_F9}, {"f10", VK_F10}, {"f11", VK_F11}, {"f12", VK_F12},
	{"backspace", VK_BACK},	{"tab", VK_TAB}, {"esc", VK_ESCAPE}, {"space", VK_SPACE},
	{"enter", VK_RETURN}, {"shift", VK_SHIFT}, {"control", VK_CONTROL},
	{"alt", VK_MENU}, {"capslock", VK_CAPITAL}, {"pause", VK_PAUSE}, {"=", '='},
	{"right", VK_RIGHT}, {"left", VK_LEFT},	{"up", VK_UP}, {"down", VK_DOWN},
	{"pageup", VK_PRIOR}, {"pagedown", VK_NEXT}, {"home", VK_END}, {"end", VK_END},
	{"ins", VK_INSERT}, {"del", VK_DELETE}, {"tilda", '`'}, {"-", '-'},	{"\\", '\\'},
	{"[", '['}, {"]", ']'}, {";", ';'}, {"\'", '\''}, {",", ','}, {".", '.'},
	{"/", '/'},	{"scroll", VK_SCROLL},	{"print", VK_PRINT}, {"numlock", VK_NUMLOCK},
	{"num0", VK_NUMPAD0}, {"num1", VK_NUMPAD1},	{"num2", VK_NUMPAD2},
	{"num3", VK_NUMPAD3}, {"num4", VK_NUMPAD4},	{"num5", VK_NUMPAD5},
	{"num6", VK_NUMPAD6}, {"num7", VK_NUMPAD7},	{"num8", VK_NUMPAD8},
	{"num9", VK_NUMPAD0}, {"num+", VK_ADD},	    {"num-", VK_SUBTRACT},
	{"num*", VK_MULTIPLY},{"num/", VK_DIVIDE},	{"num.", VK_DECIMAL}
};

// Поиск номера клавиши по имени
int GetKeyNumberByName(const char* data)
{
	for( int i = 1; i < countof(Keys); i++ ){
		if( !_stricmp(Keys[i].name, data) ){
			return Keys[i].number;
		}
	}
	return 0;
}

// Поиск имени клавиши по номеру
char* GetKeyNameByNumber(int key)
{
	for( int i = 1; i < countof(Keys); i++ ){
		if( key == Keys[i].number ){
			return Keys[i].name;
		}
	}
	return "unknown";
}

// Поиск переменной по имени
ConfigLine* FindVarByName(int type, ConfigLine* configStructure, char* data)
{
	for( int i = 0; configStructure[i].type && i < 1024; i++ ){
		if( configStructure[i].type == type && !_stricmp(configStructure[i].name, data) ){
			return &configStructure[i];
		}
	}
	return 0;
}

char ZeroCfg[] = "";
// Загрузка конфигурационного файла
void ConfigLoad(ConfigLine* configStructure, char* iniFileName /*= "config.ini"*/ )
{
	static char data[4096];
	enum { readCommand, readName, readValue, readKey, readControl };
	void* var = nullptr;
	int key;
	int state = readCommand;
	if( FILE* ini = fopen(iniFileName,"r") ){
		if( fread(data, 1, 3, ini) != 3 || memcmp(data, UTF8_BOM, 3) != 0 ) fseek(ini, 0, SEEK_SET); // skip UTF-8 BOM marker
		UnbindAll();
		while( fscanf_s(ini, "%s", data, 4096) != EOF ){
			if( *(short*)data == '//' ){ // skip part of line after //
				if( fscanf_s(ini, "%4000[^\n]", data, 4096) == EOF ) break;
				continue;
			}
			switch( state ){
			case readCommand:
				if( !_stricmp(data,"bind") ){
					state = readKey;
				}else if( !_stricmp(data,"set") ){
					state = readName;
				}
				break;
			case readName:
				var = FindVarByName(cfgString, configStructure, data);
				if( var ){
					state = readValue;
				}else{
					state = readCommand;
				}
				break;
			case readValue: {
				if( var ){
				ConfigLine& cl = *(ConfigLine*)var;
				char** v = (char**) cl.value; 
				if( *v == ZeroCfg ){
					*v = _strdup( data );
				}else{
					strncpy_s( *v, strlen(*v) + 1, data, _TRUNCATE );
				}
				void* p = ((CfgParam*)cl.value)->param;
				switch( cl.def ){ // попробовать переделать на вызов лямбды по дефайну
				case ON: *(bool*)p = !(_stricmp( *v, "off" ) == 0); break; // default on
				case OFF: *(bool*)p = _stricmp( *v, "on" ) == 0; break; // default off
				} } }
				state = readCommand;
				break;
			case readKey:
				key = GetKeyNumberByName(data);
				if( key ){
					state = readControl;
				}else{
					state = readCommand;
				}
				break;
			case readControl:
				var = FindVarByName(cfgBind, configStructure, data)->value;
				if( var ){
					Bind(key, (bool*)var);
				}
				state = readCommand;
				break;
			}
		}
		fclose(ini);
	}
}

// Запись конфигурационного файла
void ConfigSave(ConfigLine* configStructure, char* iniFileName /*= "config.ini"*/ )
{
	if( FILE* ini = fopen(iniFileName,"w") ){
		for( int i = 0; configStructure[i].type && i < 1024; i++ ){
			switch( configStructure[i].type ){
			case cfgString:
				fprintf(ini, "set %s %s\n", configStructure[i].name, (char*)configStructure[i].value);
				break;
			case cfgBind:
				for( int key = 1; key < 256 ; key++ ){
					if( configStructure[i].value == (void*)GetControl(key) ){
						fprintf(ini, "bind %s %s\n", GetKeyNameByNumber(key), configStructure[i].name);
					}
				}
				break;
			}
		}
		fclose(ini);
	}
}