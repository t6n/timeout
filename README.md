# timeout

```shell
usage: timeout -t timeout command
```

## example
```shell
$ timeout -t 4 sleep 3 && echo "bar"
bar
```
```shell
$ timeout -t 4 sleep 5 && echo "bar"
Time Limit of 4 Exceeded!
```
