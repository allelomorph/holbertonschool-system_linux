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

http_req DELETE /todos?id=0 #404
http_req POST /todos -d 'title=Holberton' -d 'description=School' #201
http_req POST /todos -d 'title=Holberton2' -d 'description=School2' #201
http_req DELETE /todos?id=0 #204
http_req GET /todos #200
