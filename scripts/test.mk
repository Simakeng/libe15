.SECONDEXPANSION:
CHEAT_HEADER := $(BUILD_DIR)/cheat.h
CHEAT_URL := https://raw.githubusercontent.com/Tuplanolla/cheat/master/cheat.h
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

.PHONY : cheat_header_file_check

cheat_header_file_check: 
	@$(SHELL) -c 'if [ ! -f $(CHEAT_HEADER) ]; then exit 0; fi; cheat_state=$$(stat -L -c %s $(CHEAT_HEADER)); if [ $$cheat_state -le 100 ]; then rm $(CHEAT_HEADER); echo "Invalid cheat.h header! Removed."; fi'


# cheat testing framework
$(CHEAT_HEADER): cheat_header_file_check
	@mkdir -p $(dir $@)
	@$(SHELL) -c 'if [ -f $(CHEAT_HEADER) ]; then exit 0; fi; echo "+ DL    $@"; curl $(CHEAT_URL) 1>$(CHEAT_HEADER) 2>/dev/null; if [ 0 -ne $$? ]; then echo "Download cheat.h Failed!"; exit 1; fi'

# library source files
include scripts/source.mk

include scripts/config.mk

# include files
INC_DIRS ?= 
INC_DIRS += $(abspath $(BUILD_DIR))
INC_DIRS += $(WORKING_DIR)

CFLAGS += $(addprefix -I,$(INC_DIRS))

CFLAGS := -g $(CFLAGS)

# Compilation patterns
$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.c $(AUTO_DEP)
	@echo "+ CC    $<"
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -MMD -MF $(@:%=%.d) -c -o $@ $< $(LDLIBS)
	$(call call_fixdep,$(@:%=%.d),$@,$(CFLAGS))

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp $(AUTO_DEP)
	@echo "+ CCX   $<""
	@mkdir -p $(dir $@)
	@$(CXX) $(CFLAGS) $(CXXFLAGS) -MMD -MF $(@:%=%.d) -c -o $@ $< $(LDLIBS)
	$(call call_fixdep,$(@:%=%.d),$@,$(CFLAGS))

# Generate test-suits
$(BUILD_DIR)/test/test.%: $(TESTS_DIR)/test.%.c build/$$*/$$*.o $(CHEAT_HEADER) $(AUTO_DEP) 
	@echo "+ LD    $@"
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -MMD -MF $(@:%=%.d) -o $@ $(sort $(abspath $(filter %.c %.o %.s ,$^))) $(LDLIBS)
	$(call call_fixdep,$(@:%=%.d), $@,$(CFLAGS))

# Execute tests:
$(BUILD_DIR)/$(EXECUTION_LOG_DIR)/%.log: $(BUILD_DIR)/test/%
	@echo ""
	@echo "TEST  $(@:%.log=%) START"
	@./$<
	@echo "TEST  $(@:%.log=%) END"

# test target file path
TEST_TARGET := $(addprefix $(BUILD_DIR)/,$(SELECTED_TEST_SUITS:%.c=%))

TEST_TARGET_LOGS := $(TEST_TARGET:$(BUILD_DIR)/test/%=$(BUILD_DIR)/$(EXECUTION_LOG_DIR)/%.log)
