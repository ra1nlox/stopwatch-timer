linux_output_path = ./dist/linux_amd64
windows_output_path = ./dist/win64_x86_64

app_name = stopwatch

dlinux:
	mkdir -p $(linux_output_path)/
	cc \
	-Wall \
	-Wextra \
	-ggdb \
	-o $(linux_output_path)/$(app_name) main.c \
	-I./raylib-5.5_linux_amd64/include/ \
	-I./include/ \
  -L./raylib-5.5_linux_amd64/lib/ \
  -l:libraylib.a \
  -lm

linux:
	mkdir -p $(linux_output_path)/
	gcc \
	-Wall \
	-Wextra \
	-Wno-unused-parameter \
	-o $(linux_output_path)/$(app_name) main.c \
	-I./raylib-5.5_linux_amd64/include/ \
	-I./include/ \
  -L./raylib-5.5_linux_amd64/lib/ \
  -l:libraylib.a \
  -lm

win:
	mkdir -p $(windows_output_path)/
	x86_64-w64-mingw32-gcc \
	-Wall \
	-Wextra \
	-Wno-unused-parameter \
	-static \
	-I./raylib-5.5_win64_mingw-w64/include/ \
	-I./include/ \
  -L./raylib-5.5_win64_mingw-w64/lib/ \
  -l:libraylib.a \
  -l:libraylibdll.a \
  -l:raylib.dll \
  -lm \
	-o $(windows_output_path)/$(app_name).exe main.c
	cp ./raylib-5.5_win64_mingw-w64/lib/raylib.dll $(windows_output_path)/