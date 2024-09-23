# rrename

Rename files in the current directory using an opinionated naming scheme.

- lowercase all ASCII characters
- replace ampersands `&` with `and`
- strip single `'` and double `"` quotes
- replace spaces and punctuation with underscores `_`
- replace `__` and `_-` with single underscore `_`
- strip trailing punctuation
- replace `_.` and `..` with single period `.`
- non-ASCII characters are not modified

## Installation

Use make to compile and install.

```shell
make
make install
```
    
## Example Output

```console
$ rrename
el niño.txt => el_niño.txt
I'm WAITING....txt => im_waiting.txt
```

Run `rrename -h` to see all options.

## Running Tests

To run the tests, execute the following command.

```shell
make tests
```
