#!/bin/bash

# Unknown why checker script had this test
# if [ $# -lt 1 ]
# then
#         echo "Usage: $0 <command>" >&2
#         exit 1
# fi

rm -rf todo_api_output
rm -rf curl_output

function http_req
{
        method=$1;
        path=$2;
        shift 2
        curl -D - -X "$method" 'http://localhost:8080'"$path" "$@" >> curl_output 2> /dev/null
}

(timeout --preserve-status 5s "$1" > /dev/null 2>&1 &)
sleep 1

http_req GET /todos #200
http_req POST /todos -d 'title=Holberton' -d 'description=School' #201
http_req GET /todos #200
http_req POST /todos -d 'title=Holberton2' -d 'description=School2' #201
http_req GET /todos #200

http_req GET /todo #404
