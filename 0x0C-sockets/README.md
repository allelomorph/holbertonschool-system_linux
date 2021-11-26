# (441) 0x0C. Sockets
Specializations > System programming & Algorithm > Linux Programming

### Project author
Alexandre Gautier

### Assignment dates
06-21-2021 to 07-01-2021

### Description
Introduction to networking in C using sockets; building a REST API on a simple HTTP server.

### Requirements

### REST API
The goal of this project is to build a simple HTTP REST API in C, allowing us to manage a list of things to do. Our database will be in RAM, meaning there won’t be any persistent storage. Every time the server is started, the list of todos is empty.

A todo contains at least the following fields:

* `id` -> Positive integer, starting from 0
* `title` -> String
* `description` -> String

Here are the different routes that will have to be implemented:

* `/todos`
    * `POST` -> Creates a new todo
    * `GET` -> Retrieve all the todos
* `/todos?id={id}` (where `{id}` is a positive integer)
    * `GET` -> Retrieve the todo with the corresponding id
    * `DELETE` -> Delete the todo with the corresponding id

### Provided file(s)
* [`1-client`](./tests/1-client) [`2-server`](./tests/2-server) [`3-client`](./tests/3-client)

---

## Mandatory Tasks

### :white_check_mark: 0. Listen
Write a program that opens an `IPv4/TCP` socket, and listens to traffic on port `12345` (any address).

* Your program must hang indefinitely (eventually to be killed by a signal)
* You don’t have to accept entering connections
* Your program’s output won’t be checked, you can output any information you like

In a terminal:
```bash
$ ./0-server
Server listening on port 12345
```

In a second terminal:
```bash
$ lsof -i :12345
COMMAND    PID    USER   FD   TYPE DEVICE SIZE/OFF NODE NAME
0-server 24895 vagrant    3u  IPv4 251230      0t0  TCP *:12345 (LISTEN)
$ killall 0-server
$
```

File(s): [`0-server.c`](./0-server.c)\
Compiled: `gcc -Wall -Wextra -Werror -pedantic -o 0-server 0-server.c`

### :white_check_mark: 1. Accept
Write a program that opens an `IPv4/TCP` socket, and listens to traffic on port `12345` (any address).

* Your program must accept an entering connection, print the IP address of the connected client, and close the connection right after
* Output: See example below
* In order to test your server, you can use the program [1-client](./tests/1-client)

