pg:
	@echo "Compilando..."
	gcc -std=c11 -o pagingdemand src/pagingdemand.c
clean:
	@echo "Eliminando compilados"
	rm -f pagingdemand
	rm -f data.txt
