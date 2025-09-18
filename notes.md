# Use conventional commits
https://www.conventionalcommits.org/en/v1.0.0/

# Structure of store file
## JSON variant
```json
{
    "last-edit": "May 27 19:46",
    "settings: {
        "default-editor": "vim"
    },
    "entries": [
        {
            path: "/home/gcrg/.nbpp/repo/file.md",
            keys: [ // It should be searchable by the keys
                "foo",
                "daily",
                "something else"
            ]
        },
    ]
}
```

## Custom variant
**Core points**:
* Keys are postfixed with a colon, `:`
* Values are terminated by a newline
* Values in a list is separated by `\,`
* Before opening an entry, the default path, `$HOME/.nbpp/` is prefixed to the
  `key` in order to form the system path of the file contents.
* The environment variable `NBPP_HOME` will have priority over the default value
  of the path.
* The environment variable `NBPP_LOG_LEVEL` will be used to set the log level.
  Default log level shall be `error`
```txt

[settings]
default-editor:vim

[info]
last-edit:May 27 19\:46

[entries]
key0:value0\,value1\,value2\,value3
key1:value0\,value1\,value2\,value3

```
