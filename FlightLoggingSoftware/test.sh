#! /bin/bash

for i in {1..10}
do
    echo "Testing with client ID $i"
   ./bazel-bin/Client/Main/Main localhost "$i" &
done