# From eliemichel/WebGPU-distribution
# https://github.com/eliemichel/WebGPU-distribution/blob/main/cmake/apply_patch_idempotent.cmake
#
# MIT License
# Copyright (c) 2022-2025 Elie Michel and the wgpu-native authors

# This is a CMake file meant to be called in script mode. It applies a patch in
# a way that is robust to re-applying it multiple times.
#
# Usage:
#   cmake -DPATCH_FILE=path/to/patch.diff -P apply_patch_idempotent.cmake
#
# Patch is applied in the current working directory.

message(STATUS "Applying patch from '${PATCH_FILE}'...")

set(PATCH_CMD git apply --ignore-space-change --ignore-whitespace ${PATCH_FILE})

# Test reverse patch
execute_process(
	RESULT_VARIABLE EXIT_CODE
	ERROR_VARIABLE STDERR
	COMMAND git apply --ignore-space-change --ignore-whitespace "${PATCH_FILE}" --reverse --check
)

if (EXIT_CODE EQUAL 0)
	# Reverse patch can be applied, which means the patch has already been applied.
	message(STATUS "Patch was already applied")
else()
	execute_process(COMMAND git apply --ignore-space-change --ignore-whitespace ${PATCH_FILE})
endif()
