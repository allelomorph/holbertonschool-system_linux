#!/bin/bash

if [ $# -lt 1 ]
then
        echo "Usage: $0 <command>" >&2
        exit 1
fi

rm -rf todo_api_output
rm -rf curl_output

function http_req
{
        curl -w "%{http_code}\n" -s -o /dev/null -X "$1" 'http://localhost:8080'"$2" >> curl_output 2>&1
}

(timeout --preserve-status 5s "$1" > todo_api_output 2>&1 &)
sleep 1

http_req GET /
http_req POST /holberton_school
http_req HEAD /holberton/school
