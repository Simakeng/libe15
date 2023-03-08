# libe15-pid need libe15-fpa
$(BUILD_DIR)/libe15-pid/libe15-pid.o : $(BUILD_DIR)/libe15-fpa/libe15-fpa.o
$(BUILD_DIR)/test/test.libe15-pid : $(BUILD_DIR)/libe15-fpa/libe15-fpa.o