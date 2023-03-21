#! /bin/bash

# "10.192.194.124"

for i in {1..100}
do
    echo "Testing with client ID $i"
   ./bazel-bin/Client/Main/Main "10.192.194.124" "$i" &
done