usage

```
./pipex src/pipex.c "ls -l" "wc -l" outfile
./pipex src/pipex.c "grep a" "wc -l" outfile
./pipex file1 cmd1 cmd2 cmd3 ... cmdn file2./pipex src/pipex.c "grep a" "wc -l" outfile
```

```
./pipex src/pipex.c "grep a" "wc -l" outfile
./pipex src/pipex.c "grep a" "grep b" "wc -l" outfile
valgrind --trace-children=yes ./pipex Makefile "grep a" "wc -l" outfile
valgrind --track-fds=yes ./pipex Makefile "grep a" "wc -l" outfile
valgrind --leak-check=full --show-leak-kinds=all ./pipex Makefile "grep a" "wc -l" outfile
```
