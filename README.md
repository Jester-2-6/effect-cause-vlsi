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
./project <banchmark_name>
```

## Example

```bash
./project c17
./project c880
```

## Notes on running on UNIX machines

- The code was developed on codespace and atalanta installation of UNIX is on a different path. Set the `IS_UNIX_LAB` flag of `graph.h` to 1 when running on UNIX machines.