In a terminal:
```bash
$ ./1-server
Server listening on port 12345
Client connected: 127.0.0.1
$

In a second terminal:
```bash
$ ./1-client localhost 12345
Connected to localhost:12345
$
```

File(s): [`1-server.c`](./1-server.c)\
Compiled: `gcc -Wall -Wextra -Werror -pedantic -o 1-server 1-server.c`

### :white_check_mark: 2. Client
Write a program that connects to a listening server

* Usage: `2-client <host> <port>`
* Where `port` is an `unsigned short`
* If the number of command-line arguments is not sufficient, yout program must display: `Usage: %s <host> <port>` (where `%s` must be replaced by the name of the program), followed by a new line and exit with `EXIT_FAILURE`
* You can assume that `<port>` will be a positive integer
* Once your client established a connection with the server, it must print a message, and exit successfully
* In order to test your server, you can use the program [2-server](./tests/2-server)

In a terminal:
```bash
$ ./2-server
Server listening on port 12345
Client connected: 127.0.0.1
```

In a second terminal:
```bash
$ ./2-client localhost 12345
Connected to localhost:12345
$
```

File(s): [`2-client.c`](./2-client.c)\
Compiled: `gcc -Wall -Wextra -Werror -pedantic -o 2-client 2-client.c`

### :white_check_mark: 3. Roger
Write a program that opens an `IPv4/TCP` socket, and listens to traffic on port `12345` (any address).

* Your program must:
    * Accept an entering connection
    * Print the IP address of the connected client
    * Wait for an incoming message from the connected client
    * Print the received message
    * Close the connection with the client
    * Exit
* In order to test your server, you can use the program [3-client](./tests/3-client)

In a terminal:
```bash
$ ./3-server
Server listening on port 12345
Client connected: 127.0.0.1
Message received: "Holberton School !!!"
$
```

In a second terminal:
```bash
$ ./3-client localhost 12345 'Holberton School !!!'
Connected to localhost:12345
Sending the message: "Holberton School !!!"
Sent
$
```

File(s): [`3-server.c`](./3-server.c)\
Compiled: `gcc -Wall -Wextra -Werror -pedantic -o 3-server 3-server.c`

### :white_check_mark: 4. REST API - The Request
Write a program that opens an `IPv4/TCP` socket, and listens to traffic on port `8080` (any address).

* Your program must:
    * Accept an entering connection
    * Print the IP address of the connected client
    * Wait for an incoming message from the connected client
    * Print the full received HTTP request
    * Print the break-down of the first line of the received HTTP request (See example)
    * Send back a response to the connected client (HTTP 200 OK)
    * Close the connection with the client
    * Wait for the next connection
* The client will send an HTTP/1.1 request. You can find the RFC describing the HTTP protocol version 1.1 [here](https://datatracker.ietf.org/doc/html/rfc2616). (Tip: RFC sections 4, 5, 6 and 10)
* You can assume that the requests sent to your server will always be valid HTTP/1.1 requests.

In a terminal:
```bash
$ ./todo_api_0
Server listening on port 8080
Client connected: 127.0.0.1
Raw request: "GET /test HTTP/1.1
User-Agent: curl/7.35.0
Host: localhost:8080
Accept: */*

"
Method: GET
Path: /test
Version: HTTP/1.1
Client connected: 127.0.0.1
Raw request: "POST /holberton HTTP/1.1
User-Agent: curl/7.35.0
Host: localhost:8080
Accept: */*

