Manual loading of Lua shared library made easy

This article explains a method to manually load all Lua 5.1 functions from a shared library
into your application, by simply changing only 5 lines of C code in your program sources!
If you have already manually loaded some shared library (.dll or .so), you can skip the next four
introduction chapters.


About dynamic loaded shared libraries
On Windows, shared dynamic libraries are known as Dynamic Link Library or DLL (*.dll).
On Unix, there are usually referred as shared object (*.so) or shared library (*.sa).
Both refer to the same nature of executable files, that we will call "SL" (shared library) from now on.
A SL contains compiled and linked machine code, exports public functions to application programs, 
but does not have a "main()" function by itself and thus cannot be directly executed.
Instead, a SL is loaded by the operating system into an application memory space, and the application
can then see exported SL's functions and variables as if it was inside itself.


SL loading modes
There are two ways for an application to load a SL, either automatically or manually.
In the first mode, the operating system automatically loads the SL when the application is started and
exports all functions to it. If the SL is not found, the OS will issue an error and the application 
program cannot run. Using automatic load is quite easy: you compile the application with SL standard 
include files (on Windows, using the "__declspec(dllimport)" function prefix) and link with a special
static library (*.lib on Windows, *.a on Unix) and you are done. 
In the manual mode, the application must itself load the SL (with LoadLibrary on Windows or dlopen
on POSIX systems), export each needed function with GetProcAddress or dlsym, and close the SL when
finished with FreeLibrary or dlclose. Normally, you cannot use the SL header file, but have to 
"typedef" the function prototypes, instantiate a variable of it and affect the result of GetProcAddress
to this variable, before you can call the exported function.


Manual loading usage
Why would you use manual loading, despite it is more complicated and error prone? 
Sometimes, you don't have the choice. Manual loading is more flexible and let you:
 - load a SL only when needed and not during all application execution;
 - load a SL when its file name or path is only known at runtime;
 - export functions whose names are only known at runtime;
 - use SL when no associated static library is given (third party code);
 - use SL when the associated static library was generated for a different compiler than yours.


Traditional examples using Lua 5.1
We will of course the traditional "Hello World!" example. 
The string will be output by Lua 5.1 interpreter, inside a luaL_dostring function. 
First, the example using automatic SL loading. You will have to link
it with the associated static library (something like -llua).

#include "lua.h"
#include "lauxlib.h"
#include "lualib.h"

int main(int argc, char* argv[])
{
	lua_State* L;
	L = lua_open();
	luaL_openlibs(L);
	luaL_dostring(L, "print 'Hello World'");
	lua_close(L);
	return 0;
}

The same example using the usual manual loading on Windows system:

#include <windows.h>
#include "lua.h"

typedef lua_State * (__cdecl *luaL_newstate_t) (void);
typedef void (__cdecl *luaL_openlibs_t) (lua_State *L); 
typedef int (__cdecl *luaL_loadstring_t) (lua_State *L, const char *s);
typedef int (__cdecl *lua_pcall_t) (lua_State *L, int nargs, int nresults, int errfunc);
typedef void (__cdecl *lua_close_t) (lua_State *L);

luaL_newstate_t   luaL_newstate_ptr;
luaL_openlibs_t   luaL_openlibs_ptr;
luaL_loadstring_t luaL_loadstring_ptr;
lua_pcall_t       lua_pcall_ptr;
lua_close_t       lua_close_ptr;

int main(int argc, char* argv[])
{
	lua_State* L;
	HMODULE module = LoadLibrary("lua5.1.dll");
	if(module == NULL)
		return 1;
	luaL_newstate_ptr   = (luaL_newstate_t)  GetProcAddress(module, "luaL_newstate");
	luaL_openlibs_ptr   = (luaL_openlibs_t)  GetProcAddress(module, "luaL_openlibs");
	luaL_loadstring_ptr = (luaL_loadstring_t)GetProcAddress(module, "luaL_loadstring");
	lua_pcall_ptr       = (lua_pcall_t)      GetProcAddress(module, "lua_pcall");
	lua_close_ptr       = (lua_close_t)      GetProcAddress(module, "lua_close");
	if(luaL_newstate_ptr == NULL || luaL_openlibs_ptr == NULL || lua_close_ptr == NULL
		|| luaL_loadstring_ptr == NULL || lua_pcall_ptr == NULL)
	  	return 1;
	
	L = luaL_newstate_ptr();
	luaL_openlibs_ptr(L);
	/* Cannot use macro luaL_dostring, because lua_pcall is renamed ! */
	luaL_loadstring_ptr(L, "print 'Hello World'") || lua_pcall_ptr(L, 0, LUA_MULTRET, 0);
	lua_close_ptr(L);
	return 0;
}

