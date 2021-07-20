/*
    Plugin-SDK (Grand Theft Auto) SHARED header file
    Authors: GTA Community. See more here
    https://github.com/DK22Pac/plugin-sdk
    Do not delete this comment block. Respect others' work!
*/
#pragma once
#include "CRunningScript.h"
#include <extensions/scripting/ScriptCommandNames.h>
#include <vector>
#include <string.h>
#include <lua.hpp>
#define SOL_ALL_SAFETIES_ON 1
#include <sol/sol.hpp>

class CPed;
class CVehicle;
class CObject;

namespace pluginmy {

class scripting {

public:

enum ScriptCommandEndParameter {
    END_PARAMETER
};

private:

enum ScriptResultVarType {
    SCRIPT_RESULT_VAR_NUMBER,
    SCRIPT_RESULT_VAR_STRING,
    SCRIPT_RESULT_VAR_PED,
    SCRIPT_RESULT_VAR_VEHICLE,
    SCRIPT_RESULT_VAR_OBJECT
};

class ScriptCode {
    unsigned char *data;
    unsigned int capacity;
    unsigned int size;

    struct VarToSet {
        unsigned int varIndex;
        void *pVar;
        ScriptResultVarType varType;

        VarToSet(unsigned int _varIndex, void *_pVar, ScriptResultVarType _varType);
    };

    std::vector<VarToSet> varsToSet;
    unsigned short varIndexCounter;

public:
    ScriptCode(short commandId = -1);
    ~ScriptCode();
    void AddParameterDescription(unsigned char paramType);
    void AddBytes(unsigned char *bytes, unsigned int count);
    unsigned char *GetData();
    void SaveResultVariables(CRunningScript *script);
    void operator<<(char n);
    void operator<<(unsigned char n);
    void operator<<(short n);
    void operator<<(unsigned short n);
    void operator<<(int n);
    void operator<<(unsigned int n);
    void operator<<(float n);
    void operator<<(double n);
    void operator<<(ScriptCommandEndParameter);
    void operator<<(char *str);
    void operator<<(const char *str);
    void operator<<(float *p);
    void operator<<(int *p);
    void operator<<(unsigned int *p);
#ifdef GTASA
    void operator<<(char(*p)[16]);
#endif
    void operator<<(CPed *n);
    void operator<<(CVehicle *n);
    void operator<<(CObject *n);
    void operator<<(CPed **p);
    void operator<<(CVehicle **p);
    void operator<<(CObject **p);

    template <typename T>
    void Pack(T value) {
        operator<<(value);
    }

    template <typename First, typename... Rest>
    void Pack(First firstValue, Rest... rest) {
        Pack(firstValue);
        Pack(rest...);
    }

    void Pack() {}
};

public:


static bool CallCommandByIdPtr(unsigned int commandId, sol::table &params) {
    // create our 'script' object
    static CRunningScript script;
    memset(&script, 0, sizeof(CRunningScript));
#ifdef GTASA
    script.Init();
#else
    script.m_bWastedBustedCheck = true;
#endif
    strcpy(script.m_szName, "plg-sdk");
    script.m_bIsMission = false;
    script.m_bUseMissionCleanup = false;
    script.m_bNotFlag = (commandId >> 15) & 1;
    // our script code
    ScriptCode code(commandId);
    // for all arguments: add them to script code
	for (auto param : params)
	{
		sol::table value = param.second;
		std::string type = value[2];
		if (type._Equal("int"))
		{
			int val = value[1];
			code.Pack(val);
		} else if (type._Equal("string"))
		{
			std::string val = value[1];
			code.Pack(val.c_str());
		}
		else if (type._Equal("bool"))
		{
			bool val = value[1];
			code.Pack((int)val);
		} else if (type._Equal("float"))
		{
			float val = value[1];
			code.Pack(val);
		} else if (type._Equal("ped"))
		{
			CPed* val = value[1];
			code.Pack(val);
		} else if (type._Equal("int*"))
		{
			sol::object val = value[1];
			code.Pack((int*)val.pointer());
		} else if (type._Equal("float*"))
		{
			sol::object val = value[1];
			code.Pack((float*)val.pointer());
		} else if (type._Equal("char**"))
		{
			char(*val)[16] = value[1];
			code.Pack(val);
		} 
	}
#ifdef GTASA
    script.m_pBaseIP = script.m_pCurrentIP = code.GetData();
#else
    script.m_nIp = reinterpret_cast<int>(code.GetData()) - reinterpret_cast<int>(CRunningScript::GetScriptSpaceBase());
#endif
    script.ProcessOneCommand();
    code.SaveResultVariables(&script);
    return script.m_bCondResult ? true : false;
}
	
};

};
