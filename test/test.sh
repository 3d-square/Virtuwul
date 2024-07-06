#!/bin/bash
#

function test_case(){
   ../interpreter cases/$1 > ./actual/$2
   diff ./actual/$2 ./expected/$2 

   if [[ $? -ne 0 ]]; then
      echo "Test Failed - $1"
   else
      echo "Test Passed - $1"
   fi
}

test_case numerical.txt test1

test_case bools.txt test2
