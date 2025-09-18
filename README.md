# Documentation
## Settings
### `default-editor`
Sets the default editor. The value must be available in `$PATH`. Example:
```txt
[Cient info]
...

[Settings]
default-editor:vim

[Entries]
...
```


## Configuration path
By default nbpp will store configurations files at `$HOME/.config/nbpp`, unless
the environment variable `$NBPP_CONFIG_PATH` is set. The configuration file will
roughly look as follows:
```txt
[Cient info]
last-edit:569986840758197536

[Settings]
default-editor:vim

[Entries]
some-key:value
some-other-key:value0\,value1\,value2
```
