CC = gcc
CFLAGS = -Wall -Wextra -g

all:
	@echo ""
	@echo "BUILDING WITH:"
	@echo "CC:	" $(CC)
	@echo "CFLAGS:	" ${CFLAGS}
	@echo "..."
	@echo ""

	$(CC) $(LDLIBS) $(LDFLAGS) systrack.c -o systrack $(CFLAGS)
