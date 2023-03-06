
HEADER_FILES := $(shell find $(SOURCE_DIR) -name '*.h')
C_SOURCE_FILES := $(shell find $(SOURCE_DIR) -name '*.c')
CXX_SOURCE_FILES := $(shell find $(SOURCE_DIR) -name '*.cpp')

INC_DIRS := $(abspath $(sort $(dir $(HEADER_FILES))))

# replace .c to .o
COMPILED_OBJECTS += $(C_SOURCE_FILES:$(SOURCE_DIR)/%.c=$(BUILD_DIR)/%.o)

# replace .cpp to .o
COMPILED_OBJECTS += $(CXX_SOURCE_FILES:$(SOURCE_DIR)/%.cpp=$(BUILD_DIR)/%.o)

COMPILED_OBJECTS := $(sort $(COMPILED_OBJECTS))