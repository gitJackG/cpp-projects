SHELL = bash 

ifeq ($(OS),Windows_NT)
    RM = del /Q
    TARGET_EXT = .exe
else
    RM = rm -f
    TARGET_EXT =
endif

CXX = g++
CXXFLAGS = -Wall -g

VULKAN_PATH = C:/VulkanSDK/1.4.335.0
VULKAN_INC  = -I$(VULKAN_PATH)/Include
VULKAN_LIB  = -L$(VULKAN_PATH)/Lib -lvulkan-1

GLFW_PATH = C:/glfw/glfw-3.4.bin.WIN64
GLFW_INC  = -I$(GLFW_PATH)/include
GLFW_LIB  = -L$(GLFW_PATH)/lib-mingw-w64 -lglfw3

GLM_INC = -I C:/glm-1.0.3

WIN_LIBS = -lgdi32 -luser32 -lkernel32

GLSLC = C:\VulkanSDK\1.4.335.0\Bin\glslc.exe 

TARGET = main
SRCS = main.cpp lve_window.cpp lve_pipeline.cpp lve_device.cpp lve_swap_chain.cpp lve_model.cpp lve_game_object.cpp lve_renderer.cpp simple_render_system.cpp first_app.cpp
OBJS = $(SRCS:.cpp=.o)

VERT_SHADERS := $(wildcard shaders/*.vert)
FRAG_SHADERS := $(wildcard shaders/*.frag)

SPV_SHADERS := $(VERT_SHADERS:.vert=.vert.spv) $(FRAG_SHADERS:.frag=.frag.spv)

INCLUDES = $(VULKAN_INC) $(GLFW_INC) $(GLM_INC)
LIBS = $(VULKAN_LIB) $(GLFW_LIB) $(WIN_LIBS)

all: $(TARGET)

$(TARGET): $(SPV_SHADERS) $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS) $(LIBS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

%.vert.spv: %.vert
	$(GLSLC) $< -o $@

%.frag.spv: %.frag
	$(GLSLC) $< -o $@

run: $(TARGET)
	$(TARGET)$(TARGET_EXT)

clean:
	$(RM) $(TARGET)$(TARGET_EXT) $(OBJS) 

.PHONY: all run clean
