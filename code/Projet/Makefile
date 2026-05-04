# Set bash type
SHELL:=/bin/bash

# Variables
include Makefile.common
SRC = $(notdir $(wildcard $(SRC_DIR)/*.$(FILES_EXT)))
OBJ = $(SRC:%.$(FILES_EXT)=$(OBJ_DIR)/%.o)

# Rules
all: header depend $(OBJ) $(TARGET) footer

header:
	@echo "----------------------------------------------------------"
	@echo "Project codename : $(PROJECT_NAME)"
	@echo "Description      : $(PROJECT_DESCRIPTION)"
	@echo "Version          : $(PROJECT_VERSION)"
	@echo "Last release     : $(PROJECT_LATEST_UPDATE)"
	@echo -e "----------------------------------------------------------\n"

depend:
	@echo -n "+ Generating dependencies : "
ifneq ($(strip $(SRC)),)
	@$(CXX) -I$(INC_DIR) -MM $(SRC:%=$(SRC_DIR)/%) \
	| sed -e 's:^[^ ]:$(OBJ_DIR)/&:' > Makefile.depend
endif
	@echo "ok"

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.$(FILES_EXT)
	@echo -n "+ Compiling $(notdir $@) : "
	@$(CXX) $(CXXFLAGS) $(LDFLAGS) -I$(INC_DIR) -c $< -o $@
	@echo "ok"

$(TARGET): $(OBJ)
	@echo -n "+ Building $@ : "
	@$(CXX) $(CXXFLAGS) -I$(INC_DIR) $^ -o $@ $(LDFLAGS) $(LIBS)
	@echo "ok"

clean: header clean_doc clean_arch
	@echo -n "+ Deleting objects and target : "
	@\rm -f $(OBJ_DIR)/*.o $(TARGET)
	@echo "ok"

doc: header
	@$(MAKE) -C $(DOC_DIR) -s

clean_doc:
	@$(MAKE) -C $(DOC_DIR) -s clean

clean_arch:
	@echo -n "+ Deleting submission : "
	@\rm -rf $(SUBMISSION_DIR)/$(SUBMISSION_FILE)
	@echo "ok"

footer:
	@echo -e "\n----------------------------------------------------------"

include Makefile.depend

.PHONY: doc clean clean_doc clean_arch
