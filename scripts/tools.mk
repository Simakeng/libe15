# tools.mk
# Tools and utilities

AUTO_DEP := $(BUILD_DIR)/tools/autodep

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

