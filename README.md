DSON parser library

This is just some random goofing around to implement http://dogeon.org

Frankly, the data structures kind suck, but I wasn't going for pretty or performant.

$ make all

Will build the command line testing tool.  Someday I make come back and turn it into a properly library, but that seems unlikely.

$ ./test 'such "doge" is so "pretty" and "fluffy" many, "cat" is so "mean" also "ugly" many. "pi" is 3.1103755242! "troof" is yes? "lies" is no, "tummy" is empty, "life" is 52, "live" is 5.2 very 1 wow'

```
Object (8) {
  doge:
    Array (2) [
      string(6) "pretty"
      string(6) "fluffy"
    ]
  cat:
    Array (2) [
      string(4) "mean"
      string(4) "ugly"
    ]
  pi:
    double(3.141593)
  troof:
    bool(true)
  lies:
    bool(false)
  tummy:
    NULL
  life:
    long(42)
  live:
    double(42.000000)
}
```
