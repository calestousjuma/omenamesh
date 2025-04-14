
CFLAGS :=  -fPIC -Iinclude -Isrc 
LDFLAGS := -shared
BUILD_DIR := build
ENTRY := boot

SRC_DIR := src
VIRT_DIR := virtual
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
VIRT_FILES := $(wildcard $(VIRT_DIR)/*.c)
ALL_SRC := $(SRC_FILES) $(VIRT_FILES)

OBJ_FILES := $(patsubst %.c, $(BUILD_DIR)/%.o, $(notdir $(ALL_SRC)))

SHARED_LIB := $(BUILD_DIR)/libomena.so


# CFLAGS += ./Iinclude
all: $(SHARED_LIB) | boot
	@$(CC) -I./include -o $(BUILD_DIR)/$(ENTRY) $(ENTRY)/*.c ./$< && ./$(BUILD_DIR)/$(ENTRY)
	@#cp $(BUILD_DIR)/$< bindings
$(SHARED_LIB): $(OBJ_FILES) | $(BUILD_DIR)
	@echo "Linking shared library..."
	@$(CC) $(LDFLAGS) -o $@ $(OBJ_FILES)
	@echo "Build complete: $@"

$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c | $(BUILD_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: $(VIRT_DIR)/%.c | $(BUILD_DIR)
	@$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR):
	@mkdir -p $(BUILD_DIR)

 

clean:
	rm -rf $(BUILD_DIR)

.PHONY: all clean build
