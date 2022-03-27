
DOCUMENT_DIR = $(BUILD_DIR)/doc/html
DOCUMENT_TARGET = $(DOCUMENT_DIR)/index.html

$(DOCUMENT_TARGET) : $(CODE_SRC)
	@mkdir -p $(DOCUMENT_DIR)
	@doxygen Doxyfile