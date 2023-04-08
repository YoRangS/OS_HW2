CC=gcc
CFLAGS=-c
TARGET=cimin
OBJS=b_reduced j_reduced lx_reduced
CRASH_INPUT?=balance/testcases/fail
DET_STRING?="Unbalanced"
EXE?=$(FILEPATH)balance/balance
ARGU?=""
FILEPATH=../OperatingSystem/

all: $(TARGET)

$(TARGET): cimin.o
	$(CC) -o $(TARGET) cimin.o

cimin.o: cimin.c
	$(CC) $(CFLAGS) cimin.c

Balance: $(TARGET)
	cd $(FILEPATH)balance && ./build.sh
	chmod +x $(FILEPATH)$(CRASH_INPUT)
	./$(TARGET) -i $(FILEPATH)$(CRASH_INPUT) -m $(DET_STRING) -o b_reduced $(EXE) $(ARGU)

Jsmn: $(TARGET)
	cd $(FILEPATH)jsmn && ./build.sh
	$(info CRASH_INPUT value: $(CRASH_INPUT))
	$(eval CRASH_INPUT := jsmn/testcases/crash.json)
	$(info CRASH_INPUT value: $(CRASH_INPUT))
	chmod +x $(FILEPATH)$(CRASH_INPUT)
	$(eval DET_STRING := "heap-buffer-overflow")
	$(eval EXE := $(FILEPATH)jsmn/jsondump)
	./$(TARGET) -i $(FILEPATH)$(CRASH_INPUT) -m $(DET_STRING) -o j_reduced $(EXE) $(ARGU)

Libxml2: $(TARGET)
	cd $(FILEPATH)libxml2 && ./build.sh
	$(eval CRASH_INPUT := libxml2/testcases/crash.xml)
	chmod +x $(FILEPATH)$(CRASH_INPUT)
	$(eval DET_STRING := "SEGV on unknown address")
	$(eval EXE := $(FILEPATH)libxml2/xmllint)
	$(eval ARGU := "--recover --postvalid - < testcases/crash.xml")
	./$(TARGET) -i $(FILEPATH)$(CRASH_INPUT) -m $(DET_STRING) -o lx_reduced $(EXE) $(ARGU)

clean:
	rm -rf $(OBJS) $(TARGET) *.o