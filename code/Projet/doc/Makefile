# Variables
include ../Makefile.common

DOXYGEN_FILE = $(wildcard *.doxygen)
DOCS_DIRS    = `find . -maxdepth 1 -type d -printf "%f " | cut -d' ' -f2- -`

# Rules
all:
	@echo -n "+ Generating doc : "
	@sed -i "s/PROJECT_NAME.*/PROJECT_NAME           = $(PROJECT_NAME)/" $(DOXYGEN_FILE)
	@sed -i "s/PROJECT_NUMBER.*/PROJECT_NUMBER         = $(PROJECT_VERSION)/" $(DOXYGEN_FILE)
	@sed -i "s/GENERATE_HTML.*/GENERATE_HTML          = $(HTML_OUTPUT)/" $(DOXYGEN_FILE)
	@sed -i "s/GENERATE_LATEX.*/GENERATE_LATEX          = $(LATEX_OUTPUT)/" $(DOXYGEN_FILE)
	@doxygen $(DOXYGEN_FILE) 2>/dev/null >/dev/null
	@echo "ok"

clean:
	@echo -n "+ Deleting doc : "
	@for Dir in $(DOCS_DIRS); do \
		\rm -rf $$Dir/*; \
	done
	@echo "ok"

.PHONY: all clean
