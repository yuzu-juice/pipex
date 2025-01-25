usage

```
./pipex src/pipex.c "ls -l" "wc -l" outfile
./pipex src/pipex.c "grep a" "wc -l" outfile
./pipex file1 cmd1 cmd2 cmd3 ... cmdn file2./pipex src/pipex.c "grep a" "wc -l" outfile
```

2 cmds
0
    in infile
    out fd0
1
    in fd0
    out outfile

3 cmds
0
    in infile
    out fd0
1
    in fd0
    out fd1
2
    in fd1
    out outfile

4 cmds
0
    in infile
    out fd0
1
    in fd0
    out fd1
2
    in fd1
    out fd0
3
    in fd0
    out outfile


valgrind --track-fds=yes ./pipex src/pipex.c "grep a" "wc -l" outfile
valgrind --leak-check=full --show-leak-kinds=all ./pipex src/pipex.c "grep a" "wc -l" outfile