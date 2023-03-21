#! /bin/bash

# "10.192.194.124"

for i in {2..2}
do
    echo "Testing with client ID $i"
   ./bazel-bin/Client/Main/Main "10.144.51.10" "$i"
done