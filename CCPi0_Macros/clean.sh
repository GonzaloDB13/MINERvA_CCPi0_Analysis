#!bin/bash


## Clean anything on current directory and daughters as well

find . -type f -name '*.o' -delete

find . -type f -name '*.so' -delete

find . -type f -name '*.d' -delete

find . -type f -name '*.pcm' -delete
