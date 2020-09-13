pg:
	@echo "Compilando..."
	gcc -std=c11 -o main src/pagingdemand.c
clean:
	@echo "Eliminando compilados"
	rm -f main
