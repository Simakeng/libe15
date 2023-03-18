# tools.mk
# Tools and utilities

AUTO_DEP := $(BUILD_DIR)/tools/autodep
CORDIC := $(BUILD_DIR)/tools/cordic

CORDIC_HEADER := $(BUILD_DIR)/cordic.h

DENPENDENCIES := $(shell find . -name '*.d')

-include $(DENPENDENCIES)

$(AUTO_DEP) : $(TOOLS_SRC_DIR)/autodep/autodep.c
	@echo "+ CC    $<"
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -o $@ $<

define call_fixdep
	@echo "x DEP   $(1)"
	@$(AUTO_DEP) "$(1)" "$(2)" unused > $(1).temp
	@mv $(1).temp $(1)
endef

$(CORDIC) : $(TOOLS_SRC_DIR)/cordic/cordic.c
	@echo "+ CC    $<"
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -o $@ $<

$(CORDIC_HEADER) : $(CORDIC)
	@echo "+ GEN   $@"
	@mkdir -p $(dir $@)
	@$(CORDIC) > $@