#!/usr/bin/env sh
# sends the signal SIGQUIT to a process, given its PID
if [ $# -ne 1 ] || ! echo "$1" | grep -Eq '^-?[0-9]+$'; then
    echo "Usage: $0 <pid>"
    exit 1
fi

kill -s QUIT "$1"
