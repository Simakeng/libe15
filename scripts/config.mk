# config.mk
# configs and envs

BUILD_DIR := build
EXECUTION_LOG_DIR := exec_logs
SOURCE_DIR := src
TESTS_DIR := test
WORKING_DIR := $(abspath $(shell pwd))

DOCUMENT_DIR := $(BUILD_DIR)/doc/html
DOCUMENT_TARGET := $(DOCUMENT_DIR)/index.html

CC := gcc
CXX := g++
