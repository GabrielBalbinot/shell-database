TARGET = bin/dbview
SRC = $(wildcard src/*.c)
OBJ = $(patsubst src/%.c, obj/%.o, $(SRC))

run: clean default
	./$(TARGET) -n -f ./data.db
	./$(TARGET) -f ./data.db -a 'Fulano,Av. Fernando Machado 38,100'
	./$(TARGET) -f ./data.db -a 'Ciclano,R. Sete de Setembro 45,15'
	./$(TARGET) -f ./data.db -l

default: $(TARGET)

clean:
	rm -f obj/*.o
	rm -f bin/*
	rm -f *.db

$(TARGET): $(OBJ)
	gcc -o $@ $?

obj/%.o : src/%.c
	gcc -c $< -o $@ -Iinclude


