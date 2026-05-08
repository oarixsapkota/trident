CMAKE = cmake
BUILD_DIR_DEBUG = build/debug
BUILD_DIR_RELEASE = build/release

.PHONY: all debug release clean test-debug test-release format

all: debug release

# Create Debug build
debug:
	@mkdir -p $(BUILD_DIR_DEBUG)
	@cd $(BUILD_DIR_DEBUG) && $(CMAKE) -DCMAKE_BUILD_TYPE=Debug ../..
	@cd $(BUILD_DIR_DEBUG) && $(MAKE)

# Create Release build
release:
	@mkdir -p $(BUILD_DIR_RELEASE)
	@cd $(BUILD_DIR_RELEASE) && $(CMAKE) -DCMAKE_BUILD_TYPE=Release ../..
	@cd $(BUILD_DIR_RELEASE) && $(MAKE)

# Clean all builds
clean:
	rm -rf build/

# Format src files
format:
	clang-format -i $(wildcard src/*.c) $(wildcard src/*/*.c) $(wildcard include/*.h)