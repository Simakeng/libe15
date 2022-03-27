BUILD_DIR = build

include scripts/doc.mk

all: doc

doc: $(DOCUMENT_TARGET)

clean:
	@rm -rf $(BUILD_DIR)

help:
	@echo "Usage: make [target]"
	@echo "  target: doc     - generate documentation for this project"
	@echo "  target: clean   - clean all generated files"
	@echo "  target: all     - build all target"
	@echo "  target: help    - display this help message"