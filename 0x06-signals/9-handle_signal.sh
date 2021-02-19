#!/usr/bin/env sh
# sets a handler for the signals SIGABRT, SIGIO and SIGTERM
trap "echo Nope" ABRT IO TERM
