# effect-cause-vlsi
Input stimuli generation for effect-cause analysis to perform diagnosis

## Compilation
Run the following command in the project root to compile the code:
```bash
make
```

## Execution
Run the following command in the project root to execute the code:
```bash
./project <banchmark_name>
```

## Example
```bash
./project c17
./project c880
```

## Notes on running on UNIX machines
- The code was developed on codespace and atalanta installation of UNIX is on a different path. Change the following part of `user.c` appropriately when running on UNIX machines:

```c
	// Uncomment to run on codespace
	sprintf(command, "/home/codespace/Atalanta/atalanta -D %d -f %s -t %s %s", MAX_PATTERNS, error, result, bench);

	// uncomment to run in unix lab
	// sprintf(command, "/opt/net/apps/atalanta/atalanta -D %d -f %s -t %s %s", MAX_PATTERNS, error, result, bench);
```
