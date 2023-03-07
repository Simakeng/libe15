# config.mk
# configs and envs

BUILD_DIR := build
EXECUTION_LOG_DIR := logs
SOURCE_DIR := src
TOOLS_SRC_DIR := tools
TESTS_DIR := test
WORKING_DIR := $(abspath $(shell pwd))

DOCUMENT_DIR := $(BUILD_DIR)/doc/html
DOCUMENT_TARGET := $(DOCUMENT_DIR)/index.html

CC := gcc
CXX := g++

SHELL := sh

