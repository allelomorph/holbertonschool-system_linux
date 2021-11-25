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
        method=$1;
        path=$2;
        shift 2
        curl -w "%{http_code}\n" -s -o /dev/null -X "$method" 'http://localhost:8080'"$path" "$@" >> curl_output 2>&1
}

(timeout --preserve-status 5s "$1" > todo_api_output 2>&1 &)
sleep 1

http_req GET / -H "Holberton: School"
http_req GET / -H "Holberton: School" -H "Street: 972-Mission"
http_req GET / -H "Holberton: School" -H "Street: 972-Mission" -H "Upfront-tuition: Not-a-Number" -H "Duration: 2-Years"
