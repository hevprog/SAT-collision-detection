
name = ProSimDebug.exe
namefile = ProgramSim.cpp
all:
	g++ -o$(name) $(namefile) -Iinclude -Llib -lraylib -lwinmm -lopengl32 -lgdi32

	.\$(name)