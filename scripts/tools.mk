# tools.mk
# Tools and utilities

AUTO_DEP := $(BUILD_DIR)/tools/autodep

DENPENDENCIES = $(wildcard *.d)

-include $(DENPENDENCIES)

$(AUTO_DEP) : $(TOOLS_SRC_DIR)/autodep/autodep.c
	@echo "+ CC    $<"
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -o $@ $<

define call_fixdep
	@echo "x DEP   $(1)"
	@$(AUTO_DEP) "$(1)" "$(2)" "$(3)"
endef

