use lib 'lib';
use Test::Nginx::Socket;

#repeat_each(2);

plan tests => repeat_each() * 2 * blocks();

run_tests();

__DATA__

=== TEST 1: hello_world
--- config
    location /hello {
        hello_world;
    }
--- request
    GET /hello
--- response_body
<!DOCTYPE html>
<html>
<head>
<title>Hello, World with nginx!</title>
</head>
<body>
<p>Hello, World!</p>
</body>
</html>
