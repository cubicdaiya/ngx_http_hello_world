# nginx_http_hello_world

Hello, World with nginx

# Getting started

There is the configuration below.

```nginx
location ~ /hello_world$ {
    hello_world;
}
```

There is the response below.

```sh
$ curl -s "http://127.0.0.1:80/hello_world"
Hello, World!
$
```
