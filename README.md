# arptool

`arptool` is a CLI wrapper for [libarp][1]. More information will be added soon.

### Usage

```
arptool <verb> [args] <target path>
```

Valid verbs are `pack`, `unpack`, `list`, and `help`.

The semantics of the target path argument vary between verbs, but the given path will always be used as input to the
program of some kind.

#### Verbs

| Name | Description |
| :-- | :-- |
| `pack` | Creates a new package, using the target path as input. |
| `unpack` | Unpacks the package located at the target path. |
| `list` | Lists the resources contained by the package located at the target path. |

#### Global params

| Shorthand | Longhand | Description | Default |
| :-- | :-- | :-- | :-- |
| `-?`, `-h` | `--help` | Prints help information for the program or the provided verb if applicable. | N/A |
| `-o <path>` | `--output=<path>` | The path to direct output files to. This must be a directory if it already exists. | The current working directory. |

#### `pack` params

The following parameters are valid only for the `pack` verb.

| Shorthand | Longhand | Description | Default |
| :-- | :-- | :-- | :-- |
| `-c <type>` | `--compression=<type>` | Compression type. Currently, the only supported values are `deflate` and `none`. | `none` |
| N/A | `--deflate` | Shorthand for `-c deflate`. | N/A |
| `-f <name>` | `--name=<name>` | The name to use when generating package files. | The base name of the target directory. |
| `-m <path>` | `--mappings=<path>` | Path to a CSV file providing supplemental media type mappings (see below for details). | (empty) |
| `-n <name>` | `--namespace=<name>` | The namespace of the generated package. The package name as specified by the `-f` flag.
| `-p <size>` | `--part-size=<size>` | The maximum size in bytes for part files. The value (if provided) must be at least 4096 bytes. | 0 (unlimited) |

#### `unpack` params

The following parameters are valid only for the `unpack` verb.

| Shorthand | Longhand | Description | Default |
| :-- | :-- | :-- | :-- |
| `-r <path>` | `--resource=<path>` | Extracts a specific resource from the target package. | (empty) |

### Building

To build arptool, first clone the repository recursively and then build with CMake.

```bash
git clone --recurse-submodules https://github.com/caseif/arptool.git
cd arptool
mkdir build
cd build
cmake ..
cmake --build .
```

### Supplemental Media Type Mappings

Per the specification of [libarp][1], `arptool` provides a means for user-defined [media type][2] mappings to be
supplied when creating a package. These mappings are to be defined in a 2-column headerless CSV, with the first column
containing file extensions and the second containing media types as defined by the libarp spec.

An example CSV is provided in the `examples` directory of this repository.

### License

arptool is made available under the [MIT License][3].

[libarp][1], the foundation of arptool, is also made available under the [MIT License][3].

The ARP specification, which libarp implements, is made available under the [Apache License, Version 2.0][4].

[1]: https://github.com/caseif/libarp
[2]: https://github.com/caseif/libarp/blob/master/doc/SPEC.md#6-media-types
[3]: https://opensource.org/licenses/MIT
[4]: https://opensource.org/licenses/Apache-2.0
