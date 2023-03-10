# rule for build documentation
$(DOCUMENT_TARGET) : $(CODE_SRC) Doxyfile
	@mkdir -p $(DOCUMENT_DIR)
	@echo "+ DOXY  project"
	@doxygen Doxyfile 1 > /dev/null

