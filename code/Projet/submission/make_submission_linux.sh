#!/bin/bash

# Variables
SUB_DIR="submission"
SUB_FILE="submission.zip"
FILES_EXT="cpp"
SRC_DIR="src"
INC_DIR="include"
OBJ_DIR="obj"

# We move to parent directory
cd .. &> /dev/null

# We check the existence of directories
[ ! -d "${SRC_DIR}" ] && echo "error: source directory ${SRC_DIR} not found"
[ ! -d "${INC_DIR}" ] && echo "error: include directory ${INC_DIR} not found"
[ ! -d "${OBJ_DIR}" ] && echo "error: objects directory ${OBJ_DIR} not found"
[ ! -d "${SUB_DIR}" ] && echo "error: submission directory ${SUB_DIR} not found"

# We delete objects and executable
make clean

# We create the zip archive of the submission
echo -n "+ Creating submission archive : "
zip -r9 "${SUB_DIR}/${SUB_FILE}" "${SRC_DIR}"/*."${FILES_EXT}" "${INC_DIR}"/*.h Makefile* "${OBJ_DIR}" &> /dev/null
echo "ok"

# We go back to the submission directory
cd - &> /dev/null
