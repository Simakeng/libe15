
CHEAT_HEADER = $(BUILD_DIR)/cheat.h

TEST_SUITS = $(shell find test -name "*.c")

TARGET ?= all

# check if target is set
ifeq ($(TARGET),all)
SELECTED_TEST_SUITS := $(TEST_SUITS)
else
SELECTED_TEST_SUITS := $(filter $(TARGET:%=test/test.%.c),$(TEST_SUITS))
endif

# check if target is valid
ifeq ("$(SELECTED_TEST_SUITS)", "")
$(error "Target '$(TARGET)' is not found!")
endif

# cheat testing framework
$(CHEAT_HEADER):
	@echo "+ DL    $@"
	@mkdir -p $(dir $@)
	@curl https://raw.githubusercontent.com/Tuplanolla/cheat/master/cheat.h 1>$(BUILD_DIR)/cheat.h 2>/dev/null

# library source files
include scripts/source.mk

include scripts/config.mk

# include files
INC_DIRS ?= 
INC_DIRS += $(abspath $(BUILD_DIR))
INC_DIRS += $(WORKING_DIR)

CFLAGS += $(addprefix -I,$(INC_DIRS))

# Compilation patterns
$(BUILD_DIR)/%.o: %.c
	@echo "+ CC    $<"
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c -o $@ $<
	$(call call_fixdep, $(@:.o=.d), $@)

$(BUILD_DIR)/%.o: %.cpp
	@echo "+ CCX   $<""
	@mkdir -p $(dir $@)
	@$(CXX) $(CFLAGS) $(CXXFLAGS) -c -o $@ $<
	$(call call_fixdep, $(@:.o=.d), $@)

# Generate test-suits
$(BUILD_DIR)/test/% : $(TESTS_DIR)/%.c $(CHEAT_HEADER) $(OBJECTS_SRC)
	@echo "+ LD    $@"
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -o $@ $< $(OBJECTS_SRC)

TEST_TARGET := $(addprefix $(BUILD_DIR)/,$(SELECTED_TEST_SUITS:%.c=%))

OBJS-y += $(addprefix $(BUILD_DIR)/test/,$(obj-test))
