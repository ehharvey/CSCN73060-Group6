#! /bin/bash

for i in {1..10000}
do
    echo "Testing with client ID $i"
./bazel-bin/Client/Main/Main "10.192.194.124" "$i" &
done
