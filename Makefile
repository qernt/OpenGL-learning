APP_NAME = OpenGLApplication
BUILD_DIR = ./
CPP_FILES = ./src/*.cpp
C_FILES = ./src/*.c

APP_DEFINES :=
APP_INCLUDES := -I./src/include/GLFW -framework Cocoa -framework OpenGL -framework IOKit -I./src/include
APP_LINKERS := -L./src/include/GLFW/lib -L/usr/local/lib -lglfw3

build:
	clang++ $(CPP_FILES) $(C_FILES) -o $(BUILD_DIR)/$(APP_NAME) $(APP_INCLUDES) $(APP_LINKERS)

clean:
	rm -f $(BUILD_DIR)/$(APP_NAME)

.PHONY: all clean