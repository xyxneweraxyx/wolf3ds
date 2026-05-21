##
## EPITECH PROJECT, 2025
## wolf3d
## File description:
## Build, cleanup, and style targets for wolf3d.
##

TARGET := wolf3d
WIN_TARGET := wolf3d.exe
SOURCE_DIR := source

CC := clang
WIN_CC := clang
CFLAGS := -Wall -Wextra -g3 -O2
LDFLAGS :=
LDLIBS := -lcsfml-window -lcsfml-system -lcsfml-graphics -lcsfml-audio -lm

WIN_CSFML_PREFIX ?= C:/CSFML
WIN_CFLAGS := $(CFLAGS) -Wno-deprecated-declarations
WIN_LDFLAGS := -L$(WIN_CSFML_PREFIX)/lib/gcc
WIN_LDLIBS := -lcsfml-window -lcsfml-system -lcsfml-graphics -lcsfml-audio -lwinpthread -lm

rwildcard = $(foreach d,$(wildcard $(1)/*),$(call rwildcard,$d,$(2)) $(filter $(subst *,%,$(2)),$d))

SRC := $(sort $(call rwildcard,$(SOURCE_DIR),*.c))
HDR := $(sort $(call rwildcard,$(SOURCE_DIR),*.h))
INCLUDE_DIRS := $(sort $(dir $(HDR)))

CPPFLAGS := -D_POSIX_C_SOURCE=199309L $(addprefix -I,$(INCLUDE_DIRS))
WIN_CPPFLAGS := $(CPPFLAGS) -I$(WIN_CSFML_PREFIX)/include

OBJ := $(patsubst %.c,obj/%.o,$(SRC))
OBJ_DIRS := $(sort $(dir $(OBJ)))
WIN_OBJ := $(patsubst %.c,win_obj/%.o,$(SRC))
WIN_OBJ_DIRS := $(sort $(dir $(WIN_OBJ)))

all: $(TARGET)

.PHONY: all clean fclean re print_src epiclang epiclang_all win win_clean win_fclean win_re

$(OBJ_DIRS):
	mkdir -p $@

$(WIN_OBJ_DIRS):
	powershell -NoProfile -Command "New-Item -ItemType Directory -Force -Path '$@' | Out-Null"

obj/%.o: %.c | $(OBJ_DIRS)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

win_obj/%.o: %.c | $(WIN_OBJ_DIRS)
	$(WIN_CC) $(WIN_CPPFLAGS) $(WIN_CFLAGS) -c $< -o $@

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LDFLAGS) $(LDLIBS)

$(WIN_TARGET): $(WIN_OBJ)
	$(WIN_CC) $(WIN_OBJ) -o $@ $(WIN_LDFLAGS) $(WIN_LDLIBS)
	powershell -NoProfile -Command "if (!(Test-Path '$(WIN_CSFML_PREFIX)\\bin')) { Write-Error 'CSFML bin directory not found: $(WIN_CSFML_PREFIX)\\bin'; exit 1 }; Copy-Item -Force '$(WIN_CSFML_PREFIX)\\bin\\*.dll' '.'"

win: $(WIN_TARGET)

clean:
	rm -rf obj/
	rm -rf $(shell find . -name '*.o')
	rm -rf $(shell find . -name '*.c~')
	rm -rf $(shell find . -name '*.h~')
	rm -rf $(shell find . -name '*#*')
	rm -rf $(shell find . -name '*.pch')
	rm -rf $(shell find . -name '*.out')
	rm -rf $(shell find . -name '*~')
	rm -rf $(shell find . -name '*.Identifier')

fclean: clean
	rm -rf $(TARGET)

re: fclean all

win_clean:
	powershell -NoProfile -Command "if (Test-Path 'win_obj') { Remove-Item -Recurse -Force 'win_obj' }"

win_fclean: win_clean
	powershell -NoProfile -Command "if (Test-Path '$(WIN_TARGET)') { Remove-Item -Force '$(WIN_TARGET)' }"

win_re: win_fclean win

CFLAGS  += -Wall -Wextra -std=c99
LDFLAGS += -lcsfml-graphics -lcsfml-window -lcsfml-system -lcsfml-audio -lm
LDFLAGS += -Wl,-rpath,/usr/local/opt/sfml@2.6.1/Frameworks
CPPFLAGS += -DCSFML_NO_DEPRECATED_WARNINGS

mac: $(TARGET)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS) $(LDLIBS)

print_src:
	$(info $(SRC))

epiclang: epiclang_all

epiclang_all:
	epiclang $(CPPFLAGS) $(CFLAGS) -fsyntax-only $(SRC) $(HDR)
