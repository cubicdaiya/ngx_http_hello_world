# nginx_http_hello_world

Hello, World with nginx

```conf
location ~ /hello_world$ {
    hello_world;
}
```

```sh
$ curl -s "http://127.0.0.1:80/hello_world"
Hello, World!
$
```
