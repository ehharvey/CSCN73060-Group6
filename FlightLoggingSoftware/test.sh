#! /bin/bash

for i in {1..1000}
do
    echo "Testing with client ID $i"
   ./bazel-bin/Client/Main/Main localhost "$i" &
done