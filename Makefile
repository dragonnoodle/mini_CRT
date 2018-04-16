
buildlib:

	gcc -c -fno-builtin -nostdlib -fno-stack-protector entry.c malloc.c stdio.c string.c printf.c
	ar -rs minicrt.a malloc.o printf.o stdio.o string.o
    
    
buildbin:
	gcc -c -ggdb -fno-builtin -nostdlib -fno-stack-protector test.c
	ld -static -e mini_crt_entry entry.o test.o minicrt.a -o test

run:
	./test arg1 arg2 arg3
	
	

		
	
		
	
		
	
		
	
		
	
		
	
	