#pass the destination folder as the argument

#!/bin/bash
# usage-message.sh
if [ -z "$1" ]; then
        echo "Source directory missing"
        exit 1
elif [ -z "$2" ]; then
        echo "Destination filename missing"
        exit 1
fi

find $1 -name 'bgmodel.txt' -exec cp --parents \{\} ./$2 \;
