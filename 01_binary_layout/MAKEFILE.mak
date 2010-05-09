SOURCE=main.cpp

ALL: main.exe

run: main.exe
  main.exe

# Compilation

CPP=cl.exe
CPP_PROJ=/nologo /ML /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_CONSOLE" /D "_MBCS" /FD /c 

.cpp.obj::
   $(CPP) @<<
   $(CPP_PROJ) $< 
<<

# Linking

LINK32=link.exe
LINK32_FLAGS=/nologo /subsystem:console /incremental:no /machine:I386 /out:"main.exe" 
LINK32_OBJS= \
	"main.obj"

main.exe : $(LINK32_OBJS)
  $(LINK32) @<<
  $(LINK32_FLAGS) $(LINK32_OBJS)
<<
