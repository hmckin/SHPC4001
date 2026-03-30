CC = gcc
CFLAGS = -I/opt/homebrew/opt/openblas/include
LDFLAGS = -L/opt/homebrew/opt/openblas/lib -lopenblas

matrix_solver: matrix_solver.c
	$(CC) $(CFLAGS) matrix_solver.c -o matrix_solver $(LDFLAGS)

clean:
	rm -f matrix_solver