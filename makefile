BIN_DIR ?= ./bin
SRC_DIR = ./src
INC_DIR = ./include
DEP_DIR = ./deps

SRC_FILES := $(shell find $(SRC_DIR) -name '*.cpp')
DEP_FILES := $(SRC_FILES:$(SRC_DIR)/%.cpp=$(DEP_DIR)/%.d)
BIN_FILES := $(SRC_FILES:$(SRC_DIR)/%.cpp=$(BIN_DIR)/%)

CC = g++
CFLAGS = -O2 -I$(INC_DIR) -MT $@ -MMD -MP -MF $(DEP_DIR)/$(@F).d -Wall

.PHONY: all
all: $(BIN_FILES) | $(BIN_DIR)

#
# Magic to handle header dependencies
#
$(DEP_FILES):
include $(wildcard $(DEP_FILES))

#
# Make required directories
#
$(DEP_DIR):
	mkdir -p $(DEP_DIR)

$(BIN_DIR):
	mkdir -p $(BIN_DIR)

#
# Compilation
#
$(BIN_DIR)/066: $(SRC_DIR)/066.cpp $(INC_DIR)/* $(DEP_DIR)/066.d | $(BIN_DIR) $(DEP_DIR)
	$(CC) $(CFLAGS) $< -o $@ -lgmp

$(BIN_DIR)/146: $(SRC_DIR)/146.cpp $(INC_DIR)/* $(DEP_DIR)/146.d | $(BIN_DIR) $(DEP_DIR)
	$(CC) $(CFLAGS) $< -o $@

$(BIN_DIR)/%: $(SRC_DIR)/%.cpp $(DEP_DIR)/%.d | $(BIN_DIR) $(DEP_DIR)
	$(CC) $(CFLAGS) $< -o $@

#
# Cleanup
#
.PHONY: clean
clean:
	rm -r $(DEP_DIR)
	rm -r $(BIN_DIR)

#
# Targets to build and run solution programs
#
.PHONY: 035
035: $(BIN_DIR)/035
	$(BIN_DIR)/035

.PHONY: 064
064: $(BIN_DIR)/064
	$(BIN_DIR)/064

.PHONY: 066
066: $(BIN_DIR)/066
	$(BIN_DIR)/066

.PHONY: 070
070: $(BIN_DIR)/070
	$(BIN_DIR)/070

.PHONY: 093
093: $(BIN_DIR)/093
	$(BIN_DIR)/093

.PHONY: 096
096: $(BIN_DIR)/096
	$(BIN_DIR)/096

.PHONY: 118
118: $(BIN_DIR)/118
	$(BIN_DIR)/118

.PHONY: 134
134: $(BIN_DIR)/134
	$(BIN_DIR)/134

.PHONY: 146
146: $(BIN_DIR)/146
	$(BIN_DIR)/146

.PHONY: 148
148: $(BIN_DIR)/148
	$(BIN_DIR)/148

.PHONY: 212
212: $(BIN_DIR)/212
	$(BIN_DIR)/212

.PHONY: 214
214: $(BIN_DIR)/214
	$(BIN_DIR)/214

.PHONY: 451
451: $(BIN_DIR)/451
	$(BIN_DIR)/451
