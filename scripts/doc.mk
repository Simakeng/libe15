# rule for build documentation
$(DOCUMENT_TARGET) : $(CODE_SRC)
	@mkdir -p $(DOCUMENT_DIR)
	@echo "    DOXY      project"
	@doxygen Doxyfile

