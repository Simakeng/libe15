
HEADER_FILES := $(shell find $(SOURCE_DIR) -name '*.h')
C_SOURCE_FILES := $(shell find $(SOURCE_DIR) -name '*.c')
CXX_SOURCE_FILES := $(shell find $(SOURCE_DIR) -name '*.cpp')

INC_DIRS := $(abspath $(sort $(dir $(HEADER_FILES))))

# replace .c to .o
OBJECTS_SRC += $(C_SOURCE_FILES:%.c=$(BUILD_DIR)/%.o)

# replace .cpp to .o
OBJECTS_SRC += $(CXX_SOURCE_FILES:%.cpp=$(BUILD_DIR)/%.o)

OBJECTS_SRC := $(sort $(OBJECTS_SRC))