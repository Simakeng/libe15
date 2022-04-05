# add this to prevent removing intermediate files.
.SECONDARY:

# Default target
all: doc test

# Config
include scripts/config.mk

# Rules for search target
include scripts/source.mk

# Rules for build documentation
include scripts/doc.mk

# Rules for build and unit test
include scripts/test.mk

doc: $(DOCUMENT_TARGET)

test: $(TEST_TARGET_LOGS)
	@echo done!

clean:
	@rm -rf $(BUILD_DIR)

help:
	@echo "Usage: make [target]"
	@echo "  target: doc     - generate documentation for this project"
	@echo "  target: clean   - clean all generated files"
	@echo "  target: all     - build all target"
	@echo "  target: help    - display this help message"

.PHONY: doc test