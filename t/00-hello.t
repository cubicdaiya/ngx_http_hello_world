use Test::Nginx::Socket;

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
Hello, World!
