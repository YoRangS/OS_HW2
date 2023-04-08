CC=gcc
CFLAGS=-c
TARGET=cimin
OBJS=b_reduced j_reduced lx_reduced
CRASH_INPUT?=balance/testcases/fail
DET_STRING?="Unbalanced"
EXE?=balance
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
	export CRASH_INPUT=jsmn/testcases/crash.json
	chmod +x $(FILEPATH)$(CRASH_INPUT)
	DET_STRING:="heap-buffer-overflow"
	EXE:=jsondump
	./$(TARGET) -i $(FILEPATH)$(CRASH_INPUT) -m $(DET_STRING) -o j_reduced $(EXE) $(ARGU)

Libxml2: $(TARGET)
	cd $(FILEPATH)jsmn && ./build.sh
	export CRASH_INPUT=libxml2/testcases/crash.xml
	chmod +x $(FILEPATH)$(CRASH_INPUT)
	DET_STRING:="SEGV on unknown address"
	EXE:=xmllint
	ARGU:="--recover --postvalid - < testcases/crash.xml"
	./$(TARGET) -i $(FILEPATH)$(CRASH_INPUT) -m $(DET_STRING) -o lx_reduced $(EXE) $(ARGU)

clean:
	rm -rf $(OBJS) $(TARGET) *.o