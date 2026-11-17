# webserver_42

An HTTP/1.1 server written from scratch in C++98, built on non-blocking sockets and a single `poll()` loop.

## What it does

- Listens on several ports at once, each defined by its own `server { }` block.
- Serves static files over GET, with index files per location.
- Reads an NGINX-style config: `listen`, `host`, `location`, `root`, `index`, `methods`, `autoindex`.
- Answers with proper status codes: `200`, `404` for missing files, `405` when a method isn't allowed for a location, and `501` for methods not implemented yet.

Checked with curl against `config/default.conf`: `GET /` gives 200, `GET /nope` gives 404, `DELETE /` gives 405 and `POST /uploads` gives 501.

## Status

In progress: POST uploads, DELETE and CGI. The config parser already reads `methods GET POST DELETE` and the CGI settings, but requests other than GET currently return `501 Not Implemented`.

## Build and run

```bash
make                                  # builds ./webserv with -Wall -Wextra -Werror -std=c++98
./webserv config/default.conf         # then open http://127.0.0.1:8181/
make fclean
```

`run_webserv.sh` builds, starts the server with a config (default `config/default.conf`) and cleans up when it stops. `alias.sh` adds a `run` shortcut for it to your shell profile.

## How it works

`ServerManager` opens one non-blocking listening socket per configured port (`fcntl` with `O_NONBLOCK`) and watches all sockets in one `poll()` loop. When a client socket is readable, its bytes are added to that client's buffer until a full request is parsed (`Request`). `Router` matches the path to a `location` and checks the allowed methods, and `Response` builds the reply from the file on disk. Nothing blocks: reads and writes only happen when `poll()` says the socket is ready.

The config goes through a small `Lexer` and `Parser` into `Config` structures before any socket is opened.

## Project structure

- `srcs/`, `include/`: server, sockets, config lexer and parser, request, router, response
- `config/`: example configs (`default.conf`, `test.conf`)
- `www/`: files served by the example config
- `documents/`: the 42 subject

A 42 İstanbul project.