"
Method: POST
Path: /holberton
Version: HTTP/1.1
^C
$
```

In a second terminal:
```bash
$ curl -D - 'http://localhost:8080/test' 2> /dev/null | cat -e
HTTP/1.1 200 OK^M$
^M$
$ curl -D - -X POST 'http://localhost:8080/holberton' 2> /dev/null | cat -e
HTTP/1.1 200 OK^M$
^M$
$
```

File(s): [`API_server.h`](./API_server.h) [`4-5-6-7-API_server.c`](./4-5-6-7-API_server.c) [`4-5-6-7-setSignalHandler.c`](./4-5-6-7-setSignalHandler.c) [`tokenizeBySubstr.c`](./tokenizeBySubstr.c) [`parseRequest.c`](./parseRequest.c) [`4-5-6-7-valid_fields.c`](./4-5-6-7-valid_fields.c) [`parseRequestLine.c`](./parseRequestLine.c) [`4-5-6-7-HTTP_response.c`](./) [`cleanup.c`](./cleanup.c) [`4-printRequest.c`](./4-printRequest.c)\
Compiled: `make todo_api_0`

### :white_check_mark: 5. REST API - Queries
Write a program that opens an `IPv4/TCP` socket, and listens to traffic on port `8080` (any address).

* Your program must:
    * Accept an entering connection
    * Print the IP address of the connected client
    * Wait for an incoming message from the connected client
    * Print the full received HTTP request
    * Print the the request path and all queries key/value pairs of the received HTTP request (See example)
    * Send back a response to the connected client (HTTP 200 OK)
    * Close the connection with the client
    * Wait for the next connection
* The client will send an HTTP/1.1 request. You can find the RFC describing the HTTP protocol version 1.1 [here](https://datatracker.ietf.org/doc/html/rfc2616). (Tip: RFC sections 4, 5, 6 and 10)
* You can assume that the requests sent to your server will always be valid HTTP/1.1 requests.

In a terminal:
```bash
$ ./todo_api_1
Server listening on port 8080
Client connected: 127.0.0.1
Raw request: "GET /test?value1=holberton&value2=school HTTP/1.1
User-Agent: curl/7.35.0
Host: localhost:8080
Accept: */*

"
Path: /test
Query: "value1" -> "holberton"
Query: "value2" -> "school"
^C
$ 
```

In a second terminal:
```bash
$ curl -D - 'http://localhost:8080/test?value1=holberton&value2=school' 2> /dev/null | cat -e
HTTP/1.1 200 OK^M$
^M$
$ 
```

File(s): [`API_server.h`](./API_server.h) [`4-5-6-7-API_server.c`](./4-5-6-7-API_server.c) [`4-5-6-7-setSignalHandler.c`](./4-5-6-7-setSignalHandler.c) [`tokenizeBySubstr.c`](./tokenizeBySubstr.c) [`parseRequest.c`](./parseRequest.c) [`4-5-6-7-valid_fields.c`](./4-5-6-7-valid_fields.c) [`parseRequestLine.c`](./parseRequestLine.c) [`4-5-6-7-HTTP_response.c`](./) [`cleanup.c`](./cleanup.c) [`5-printRequest.c`](./5-printRequest.c)\
Compiled: `make todo_api_1`

### :white_check_mark: 6. REST API - Headers
Write a program that opens an `IPv4/TCP` socket, and listens to traffic on port `8080` (any address).

* Your program must:
    * Accept an entering connection
    * Print the IP address of the connected client
    * Wait for an incoming message from the connected client
    * Print the full received HTTP request
    * Print the the headers key/value pairs of the received HTTP request (See example)
    * Send back a response to the connected client (HTTP 200 OK)
    * Close the connection with the client
    * Wait for the next connection
* The client will send an HTTP/1.1 request. You can find the RFC describing the HTTP protocol version 1.1 [here](https://datatracker.ietf.org/doc/html/rfc2616). (Tip: RFC sections 4, 5, 6 and 10)
* You can assume that the requests sent to your server will always be valid HTTP/1.1 requests.

In a terminal:
```bash
$ ./todo_api_2
Server listening on port 8080
Client connected: 127.0.0.1
Raw request: "GET /test HTTP/1.1
User-Agent: curl/7.35.0
Host: localhost:8080
Accept: */*