This second version has numerous disadvantages: 
 - the code is much longer, with a lot of redundancy;
 - we could not take benefit of function prototypes inside lua.h, lauxlib.h and lualib.h;
 - we cannot generally use the macros like luaL_dostring, because they are implemented
   using other API functions, which are renamed in the source code;
 - it is far too easy to make an error on typedef lines, like forgetting a __cdecl.

Third version of the example, using the proposed simplified method. You must compile
it together with a second lua_dyn.c file: it may be "gcc -o hello hello.c lua_dyn.c"

#include <windows.h>
#include "lua_dyn.h"

#define LUA_PREFIX LuaFunctions.
lua_All_functions LuaFunctions;

int main(int argc, char* argv[])
{
	lua_State* L;
	HMODULE module = LoadLibrary("lua5.1.dll");
	if(!luaL_loadfunctions(module, &LuaFunctions, sizeof(LuaFunctions)))
		return 1;

	L = lua_open();
	luaL_openlibs(L);
	luaL_dostring(L, "print 'Hello World'");
	lua_close(L);
	return 0;
}

You can see that we only had to change the header file, instantiate a structure, define a macro,
load the library and call an external function luaL_loadfunctions from lua_dyn.c.
After that, the same code as the automatic loading can be used, including all macros !


How does it work ?
The Lua script "export_h.lua" is used to generate files lua_dyn.h and lua_dyn.c from Lua 5.1 
header files lua.h, lauxlib.h and lualib.h. It can be runned on any standard Lua 5.1 interpreter.
Every time the script sees an external function definition, it replaces it with a typedef, a field
in a function structure and a #define. Example:

LUA_API lua_State *(lua_newstate) (lua_Alloc f, void *ud);
LUA_API void       (lua_close) (lua_State *L);
LUA_API lua_State *(lua_newthread) (lua_State *L);

becomes:

typedef lua_State * (__cdecl *lua_newstate_t) (lua_Alloc f, void *ud);
typedef void (__cdecl *lua_close_t) (lua_State *L);
typedef lua_State * (__cdecl *lua_newthread_t) (lua_State *L);
...
typedef struct lua_All_functions
{
  lua_newstate_t          Newstate;
  lua_close_t             Close;
  lua_newthread_t         Newthread;
  ...
} lua_All_functions;
...
#define lua_newstate            LUA_PREFIX Newstate
#define lua_close               LUA_PREFIX Close
#define lua_newthread           LUA_PREFIX Newthread

The structure lua_All_functions contains a pointer to all external Lua API functions. 
This permits instantiating all function pointers in a single line: either declare a static variable,
or allocate it with malloc or with new in C++. It also simplifies the initialization: a single
function call to luaL_loadfunctions will export all functions from a function names table.
The macro definitions enable you to use the standard function names like "lua_newstate"
instead of a structure access like "luaFct.Newstate" or "m_pLuaF->Newstate". 
Also, it assures the compatibility with the API macros built on top of other functions. 
For this to compile, you have to define LUA_PREFIX as "luaFct." or "m_pLuaF->" respectively.
As the original function declaration is removed from the header, there is no name clash.

Example: in the previous example, line 16:
	luaL_dostring(L, "print 'Hello World'");
is replaced with the following code by the preprocessor:
	(LuaFunctions. LoadstringL(L, "print 'Hello World'") || LuaFunctions. Pcall(L, 0, (-1), 0));
This will compile because LuaFunctions is a static declaration of struct lua_All_functions.


Customization
The provided files lua_dyn.c and lua_dyn.h are build with Lua 5.1.2 header files without modification.
For this reason, it will probably work on any standard distribution of Lua interpreter. 
It has been tested it on both Windows and Linux platforms. 
If you have a different configuration, proceed the following way:
 - Edit top of script file export_h.lua. The 16 first lines are for configuration. You might want
   to add other include files (for your custom libraries), or adapt conf_defines table to reflect
   the parameters in luaconf.h;
 - Go to the Lua 5.1 source directory (cd lua-5.1.2/src);
 - Run export_h.lua from there (lua export_h.lua);
 - Move generated files lua_dyn.* into where you want.

If you can recompile Lua shared library, you might prefer to compile file lua_dyn.c inside that library,
thus simplifying event more manual loading. If this is the case, change loaderfct_indll flag to true,
which will define a help macro LUA_LOAD_FUNCTIONS. That macro will manually load the function 
luaL_loadfunctions from Lua SL, and then calling it, in a single instruction.
