# effect-cause-vlsi

Input stimuli generation for effect-cause analysis to perform diagnosis

## Project Board

View the project board [here](https://talented-pen-f74.notion.site/cda3ebff01bb4e7a8b2c7296ddc31d42?v=b34cafafcace45a6b26e21411567bbf2&pvs=74)

## Compilation

Run the following command in the project root to compile the code:

```bash
make
```

## Execution

Run the following command in the project root to execute the code:

```bash
./project <banchmark_name> <group_size> <run_p1> <run_p2> <run_p3> 
```

Note: The `group_size`, `run_p1`, `run_p2`, `run_p3` are optional arguments. If not provided, the default value is 1.

## Example

```bash
./project c17 3 1 1 1
./project c880 2 0 0 1
./project c1908
```

## Notes on running on UNIX machines

- The code was developed on codespace and atalanta installation of UNIX is on a different path. Set the `IS_UNIX_LAB` flag of `graph.h` to 1 when running on UNIX machines.

## Persist Directory

- The `persist` directory contains the output of the code for the benchmarks. The output is in the form of a `.resolutions` file.
