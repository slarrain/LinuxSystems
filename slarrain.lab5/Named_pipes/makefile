all: my_npipe_reader my_npipe_writer

my_npipe_reader: my_npipe_reader.c
	gcc -Wall my_npipe_reader.c -o my_npipe_reader

my_npipe_writer: my_npipe_writer.c
	gcc -Wall my_npipe_writer.c -o my_npipe_writer
clean:
	rm my_npipe_reader my_npipe_writer