"
Header: "User-Agent" -> "curl/7.35.0"
Header: "Host" -> "localhost:8080"
Header: "Accept" -> "*/*"
^C
$ 
```

In a second terminal:
```bash
$ curl -D - 'http://localhost:8080/test' 2> /dev/null | cat -e
HTTP/1.1 200 OK^M$
^M$
$ 
```

File(s): [`API_server.h`](./API_server.h) [`4-5-6-7-API_server.c`](./4-5-6-7-API_server.c) [`4-5-6-7-setSignalHandler.c`](./4-5-6-7-setSignalHandler.c) [`tokenizeBySubstr.c`](./tokenizeBySubstr.c) [`parseRequest.c`](./parseRequest.c) [`4-5-6-7-valid_fields.c`](./4-5-6-7-valid_fields.c) [`parseRequestLine.c`](./parseRequestLine.c) [`4-5-6-7-HTTP_response.c`](./) [`cleanup.c`](./cleanup.c) [`6-printRequest.c`](./6-printRequest.c)\
Compiled: `make todo_api_2`

### :white_check_mark: 7. REST API - Body parameters
Write a program that opens an `IPv4/TCP` socket, and listens to traffic on port `8080` (any address).

* Your program must:
    * Accept an entering connection
    * Print the IP address of the connected client
    * Wait for an incoming message from the connected client
    * Print the full received HTTP request
    * Print the request path and all body parameters key/value pairs of the received HTTP request (See example)
    * Send back a response to the connected client (HTTP 200 OK)
    * Close the connection with the client
    * Wait for the next connection
* The client will send an HTTP/1.1 request. You can find the RFC describing the HTTP protocol version 1.1 [here](https://datatracker.ietf.org/doc/html/rfc2616). (Tip: RFC sections 4, 5, 6 and 10)
* You can assume that the requests sent to your server will always be valid HTTP/1.1 requests.


In a terminal:
```bash
$ ./todo_api_3
Server listening on port 8080
Client connected: 127.0.0.1
Raw request: "POST /test HTTP/1.1
User-Agent: curl/7.35.0
Host: localhost:8080
Accept: */*
Content-Length: 30
Content-Type: application/x-www-form-urlencoded

value1=holberton&value2=school"
Path: /test
Body param: "value1" -> "holberton"
Body param: "value2" -> "school"
^C
$ 
```

In a second terminal:
```bash
$ curl -D - -X POST 'http://localhost:8080/test' -d 'value1=holberton&value2=school' 2> /dev/null | cat -e
HTTP/1.1 200 OK^M$
^M$
$ 
```

File(s): [`API_server.h`](./API_server.h) [`4-5-6-7-API_server.c`](./4-5-6-7-API_server.c) [`4-5-6-7-setSignalHandler.c`](./4-5-6-7-setSignalHandler.c) [`tokenizeBySubstr.c`](./tokenizeBySubstr.c) [`parseRequest.c`](./parseRequest.c) [`4-5-6-7-valid_fields.c`](./4-5-6-7-valid_fields.c) [`parseRequestLine.c`](./parseRequestLine.c) [`4-5-6-7-HTTP_response.c`](./) [`cleanup.c`](./cleanup.c) [`7-printRequest.c`](./7-printRequest.c)\
Compiled: `make todo_api_3`

### :white_check_mark: 8. REST API - Create TODO
Now that you can parse an HTTP request, it’s time to implement our REST API. Start by handling the `POST` method for the path `/todos`

* Method: `POST`
* Path: `/todos`
* Required queries: None
* Required headers:
    * `Content-Length`
* Required body parameters:
    * `title` -> String
    * `description`: -> String
    * NOTE: since the format is URL encoded, you don’t have to handle special characters (spaces, quotes, …). Only non-encoded characters need be considered in testing.
* Creates a todo and adds it to the list
* Response:
    * `201 Created` -> Todo has been created.
        * Response must include the header `Content-Type: application/json`
        * Response must include the json representation of the created todo in its body
    * `411 Length Required` -> Missing the `Content-Length` header
    * `422 Unprocessable Entity` -> Missing either the `title` or `description` body parameter
* You are free to print any information you like on stdout/stderr on the server side
* You are free to use any syscall/standard library call
* You are free to define and use any data structure

Also, starting from this task, your server must return a `404 Not Found` for any path that is not recognized, or for a method that is not implemented.

In a first terminal: (just as an example, you are free to output the information of your choice)
```bash
$ ./todo_api_4
Server listening on port 8080
127.0.0.1 POST /todos -> 201 Created
127.0.0.1 POST /todos -> 422 Unprocessable Entity
127.0.0.1 POST /holberton -> 404 Not Found
127.0.0.1 GET /todos -> 404 Not Found
^C
$ 
```

In a second terminal:
```bash
$ curl -D - -X POST 'http://localhost:8080/todos' -d 'title=Dishes&description=Not_really_urgent' 2> /dev/null | cat -e
HTTP/1.1 201 Created^M$
Content-Length: 59^M$
Content-Type: application/json^M$
^M$ 
{"id":0,"title":"Dishes","description":"Not_really_urgent"}$
$ curl -D - -X POST 'http://localhost:8080/todos' -d 'title=Homework' 2> /dev/null | cat -e
HTTP/1.1 422 Unprocessable Entity^M$
^M$ 
$ curl -D - -X POST 'http://localhost:8080/holberton' -d 'title=Dishes&description=Not_really_urgent' 2> /dev/null | cat -e
HTTP/1.1 404 Not Found^M$
^M$ 
$ curl -D - 'http://localhost:8080/todos' 2> /dev/null | cat -e
HTTP/1.1 404 Not Found^M$
^M$
$ 
```

File(s): [`API_server.h`](./API_server.h) [`8-9-10-11-API_server.c`](./8-9-10-11-API_server.c) [`8-9-10-11-setSignalHandler.c`](./8-9-10-11-setSignalHandler.c) [`tokenizeBySubstr.c`](./tokenizeBySubstr.c) [`parseRequest.c`](./parseRequest.c) [`8-valid_fields.c`](./8-valid_fields.c) [`parseRequestLine.c`](./parseRequestLine.c) [`8-9-10-11-HTTP_response.c`](./8-9-10-11-HTTP_response.c) [`cleanup.c`](./cleanup.c) [`methods.c`](./methods.c) [`method_utils.c`](./method_utils.c) [`todos.c`](./todos.c)\
Compiled: `make todo_api_4`

### :white_check_mark: 9. REST API - Retrieve all TODOs
Handle the `GET` method for the path `/todos`

* Method: `GET`
* Path: `/todos`
* Required queries: None
* Required headers: None
* Required body parameters: None
* Retrieves the list of all the todos
* Response:
    * `200 OK` -> Todos retrieved
        * Response must include the header `Content-Type: application/json`
        * Response must include the json representation of the list of todos in its body
        * NOTE: If there is no todo in the list, your server must return an empty list
* You are free to print any information you like on stdout/stderr on the server side
* You are free to use any syscall/standard library call
* You are free to define and use any data structure

In a first terminal:  (just as an example, you are free to output the information of your choice)
```bash
$ ./todo_api_5
Server listening on port 8080
127.0.0.1 GET /todos -> 200 OK
127.0.0.1 POST /todos -> 201 Created
127.0.0.1 GET /todos -> 200 OK
127.0.0.1 POST /todos -> 201 Created
127.0.0.1 GET /todos -> 200 OK
^C
$ 
```

In a second terminal:
```bash
$ curl -D - 'http://localhost:8080/todos' 2> /dev/null | cat -e
HTTP/1.1 200 OK^M$
Content-Length: 2^M$
Content-Type: application/json^M$
^M$
[]$
$ curl -D - -X POST 'http://localhost:8080/todos' -d 'title=Dishes&description=Not_really_urgent' 2> /dev/null | cat -e
HTTP/1.1 201 Created^M$
Content-Length: 59^M$
Content-Type: application/json^M$
^M$
{"id":0,"title":"Dishes","description":"Not_really_urgent"}$ 
$ curl -D - 'http://localhost:8080/todos' 2> /dev/null | cat -e
HTTP/1.1 200 OK^M$
Content-Length: 61^M$
Content-Type: application/json^M$
^M$
[{"id":0,"title":"Dishes","description":"Not_really_urgent"}]$ 
$ curl -D - -X POST 'http://localhost:8080/todos' -d 'title=Laundry&description=To_be_done_by_yesterday' 2> /dev/null | cat -e
HTTP/1.1 201 Created^M$
Content-Length: 66^M$
Content-Type: application/json^M$
^M$
{"id":1,"title":"Laundry","description":"To_be_done_by_yesterday"}$ 
$ curl -D - 'http://localhost:8080/todos' 2> /dev/null | cat -e
HTTP/1.1 200 OK^M$
Content-Length: 128^M$
Content-Type: application/json^M$
^M$
[{"id":0,"title":"Dishes","description":"Not_really_urgent"},{"id":1,"title":"Laundry","description":"To_be_done_by_yesterday"}]$ 
$ 
```

File(s): [`API_server.h`](./API_server.h) [`8-9-10-11-API_server.c`](./8-9-10-11-API_server.c) [`8-9-10-11-setSignalHandler.c`](./8-9-10-11-setSignalHandler.c) [`tokenizeBySubstr.c`](./tokenizeBySubstr.c) [`parseRequest.c`](./parseRequest.c) [`9-10-valid_fields.c`](./9-10-valid_fields.c) [`parseRequestLine.c`](./parseRequestLine.c) [`8-9-10-11-HTTP_response.c`](./8-9-10-11-HTTP_response.c) [`cleanup.c`](./cleanup.c) [`methods.c`](./methods.c) [`method_utils.c`](./method_utils.c) [`todos.c`](./todos.c)\
Compiled: `make todo_api_5`

## Advanced Tasks

### :white_check_mark: 10. REST API - Retrieve a single TODO
Handle the `GET` method for the path `/todos?id={id}`

* Method: `GET`
* Path: `/todos`
* Required queries:
    * `id` -> Positive integer
* Required headers: None
* Required body parameters: None
* Retrieves a single todo, identified by its id
* Response:
    * `200 OK` -> Todo retrieved
        * Response must include the header Content-Type: `application/json`
        * Response must include the json representation of the list of todos in its body
    * `404 Not Found` -> No todo with this id
* You are free to print any information you like on stdout/stderr on the server side
* You are free to use any syscall/standard library call
* You are free to define and use any data structure

In a first terminal: (just as an example, you are free to output the information of your choice)
```bash
$ ./todo_api_6
Server listening on port 8080
127.0.0.1 POST /todos -> 201 Created
127.0.0.1 POST /todos -> 201 Created
127.0.0.1 GET /todos -> 200 OK
127.0.0.1 GET /todos -> 200 OK
127.0.0.1 GET /todos -> 404 Not Found
127.0.0.1 GET /todos -> 200 OK
^C
$
```

In a second terminal:
```bash
$ curl -D - -X POST 'http://localhost:8080/todos' -d 'title=Dishes&description=Not_really_urgent' 2> /dev/null | cat -e
HTTP/1.1 201 Created^M$
Content-Length: 59^M$
Content-Type: application/json^M$
^M$
{"id":0,"title":"Dishes","description":"Not_really_urgent"}$ 
$ curl -D - -X POST 'http://localhost:8080/todos' -d 'title=Laundry&description=To_be_done_by_yesterday' 2> /dev/null | cat -e
HTTP/1.1 201 Created^M$
Content-Length: 66^M$
Content-Type: application/json^M$
^M$
{"id":1,"title":"Laundry","description":"To_be_done_by_yesterday"}$ 
$ curl -D - 'http://localhost:8080/todos?id=0' 2> /dev/null | cat -e
HTTP/1.1 200 OK^M$
Content-Length: 59^M$
Content-Type: application/json^M$
^M$
{"id":0,"title":"Dishes","description":"Not_really_urgent"}$ 
$ curl -D - 'http://localhost:8080/todos?id=1' 2> /dev/null | cat -e
HTTP/1.1 200 OK^M$
Content-Length: 66^M$
Content-Type: application/json^M$
^M$
{"id":1,"title":"Laundry","description":"To_be_done_by_yesterday"}$ 
$ curl -D - 'http://localhost:8080/todos?id=2' 2> /dev/null | cat -e
HTTP/1.1 404 Not Found^M$
^M$
$ curl -D - 'http://localhost:8080/todos' 2> /dev/null | cat -e
HTTP/1.1 200 OK^M$
Content-Length: 128^M$
Content-Type: application/json^M$
^M$
[{"id":0,"title":"Dishes","description":"Not_really_urgent"},{"id":1,"title":"Laundry","description":"To_be_done_by_yesterday"}]$ 
$ 
```

File(s): [`API_server.h`](./API_server.h) [`8-9-10-11-API_server.c`](./8-9-10-11-API_server.c) [`8-9-10-11-setSignalHandler.c`](./8-9-10-11-setSignalHandler.c) [`tokenizeBySubstr.c`](./tokenizeBySubstr.c) [`parseRequest.c`](./parseRequest.c) [`9-10-valid_fields.c`](./9-10-valid_fields.c) [`parseRequestLine.c`](./parseRequestLine.c) [`8-9-10-11-HTTP_response.c`](./8-9-10-11-HTTP_response.c) [`cleanup.c`](./cleanup.c) [`methods.c`](./methods.c) [`method_utils.c`](./method_utils.c) [`todos.c`](./todos.c)\
Compiled: `make todo_api_6`

### :white_check_mark: 11. REST API - Delete a TODO
Handle the `DELETE` method for the path `/todos?id={id}`

* Method: `DELETE`
* Path: `/todos`
* Required queries:
    * `id` -> Positive integer
* Required headers: None
* Required body parameters: None
* Deletes a todo, identified by its id
* Response:
    * `204 No Content` -> Todo deleted
    * `404 Not Found` -> No todo with this id
* You are free to print any information you like on stdout/stderr on the server side
* You are free to use any syscall/standard library call
* You are free to define and use any data structure

In a first terminal: (just as an example, you are free to output the information of your choice)
```bash
$ ./todo_api_7
Server listening on port 8080
127.0.0.1 POST /todos -> 201 Created
127.0.0.1 POST /todos -> 201 Created
127.0.0.1 GET /todos -> 200 OK
127.0.0.1 DELETE /todos -> 204 No Content
127.0.0.1 GET /todos -> 200 OK
127.0.0.1 DELETE /todos -> 404 Not Found
^C
$
```

In a second terminal:
```bash
$ curl -D - -X POST 'http://localhost:8080/todos' -d 'title=Dishes&description=Not_really_urgent' 2> /dev/null | cat -e
HTTP/1.1 201 Created^M$
Content-Length: 59^M$
Content-Type: application/json^M$
^M$
{"id":0,"title":"Dishes","description":"Not_really_urgent"}$ 
$ curl -D - -X POST 'http://localhost:8080/todos' -d 'title=Laundry&description=To_be_done_by_yesterday' 2> /dev/null | cat -e
HTTP/1.1 201 Created^M$
Content-Length: 66^M$
Content-Type: application/json^M$
^M$
{"id":1,"title":"Laundry","description":"To_be_done_by_yesterday"}$ 
$ curl -D - 'http://localhost:8080/todos' 2> /dev/null | cat -e
HTTP/1.1 200 OK^M$
Content-Length: 128^M$
Content-Type: application/json^M$
^M$
[{"id":0,"title":"Dishes","description":"Not_really_urgent"},{"id":1,"title":"Laundry","description":"To_be_done_by_yesterday"}]$ 
$ curl -D - -X DELETE 'http://localhost:8080/todos?id=0' 2> /dev/null | cat -e
HTTP/1.1 204 No Content^M$
^M$
$ curl -D - 'http://localhost:8080/todos' 2> /dev/null | cat -e
HTTP/1.1 200 OK^M$
Content-Length: 68^M$
Content-Type: application/json^M$
^M$
[{"id":1,"title":"Laundry","description":"To_be_done_by_yesterday"}]$ 
$ curl -D - -X DELETE 'http://localhost:8080/todos?id=10' 2> /dev/null | cat -e
HTTP/1.1 404 Not Found^M$
^M$
$ 
```

File(s): [`API_server.h`](./API_server.h) [`8-9-10-11-API_server.c`](./8-9-10-11-API_server.c) [`8-9-10-11-setSignalHandler.c`](./8-9-10-11-setSignalHandler.c) [`tokenizeBySubstr.c`](./tokenizeBySubstr.c) [`parseRequest.c`](./parseRequest.c) [`11-valid_fields.c`](./11-valid_fields.c) [`parseRequestLine.c`](./parseRequestLine.c) [`8-9-10-11-HTTP_response.c`](./8-9-10-11-HTTP_response.c) [`cleanup.c`](./cleanup.c) [`methods.c`](./methods.c) [`method_utils.c`](./method_utils.c) [`todos.c`](./todos.c)\
Compiled: `make todo_api_7`

---

## Student
* **Samuel Pomeroy** - [allelomorph](github.com/allelomorph)
