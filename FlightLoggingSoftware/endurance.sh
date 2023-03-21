#! /bin/bash

while :
do
    echo "Starting iteration"
    for i in {1..500}
    do
        echo "Testing with client ID $i"
    ./bazel-bin/Client/Main/Main "10.192.194.124" "$i" &
    done
    sleep 250
    echo "Iteration Done"
done