# the compiler to use
CC = gcc

# compiler flags:
CFLAGS  = -g
  

  
# the name to use for both the target source file, and the output file:
TARGET = shell
  
all: $(TARGET)
  
$(TARGET): $(TARGET).c
	$(CC) $(CFLAGS) -o $(TARGET) $(TARGET).c 
